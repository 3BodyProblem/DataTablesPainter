#pragma warning(disable:4244)
#include "MemRecord.h"
#include <string.h>
#include <math.h>


namespace MemoryCollection
{


CodeKey::CodeKey( char* pszCode )
 : m_pszCode( pszCode ), m_nKeyID( -1 )
{
}

int CodeKey::GetKeyID() const
{
	if( m_nKeyID >= 0 )
	{
		return m_nKeyID;
	}

	if( NULL == m_pszCode )
	{
		return -1;
	}

	return GenHashKey( m_pszCode, ::strlen( m_pszCode ) );
}

unsigned __int64 CodeKey::GenHashKey( const char* pszCode, unsigned int nCodeLen )
{
	char							pszMainKey[32] = { 0 };
	unsigned __int64				nTmpVal = 0;
	unsigned __int64				nCodeKey = ::pow( 10., 18 );	///< 分配一个hash的基值

	for( unsigned int i = 0; i < nCodeLen; i++ )
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


DyncRecord::DyncRecord( char* pRecord, unsigned int nRecordLen )
	: m_pRecordData( pRecord ), m_nRecordLen( nRecordLen ), m_oCodeKey( pRecord )
{
}

DyncRecord::DyncRecord( const DyncRecord& obj )
 : m_oCodeKey( obj.m_pRecordData )
{
	m_pRecordData = obj.m_pRecordData;
	m_nRecordLen = obj.m_nRecordLen;
}

unsigned int DyncRecord::Length() const
{
	return m_nRecordLen;
}

bool DyncRecord::IsNone() const
{
	if( NULL == m_pRecordData )
	{
		return true;
	}

	return false;
}

int DyncRecord::CloneFrom( const DyncRecord& refRecord )
{
	if( 0 != ::memcmp( m_pRecordData, refRecord.m_pRecordData, refRecord.Length() ) )
	{
		memcpy( m_pRecordData, refRecord.m_pRecordData, refRecord.Length() );

		return 1;
	}

	return 0;
}

int DyncRecord::GetSerial() const
{
	return m_oCodeKey.GetKeyID();
}



}







