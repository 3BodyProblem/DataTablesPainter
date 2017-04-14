#pragma warning(disable:4244)
#include "MemRecord.h"
#include <string.h>
#include <math.h>


namespace MemoryCollection
{


RecordWithKey::RecordWithKey( char* pRecord, unsigned int nRecordLen )
	: m_pRecordData( pRecord ), m_nRecordLen( nRecordLen )
{
}

RecordWithKey::RecordWithKey( const RecordWithKey& obj )
{
	m_pRecordData = obj.m_pRecordData;
	m_nRecordLen = obj.m_nRecordLen;
}

unsigned int RecordWithKey::Length() const
{
	return m_nRecordLen;
}

bool RecordWithKey::IsNone() const
{
	if( NULL == m_pRecordData )
	{
		return true;
	}

	return false;
}

int RecordWithKey::CloneFrom( const RecordWithKey& refRecord )
{
	if( 0 != ::memcmp( m_pRecordData, refRecord.m_pRecordData, refRecord.Length() ) )
	{
		memcpy( m_pRecordData, refRecord.m_pRecordData, refRecord.Length() );

		return 1;
	}

	return 0;
}

int RecordWithKey::GetSerialInTable() const
{
	if( true == IsNone() )
	{
		return -1;
	}

	char*							pszCodeKey = m_pRecordData;
	int								nCodeLen = ::strlen( pszCodeKey );

	return GenerateHashKey( pszCodeKey, nCodeLen );
}

unsigned __int64 RecordWithKey::GenerateHashKey( const char* pszCode, unsigned int nCodeLen )
{
	char							pszMainKey[32] = { 0 };
	unsigned __int64				nTmpVal = 0;
	unsigned __int64				nCodeKey = ::pow( 10., 18 );		///< 分配一个hash的基值

	for( int i = 0; i < nCodeLen; i++ )
	{
		char	cChar = pszCode[i];

		if( (cChar>='A'&&cChar<='Z') || (cChar>='a'&&cChar<='z') )	///< 判断是否为英文字符
		{
			if( nTmpVal == 0 )
			{
				nTmpVal = ( cChar-'A' );
			}
			else
			{
				nTmpVal = nTmpVal * 100 + (cChar-'A');
			}
		}
		else if( cChar>='0'&&cChar<='9' )							///< 判断是否为数字字符
		{
			int	nVal = cChar - '0';		///< 基于'A'取差值
			if( nTmpVal == 0 )
			{
				nTmpVal = nVal;
			}
			else
			{
				nTmpVal = nTmpVal * 10 + nVal;
			}
		}
		else														///< 其他情况
		{
			if( nTmpVal == 0 )
			{
				nTmpVal = cChar;
			}
			else
			{
				nTmpVal = nTmpVal * 1000 + cChar;
			}
		}
	}

	nCodeKey += nTmpVal;

	return nCodeKey;
}



}







