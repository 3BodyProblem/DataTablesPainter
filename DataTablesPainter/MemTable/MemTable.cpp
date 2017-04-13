#include "MemTable.h"


namespace MemoryCollection
{


VariableRecordTable::VariableRecordTable( unsigned int nRecordWidth, unsigned int nKeyStrLen )
	: m_nRecordWidth( nRecordWidth ), m_pRecordsBuffer( NULL ), m_nMainStrKeyLen( nKeyStrLen )
	, m_nMaxBufferSize( 0 ), m_nCurrentDataSize( 0 )
{
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

int VariableRecordTable::PushBack( const char* pszData, unsigned int nDataLen )
{
	if( NULL == m_pRecordsBuffer || m_nMaxBufferSize == m_nCurrentDataSize )
	{	///< 内存未分配的情况 或 内存已经用完的情况
		if( false == EnlargeBuffer() )
		{
			return -1;
		}
	}

	unsigned __int64	nDataSeqKey = GenerateHashKey( pszData, m_nMainStrKeyLen );
	unsigned int		nDataOffsetIndex = m_nRecordWidth * nDataSeqKey;
	char*				pRecord = m_pRecordsBuffer + m_nCurrentDataSize;

	if( nDataOffsetIndex >= (m_nMaxBufferSize-nDataLen) )
	{
		return -2;
	}

	if( 0 != ::memcmp( pRecord, pszData, nDataLen ) )
	{
		memcpy( m_pRecordsBuffer+m_nCurrentDataSize, pszData, nDataLen );
		++m_nCurrentDataSize;
	}

	return 0;
}



}







