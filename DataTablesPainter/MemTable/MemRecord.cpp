#pragma warning(disable:4244)
#include "MemRecord.h"
#include <string.h>
#include <math.h>


namespace MemoryCollection
{

unsigned __int64 GenerateHashKey( char (&Code)[20] )
{
	unsigned __int64				nTmpVal = 0;
	unsigned __int64				nCodeKey = ::pow( 10., 18 );		///< ����һ��hash�Ļ�ֵ
	int								nCodeLen = ::strlen( Code );	///< �������Ȩʱ��code����, EID_Mk_OPTION

	for( int i = 0; i < nCodeLen; i++ )
	{
		char	cChar = Code[i];

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

}







