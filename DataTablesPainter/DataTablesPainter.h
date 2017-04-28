#ifndef __DATA_TABLE_PAINTER_H__
#define	__DATA_TABLE_PAINTER_H__


#include "Interface.h"
#include "Infrastructure/Lock.h"


/**
 * @class							DBFactory
 * @brief							内存数据分配管理工厂类
 */
class DBFactory : public IDBFactory
{
private:
	DBFactory();

public:
	/**
	 * @brief						取得单键
	 */
	static	IDBFactory&				GetFactory();

public:
	/**
	 * @brief						创建并返回数据库对象指针
	 * @return						返回数据库指针的地址
	 */
	I_Database*						GrapDatabaseInterface();

	/**
	 * @brief						释放分配的所有数据库对象
	 */
	bool							ReleaseAllDatabase();

};



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
	__declspec(dllexport) IDBFactory&		GetFactoryObject();

	/**
	 * @brief								单元测试导出函数
	 */
	__declspec(dllexport) void				ExecuteUnitTest();
}




#endif





