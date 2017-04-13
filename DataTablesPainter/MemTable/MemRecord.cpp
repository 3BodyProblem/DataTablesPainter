#pragma warning(disable:4244)
#include "MemRecord.h"
#include <string.h>
#include <math.h>


namespace MemoryCollection
{

unsigned __int64 GenerateHashKey( const char* pszCode, unsigned int nMaxCodeLen )
{
	unsigned __int64				nTmpVal = 0;
	unsigned __int64				nCodeKey = ::pow( 10., 18 );		///< 分配一个hash的基值
	int								nCodeLen = ::strlen( pszCode );		///< 如果是期权时的code长度, EID_Mk_OPTION

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







