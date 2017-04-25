#include "MemTable.h"


namespace MemoryCollection
{

VariableRecordTable::TableMeta::TableMeta( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen )
	: m_nBindID( nBindID ), m_nRecordWidth( nRecordWidth ), m_nKeyStrLen( nKeyStrLen )
{
}

void VariableRecordTable::TableMeta::Clear()
{
	m_nKeyStrLen = 0;
	m_nRecordWidth = 0;
}

bool VariableRecordTable::TableMeta::IsEngaged()
{
	return 0 != m_nRecordWidth;
}

VariableRecordTable::VariableRecordTable()
	: m_oTableMeta( 0, 0 ), m_pRecordsBuffer( NULL )
	, m_nMaxBufferSize( 0 ), m_nCurrentDataSize( 0 )
{
}

VariableRecordTable::~VariableRecordTable()
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

void VariableRecordTable::Initialize( const VariableRecordTable::TableMeta& refMeta )
{
	Release();

	m_oTableMeta = refMeta;
}

void VariableRecordTable::Release()
{
	m_oTableMeta.Clear();
}

bool VariableRecordTable::EnlargeBuffer( unsigned long nAllocItemNum )
{
	unsigned int	nNewBufferSize = m_oTableMeta.m_nRecordWidth * nAllocItemNum;

	if( 0 == nAllocItemNum )
	{
		return false;
	}

	if( NULL == m_pRecordsBuffer )	///< 第一次分配，只分配一条记录的空间
	{
		if( NULL == (m_pRecordsBuffer = (char*)::malloc( nNewBufferSize )) )
		{
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

RecordWithKey VariableRecordTable::operator[]( int nIndex )
{
	if( nIndex < 0 )
	{
		return RecordWithKey( NULL, 0 );
	}

	unsigned int		nRecordOffset = m_oTableMeta.m_nRecordWidth * nIndex;
	if( nRecordOffset >= (m_nMaxBufferSize-m_oTableMeta.m_nRecordWidth) )
	{
		return RecordWithKey( NULL, 0 );
	}

	return RecordWithKey( m_pRecordsBuffer+nRecordOffset, m_oTableMeta.m_nRecordWidth );
}

int VariableRecordTable::PushBack( const RecordWithKey& refRecord )
{
	if( NULL == m_pRecordsBuffer || m_nMaxBufferSize == m_nCurrentDataSize )
	{	///< 内存未分配的情况 或 内存已经用完的情况
		if( false == EnlargeBuffer() )
		{
			return -1;
		}
	}

	unsigned int		nDataSeqKey = refRecord.GetSerialInTable();
	unsigned int		nDataOffsetIndex = m_oTableMeta.m_nRecordWidth * nDataSeqKey;
	RecordWithKey		oCurRecord( m_pRecordsBuffer + nDataOffsetIndex, m_oTableMeta.m_nRecordWidth );

	if( nDataOffsetIndex >= (m_nMaxBufferSize-refRecord.Length()) )
	{
		return -2;
	}

	if( oCurRecord.CloneFrom( refRecord ) > 0 )
	{
		++m_nCurrentDataSize;
	}

	return 0;
}



}







