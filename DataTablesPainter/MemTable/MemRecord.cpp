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

__int64 CodeKey::GetKeyID() const
{
	if( m_nKeyID >= 0 )
	{
		return m_nKeyID;
	}

	if( NULL == m_pszCode )
	{
		return -1;
	}

	m_nKeyID = GenHashKey( m_pszCode, ::strlen( m_pszCode ) );

	return m_nKeyID;
}

__int64 CodeKey::GenHashKey( const char* pszCode, unsigned int nCodeLen )
{
	char				pszMainKey[32] = { 0 };
	__int64				nTmpVal = 0;
	__int64				nCodeKey = ::pow( 10., 18 );	///< ����һ��hash�Ļ�ֵ

	for( unsigned int i = 0; i < nCodeLen; i++ )
	{
		char	cChar = pszCode[i];

		if( (cChar>='A'&&cChar<='Z') || (cChar>='a'&&cChar<='z') )	///< �ж��Ƿ�ΪӢ���ַ�
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
		else if( cChar>='0'&&cChar<='9' )							///< �ж��Ƿ�Ϊ�����ַ�
		{
			int	nVal = cChar - '0';		///< ����'A'ȡ��ֵ
			if( nTmpVal == 0 )
			{
				nTmpVal = nVal;
			}
			else
			{
				nTmpVal = nTmpVal * 10 + nVal;
			}
		}
		else														///< �������
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
	: RecordBlock( pRecord, nRecordLen ), m_oCodeKey( pRecord )
{
}

DyncRecord::DyncRecord( const DyncRecord& obj )
 : m_oCodeKey( obj.m_pRecordData )
{
	*((RecordBlock*)this) = *((RecordBlock*)&obj);
}

__int64 DyncRecord::GetMainKey() const
{
	return m_oCodeKey.GetKeyID();
}



}







