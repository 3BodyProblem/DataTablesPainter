#ifndef __DATA_TABLE_PAINTER_H__
#define	__DATA_TABLE_PAINTER_H__


#include "Interface.h"


/**
 * @brief				DLL导出接口
 * @author				barry
 * @date				2017/4/1
 */
extern "C"
{
	__declspec(dllexport) DBFactory&		GetFactoryObject();

	__declspec(dllexport) void				ExecuteUnitTest();
}




#endif





