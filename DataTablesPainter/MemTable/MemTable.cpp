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

	if( NULL == m_pRecordsBuffer )	///< ��һ�η��䣬ֻ����һ����¼�Ŀռ�
	{
		if( NULL == (m_pRecordsBuffer = (char*)::malloc( nNewBufferSize )) )
		{
			return false;
		}

		m_nMaxBufferSize = nNewBufferSize;
		::memset( m_pRecordsBuffer, 0, m_nMaxBufferSize );
	}
	else							///< �ٴη��䣬ÿ�ζ�����1024����¼�Ŀռ�
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
	{	///< �ڴ�δ�������� �� �ڴ��Ѿ���������
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







