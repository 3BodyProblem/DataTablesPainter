#include "MemTable.h"


namespace MemoryCollection
{


VariableRecordTable::VariableRecordTable( unsigned int nRecordWidth, unsigned int nKeyStrLen )
	: m_nRecordWidth( nRecordWidth ), m_pRecordsBuffer( NULL ), m_nMainStrKeyLen( nKeyStrLen )
	, m_nMaxBufferSize( 0 ), m_nCurrentDataSize( 0 )
{
}

VariableRecordTable::~VariableRecordTable()
{
	if( NULL != m_pRecordsBuffer )
	{
		m_nRecordWidth = 0;
		m_nMainStrKeyLen = 0;
		m_nMaxBufferSize = 0;
		m_nCurrentDataSize = 0;
		::free( m_pRecordsBuffer );
		m_pRecordsBuffer = NULL;
	}
}

bool VariableRecordTable::EnlargeBuffer( unsigned long nAllocItemNum )
{
	unsigned int	nNewBufferSize = m_nRecordWidth * m_nRecordWidth;

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

	unsigned int		nRecordOffset = m_nRecordWidth * nIndex;
	if( nRecordOffset >= (m_nMaxBufferSize-m_nRecordWidth) )
	{
		return RecordWithKey( NULL, 0 );
	}

	return RecordWithKey( m_pRecordsBuffer+nRecordOffset, m_nRecordWidth );
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
	unsigned int		nDataOffsetIndex = m_nRecordWidth * nDataSeqKey;
	RecordWithKey		oCurRecord( m_pRecordsBuffer + nDataOffsetIndex, m_nRecordWidth );

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







