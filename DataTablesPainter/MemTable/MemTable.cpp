#include "MemTable.h"
#include "../Infrastructure/DateTime.h"


namespace MemoryCollection
{


GlobalSequenceNo::GlobalSequenceNo()
 : m_nIncNum( 0 ), m_nBaseData( 0 )
{
	m_nBaseData = DateTime::Now().DateToLong() % 1000000 * 10000000000;
}

GlobalSequenceNo& GlobalSequenceNo::GetObj()
{
	static GlobalSequenceNo		obj;

	return obj;
}

void GlobalSequenceNo::Reset()
{
	CriticalLock			lock( m_oLock );

	m_nIncNum = 0;
	m_nBaseData = DateTime::Now().DateToLong() % 1000000 * 10000000000;
}

unsigned __int64 GlobalSequenceNo::GetSeqNo()
{
	CriticalLock			lock( m_oLock );

	return m_nBaseData + m_nIncNum;
}

unsigned __int64 GlobalSequenceNo::GenerateSeq()
{
	CriticalLock			lock( m_oLock );

	return m_nBaseData + m_nIncNum++;
}


DynamicTable::TableMeta::TableMeta( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen )
	: m_nBindID( nBindID ), m_nRecordWidth( nRecordWidth ), m_nKeyStrLen( nKeyStrLen )
{
}

void DynamicTable::TableMeta::Clear()
{
	m_nBindID = 0;
	m_nKeyStrLen = 0;
	m_nRecordWidth = 0;
}

bool DynamicTable::TableMeta::IsEngaged()
{
	return 0 != m_nRecordWidth;
}

DynamicTable::DynamicTable()
	: m_oTableMeta( 0, 0 ), m_pRecordsBuffer( NULL )
	, m_nMaxBufferSize( 0 ), m_nCurrentDataSize( 0 )
{
}

DynamicTable::~DynamicTable()
{
	if( NULL != m_pRecordsBuffer )
	{
		m_oTableMeta.Clear();
		m_nMaxBufferSize = 0;
		m_nCurrentDataSize = 0;
		::free( m_pRecordsBuffer );
		m_pRecordsBuffer = NULL;
	}
}

void DynamicTable::Initialize( const DynamicTable::TableMeta& refMeta )
{
	CriticalLock	lock( m_oCSLock );

	Release();

	m_oTableMeta = refMeta;
}

void DynamicTable::Release()
{
	CriticalLock	lock( m_oCSLock );

	m_nCurrentDataSize = 0;
	m_oTableMeta.Clear();
	m_oHashTableOfIndex.Clear();

	if( NULL != m_pRecordsBuffer )
	{
		::free( m_pRecordsBuffer );
		m_pRecordsBuffer = NULL;
		m_nMaxBufferSize = 0;
	}
}

DynamicTable::TableMeta DynamicTable::GetMeta()
{
	return m_oTableMeta;
}

bool DynamicTable::SwapTableInSameArray( DynamicTable* pTable )
{
	if( NULL == pTable )
	{
		::printf( "DynamicTable::SwapTableInSameArray() : invalid argument pointer [Null]\n" );
		return false;
	}

	TableMeta		objTmpTableMeta = m_oTableMeta;
	m_oTableMeta = pTable->m_oTableMeta;
	pTable->m_oTableMeta = objTmpTableMeta;

	char*			pTmpBuffer = m_pRecordsBuffer;
	m_pRecordsBuffer = pTable->m_pRecordsBuffer;
	pTable->m_pRecordsBuffer = pTmpBuffer;

	unsigned int	nTmpMaxBufSize = m_nMaxBufferSize;
	m_nMaxBufferSize = pTable->m_nMaxBufferSize;
	pTable->m_nMaxBufferSize = nTmpMaxBufSize;

	unsigned int	nTmpCurrentDataSize = m_nCurrentDataSize;
	m_nCurrentDataSize = pTable->m_nCurrentDataSize;
	pTable->m_nCurrentDataSize = nTmpCurrentDataSize;

	CollisionHash	oTmpHashTable = m_oHashTableOfIndex;
	m_oHashTableOfIndex = pTable->m_oHashTableOfIndex;
	pTable->m_oHashTableOfIndex = oTmpHashTable;

	int		nOffset = (char*)this - (char*)pTable;
	m_oHashTableOfIndex.CoordinateNodePtr( nOffset );
	pTable->m_oHashTableOfIndex.CoordinateNodePtr( nOffset*-1 );

	return true;
}

unsigned int DynamicTable::GetRecordCount()
{
	return m_nCurrentDataSize/m_oTableMeta.m_nRecordWidth;
}

bool DynamicTable::EnlargeBuffer( unsigned long nAllocItemNum )
{
	unsigned int	nNewBufferSize = m_oTableMeta.m_nRecordWidth * nAllocItemNum;

	if( 0 == nAllocItemNum )
	{
		::printf( "DynamicTable::EnlargeBuffer() : invalid arguments [Zero]\n" );
		return false;
	}

	if( NULL == m_pRecordsBuffer )	///< 第一次分配，只分配一条记录的空间
	{
		if( NULL == (m_pRecordsBuffer = (char*)::malloc( nNewBufferSize )) )
		{
			::printf( "DynamicTable::EnlargeBuffer() : failed 2 ::malloc() buffer\n" );
			return false;
		}

		m_nMaxBufferSize = nNewBufferSize;
		::memset( m_pRecordsBuffer, 0, m_nMaxBufferSize );
	}
	else							///< 再次分配，每次都增加1024条记录的空间
	{
		char* pTemp = (char*)::malloc( m_nMaxBufferSize + nNewBufferSize );
		if( pTemp == NULL )
		{
			::printf( "DynamicTable::EnlargeBuffer() : failed 2 ::remalloc() buffer\n" );
			return false;
		}

		m_nMaxBufferSize += nNewBufferSize;
		::memset( pTemp, 0, nNewBufferSize );
		::memcpy( pTemp, m_pRecordsBuffer, m_nCurrentDataSize );

		::free( m_pRecordsBuffer );
		m_pRecordsBuffer = pTemp;
	}

	return true;
}

RecordBlock DynamicTable::SelectRecord( char* pKeyStr, unsigned int nKeyLen )
{
	try
	{
		if( NULL == pKeyStr || nKeyLen <= 0 )
		{
			::printf( "DynamicTable::SelectRecord() : invalid arguments\n" );
			return RecordBlock( NULL, 0 );
		}

		DyncRecord				objRecord( pKeyStr, nKeyLen );
		__int64					nDataSeqKey = objRecord.GetMainKey();
		CriticalLock			lock( m_oCSLock );
		struct T_ListNode*		pRecordPostion = m_oHashTableOfIndex[nDataSeqKey];

		if( NULL == pRecordPostion )
		{
			return RecordBlock( NULL, 0 );
		}

		unsigned int			nRecordOffset = m_oTableMeta.m_nRecordWidth * pRecordPostion->nDataPos;
		if( m_oTableMeta.m_nRecordWidth > (m_nMaxBufferSize-nRecordOffset) )	///< 是否有足够的后续长度( 需要大于等于DataBlockSize)
		{
			::printf( "DynamicTable::SelectRecord() : subscript out of range of memo-tables list\n" );
			return RecordBlock( NULL, 0 );
		}

		return RecordBlock( m_pRecordsBuffer+nRecordOffset, m_oTableMeta.m_nRecordWidth );
	}
	catch( std::exception& err )
	{
		::printf( "DynamicTable::SelectRecord() : exception : %s\n", err.what() );
	}
	catch( ... )
	{
		::printf( "DynamicTable::SelectRecord() : unknow exception\n" );
	}

	return RecordBlock( NULL, 0 );
}

int DynamicTable::DeleteRecord( char* pKeyStr, unsigned int nKeyLen, unsigned __int64& nDbSerialNo )
{
	try
	{
		if( NULL == pKeyStr || nKeyLen <= 0 )
		{
			::printf( "DynamicTable::DeleteRecord() : invalid arguments\n" );
			return -1;
		}

		DyncRecord				objRecord( pKeyStr, nKeyLen );
		__int64					nDataSeqKey = objRecord.GetMainKey();
		CriticalLock			lock( m_oCSLock );
		struct T_ListNode*		pRecordPostion = m_oHashTableOfIndex[nDataSeqKey];

		nDbSerialNo = GlobalSequenceNo::GetObj().GetSeqNo();
		if( NULL == pRecordPostion )
		{
			return 0;
		}

		unsigned int			nRecordOffset = m_oTableMeta.m_nRecordWidth * pRecordPostion->nDataPos;
		unsigned int			nNextRecordPos = nRecordOffset + m_oTableMeta.m_nRecordWidth;
		if( m_oTableMeta.m_nRecordWidth > (m_nMaxBufferSize-nRecordOffset) )	///< 是否有足够的后续长度( 需要大于等于DataBlockSize)
		{
			::printf( "DynamicTable::DeleteRecord() : subscript out of range of memo-tables list\n" );
			return -2;
		}

		if( 0 != ::memcmp( pKeyStr, m_pRecordsBuffer+nRecordOffset, ::strlen( pKeyStr ) ) )
		{
			::printf( "DynamicTable::DeleteRecord() : failed 2 locate record key, %s != %s\n", pKeyStr, m_pRecordsBuffer+nRecordOffset );
			return -3;
		}

		if( NULL != ::memmove( m_pRecordsBuffer+nRecordOffset, m_pRecordsBuffer+nNextRecordPos, m_nCurrentDataSize - nNextRecordPos ) )
		{
			m_nCurrentDataSize -= m_oTableMeta.m_nRecordWidth;
			if( 0 >= m_oHashTableOfIndex.DeleteKey( nDataSeqKey ) )
			{
				::printf( "DynamicTable::DeleteRecord() : failed 2 delete hash key\n" );
				return -4;
			}

			return 1;
		}

		return -5;
	}
	catch( std::exception& err )
	{
		::printf( "DynamicTable::DeleteRecord() : exception : %s\n", err.what() );
	}
	catch( ... )
	{
		::printf( "DynamicTable::DeleteRecord() : unknow exception\n" );
	}

	return 0;
}

int DynamicTable::InsertRecord( char* pRecord, unsigned int nRecordLen, unsigned __int64& nDbSerialNo )
{
	try
	{
		struct T_ListNode*		pRecordPostion = NULL;
		CriticalLock			lock( m_oCSLock );
		DyncRecord				objRecord( pRecord, nRecordLen );
		__int64					nDataSeqKey = objRecord.GetMainKey();

		nDbSerialNo = 0;
		if( NULL == m_pRecordsBuffer || m_nMaxBufferSize <= m_nCurrentDataSize )
		{	///< 内存未分配的情况 或 内存已经用完的情况
			if( false == EnlargeBuffer( (0==m_nMaxBufferSize)?1:1024 ) )
			{
				return -1;
			}
		}

		int		nInsertAffect = m_oHashTableOfIndex.NewKey( nDataSeqKey
				, m_nCurrentDataSize/m_oTableMeta.m_nRecordWidth, (nDbSerialNo=GlobalSequenceNo::GetObj().GenerateSeq()) );
		if( nInsertAffect < 0 )	///< 新增记录:	位置为, 第 0 个索引位置
		{	///< 记录不存在，新增成功的情况
			::printf( "DynamicTable::InsertRecord() : failed 2 insert data 2 hash table.\n" );
			return -2;
		}
		else if( 0 == nInsertAffect )
		{
			return 0;
		}

		if( NULL == (pRecordPostion = m_oHashTableOfIndex[nDataSeqKey]) )	{
			::printf( "DynamicTable::InsertRecord() : invalid Sequence\n" );
			return -3;
		}

		unsigned int			nDataOffsetIndex = m_oTableMeta.m_nRecordWidth * pRecordPostion->nDataPos;
		if( m_oTableMeta.m_nRecordWidth > (m_nMaxBufferSize-nDataOffsetIndex) )	///< 是否有足够的后续长度( 需要大于等于DataBlockSize)
		{
			::printf( "DynamicTable::InsertRecord() : subscript out of range of memo-tables list\n" );
			return -4;
		}

		DyncRecord				oCurRecord( m_pRecordsBuffer + nDataOffsetIndex, m_oTableMeta.m_nRecordWidth );

		if( true == pRecordPostion->IsNull() )
		{
			::printf( "DynamicTable::InsertRecord() : invalid MainKey, cannot locate record in table\n" );
			return -5;
		}

		if( nDataOffsetIndex > (m_nMaxBufferSize-nRecordLen) )
		{
			::printf( "DynamicTable::InsertRecord() : subscript out of range of memo-tables list\n" );
			return -6;
		}

		if( oCurRecord.CloneFrom( objRecord ) >= 0 )
		{
			m_nCurrentDataSize += m_oTableMeta.m_nRecordWidth;
		}

		return 1;
	}
	catch( std::exception& err )
	{
		::printf( "DynamicTable::InsertRecord() : exception : %s\n", err.what() );
	}
	catch( ... )
	{
		::printf( "DynamicTable::InsertRecord() : unknow exception\n" );
	}

	return -5;
}

int DynamicTable::UpdateRecord( char* pRecord, unsigned int nRecordLen, unsigned __int64& nDbSerialNo )
{
	try
	{
		int						nAffectNum = 0;
		struct T_ListNode*		pRecordPostion = NULL;
		CriticalLock			lock( m_oCSLock );
		DyncRecord				objRecord( pRecord, nRecordLen );
		__int64					nDataSeqKey = objRecord.GetMainKey();

		nDbSerialNo = 0;
		if( NULL == m_pRecordsBuffer || m_nMaxBufferSize <= m_nCurrentDataSize )
		{	///< 内存未分配的情况 或 内存已经用完的情况
			if( false == EnlargeBuffer( (0==m_nMaxBufferSize)?1:1024 ) )
			{
				return -1;
			}
		}

		if( NULL == (pRecordPostion = m_oHashTableOfIndex[nDataSeqKey]) )	{
			return 0;
		}

		unsigned int			nDataOffsetIndex = m_oTableMeta.m_nRecordWidth * pRecordPostion->nDataPos;
		DyncRecord				oCurRecord( m_pRecordsBuffer + nDataOffsetIndex, m_oTableMeta.m_nRecordWidth );

		if( true == pRecordPostion->IsNull() )	{
			return 0;
		}

		if( m_oTableMeta.m_nRecordWidth > (m_nMaxBufferSize-nRecordLen) )
		{
			::printf( "DynamicTable::UpdateRecord() : subscript out of range of memo-tables list\n" );
			return -3;
		}

		nAffectNum = oCurRecord.CloneFrom( objRecord );
		if( nAffectNum > 0 )
		{
			nDbSerialNo = GlobalSequenceNo::GetObj().GenerateSeq();
			pRecordPostion->nUpdateSequence = nDbSerialNo;
		}

		return nAffectNum;
	}
	catch( std::exception& err )
	{
		::printf( "DynamicTable::UpdateRecord() : exception : %s\n", err.what() );
	}
	catch( ... )
	{
		::printf( "DynamicTable::UpdateRecord() : unknow exception\n" );
	}

	return -5;
}

int DynamicTable::CopyToBuffer( char* pBuffer, unsigned int nBufferSize, unsigned __int64& nDbSerialNo )
{
	try
	{
		CriticalLock				lock( m_oCSLock );

		if( m_nCurrentDataSize >= nBufferSize )
		{
			::printf( "DynamicTable::CopyToBuffer() : data buffer isn\'t less than data size.\n" );
			return -1;
		}

		if( m_nCurrentDataSize > 0 && 0 == nDbSerialNo )
		{
			::memcpy( pBuffer, m_pRecordsBuffer, m_nCurrentDataSize );
			nDbSerialNo = GlobalSequenceNo::GetObj().GetSeqNo();
			return m_nCurrentDataSize;
		}
		else
		{
			unsigned int			nRecordsSize = 0;
			unsigned int			nCount = m_nCurrentDataSize / m_oTableMeta.m_nRecordWidth;

			for( unsigned int n = 0; n < nCount; n++ )
			{
				int					nOffset = nCount * m_oTableMeta.m_nRecordWidth;
				char*				pszMainKey = m_pRecordsBuffer + nOffset;
				DyncRecord			objRecord( pszMainKey, 32 );
				__int64				nDataSeqKey = objRecord.GetMainKey();
				struct T_ListNode*	pRecordPostion = m_oHashTableOfIndex[nDataSeqKey];

				if( NULL != pRecordPostion )
				{
					if( nDbSerialNo < pRecordPostion->nUpdateSequence )
					{
						::memcpy( pBuffer+nRecordsSize, m_pRecordsBuffer+nOffset, m_oTableMeta.m_nRecordWidth );
						nRecordsSize += m_oTableMeta.m_nRecordWidth;
					}
				}
			}

			nDbSerialNo = GlobalSequenceNo::GetObj().GetSeqNo();

			return nRecordsSize;
		}
	}
	catch( std::exception& err )
	{
		::printf( "DynamicTable::UpdateRecord() : exception : %s\n", err.what() );
	}
	catch( ... )
	{
		::printf( "DynamicTable::UpdateRecord() : unknow exception\n" );
	}

	return -10;
}



}







