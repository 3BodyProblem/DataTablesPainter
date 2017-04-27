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
	/**
	 * @brief								取得工厂对象引用,用于创建/管理/收回内存数据库对象
	 * @return								返回管理对象的引用
	 */
	__declspec(dllexport) DBFactory&		GetFactoryObject();

	/**
	 * @brief								单元测试导出函数
	 */
	__declspec(dllexport) void				ExecuteUnitTest();
}




#endif





