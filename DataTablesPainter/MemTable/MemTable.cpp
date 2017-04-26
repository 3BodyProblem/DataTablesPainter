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

	m_oTableMeta.Clear();
}

bool DynamicTable::EnlargeBuffer( unsigned long nAllocItemNum )
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

DyncRecord DynamicTable::SelectRecord( int nRecordIndex )
{
	if( nRecordIndex < 0 )
	{
		return DyncRecord( NULL, 0 );
	}

	CriticalLock		lock( m_oCSLock );
	unsigned int		nRecordOffset = m_oTableMeta.m_nRecordWidth * nRecordIndex;
	if( nRecordOffset >= (m_nMaxBufferSize-m_oTableMeta.m_nRecordWidth) )
	{
		return DyncRecord( NULL, 0 );
	}

	return DyncRecord( m_pRecordsBuffer+nRecordOffset, m_oTableMeta.m_nRecordWidth );
}

int DynamicTable::InsertRecord( const DyncRecord& refRecord )
{
	CriticalLock			lock( m_oCSLock );

	if( NULL == m_pRecordsBuffer || m_nMaxBufferSize == m_nCurrentDataSize )
	{	///< 内存未分配的情况 或 内存已经用完的情况
		if( false == EnlargeBuffer() )
		{
			return -1;
		}
	}

	unsigned int		nDataSeqKey = refRecord.GetSerial();
	unsigned int		nDataOffsetIndex = m_oTableMeta.m_nRecordWidth * nDataSeqKey;
	DyncRecord			oCurRecord( m_pRecordsBuffer + nDataOffsetIndex, m_oTableMeta.m_nRecordWidth );

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







