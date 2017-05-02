#include "MemTable.h"


namespace MemoryCollection
{

DynamicTable::TableMeta::TableMeta( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen )
	: m_nBindID( nBindID ), m_nRecordWidth( nRecordWidth ), m_nKeyStrLen( nKeyStrLen )
{
}

void DynamicTable::TableMeta::Clear()
{
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
	::memset( m_pRecordsBuffer, 0, m_nMaxBufferSize );
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
		T_RECORD_POS			recordPostion = m_oHashTableOfIndex[nDataSeqKey];
		unsigned int			nRecordOffset = m_oTableMeta.m_nRecordWidth * recordPostion.nRecordPos;

		if( nRecordOffset >= (m_nMaxBufferSize-nRecordOffset) )
		{
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

int DynamicTable::InsertRecord( char* pRecord, unsigned int nRecordLen )
{
	try
	{
		CriticalLock			lock( m_oCSLock );
		DyncRecord				objRecord( pRecord, nRecordLen );
		__int64					nDataSeqKey = objRecord.GetMainKey();

		if( NULL == m_pRecordsBuffer || m_nMaxBufferSize <= m_nCurrentDataSize )
		{	///< 内存未分配的情况 或 内存已经用完的情况
			if( false == EnlargeBuffer( (0==m_nMaxBufferSize)?1:1024 ) )
			{
				return -1;
			}
		}

		int		nInsertAffect = m_oHashTableOfIndex.NewKey( nDataSeqKey, T_RECORD_POS( m_nCurrentDataSize/m_oTableMeta.m_nRecordWidth ) );
		if( nInsertAffect < 0 )	///< 新增记录:	位置为, 第 0 个索引位置
		{	///< 记录不存在，新增成功的情况
			::printf( "DynamicTable::InsertRecord() : failed 2 insert data 2 hash table.\n" );
			return -2;
		}
		else if( 0 == nInsertAffect )
		{
			return 0;
		}

		T_RECORD_POS			recordPostion = m_oHashTableOfIndex[nDataSeqKey];
		unsigned int			nDataOffsetIndex = m_oTableMeta.m_nRecordWidth * recordPostion.nRecordPos;
		DyncRecord				oCurRecord( m_pRecordsBuffer + nDataOffsetIndex, m_oTableMeta.m_nRecordWidth );

		if( true == recordPostion.Empty() )
		{
			::printf( "DynamicTable::InsertRecord() : invalid MainKey, cannot locate record in table\n" );
			return -3;
		}

		if( nDataOffsetIndex >= (m_nMaxBufferSize-nDataOffsetIndex) )
		{
			::printf( "MemDatabase::InsertRecord() : subscript out of range of memo-tables list\n" );
			return -4;
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

int DynamicTable::UpdateRecord( char* pRecord, unsigned int nRecordLen )
{
	try
	{
		CriticalLock			lock( m_oCSLock );
		DyncRecord				objRecord( pRecord, nRecordLen );
		__int64					nDataSeqKey = objRecord.GetMainKey();

		if( NULL == m_pRecordsBuffer || m_nMaxBufferSize <= m_nCurrentDataSize )
		{	///< 内存未分配的情况 或 内存已经用完的情况
			if( false == EnlargeBuffer( (0==m_nMaxBufferSize)?1:1024 ) )
			{
				return -1;
			}
		}

		T_RECORD_POS			recordPostion = m_oHashTableOfIndex[nDataSeqKey];
		unsigned int			nDataOffsetIndex = m_oTableMeta.m_nRecordWidth * recordPostion.nRecordPos;
		DyncRecord				oCurRecord( m_pRecordsBuffer + nDataOffsetIndex, m_oTableMeta.m_nRecordWidth );

		if( true == recordPostion.Empty() )
		{
			return 0;
		}

		if( nDataOffsetIndex >= (m_nMaxBufferSize-nDataOffsetIndex) )
		{
			::printf( "MemDatabase::UpdateRecord() : subscript out of range of memo-tables list\n" );
			return -3;
		}

		return oCurRecord.CloneFrom( objRecord );
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



}







