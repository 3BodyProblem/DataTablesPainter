#ifndef __QLX_UNIT_TEST_H__
#define __QLX_UNIT_TEST_H__
#pragma warning(disable : 4996)
#pragma warning(disable : 4204)


#include "gtest/gtest.h"
#include "../Interface.h"
#include "../MemTable/MemTable.h"


///< --------------------- 单元测试类定义 --------------------------------


/**
 * @class							TestCreateNameTable1000_ZeroMemo
 * @brief							测试创建数据表和空记录(Code为空，所有属性都为空)	(ID=1000)
 * @author							barry
 */
class TestCreateNameTable1000_ZeroMemo : public testing::Test
{
public:
	typedef struct
	{
		char				SecurityID[32];				///< 商品代码
		char				SecurityName[64];			///< 商品名称
		unsigned int		SecurityType;				///< 商品类型
		unsigned int		Number1;
		unsigned int		Number2;
		unsigned int		Number3;
	} T_Message1000_NameTable;

protected:
	virtual void										TestLocateTable();
	virtual void										TestInsertRecord();
	virtual void										TestSelectRecord();

protected:
	static	void										SetUpTestCase();
	static	void										TearDownTestCase();
	void												SetUp();
	void												TearDown();

protected:
	static I_Table*										s_pTablePtr;			///< 数据表指针
	static MemoryCollection::DynamicTable::TableMeta	s_oTableMeta;			///< 数据表元信息
	static T_Message1000_NameTable						s_sMsg1000NameTable;	///< 码表结构
};


/**
 * @class							TestCreateNameTable1000_Normal
 * @brief							测试创建数据表和一条有内容的记录	(ID=1000)
 * @author							barry
 */
class TestCreateNameTable1000_Normal : public TestCreateNameTable1000_ZeroMemo
{
protected:
	static	void										SetUpTestCase();
	virtual void										TestInsertRecord();
	virtual void										TestUpdateRecord();
	virtual void										TestSelectRecord();
};




///< ------------ 单元测试初始化类定义 ------------------------------------


/**
 * @class					QLXEnDeCodeTestEnv
 * @brief					全局事件(初始化引擎)
 * @author					barry
 */
class UnitTestEnv : public testing::Environment
{
public:
	/**
	 * @brief				取单数据库对象指针,从而进行指定测试
	 */
	static	I_Database*		GetDatabasePtr();
	void					SetUp();
	void					TearDown();

private:
	static I_Database*		m_pIDatabase;		///< 进行压力操作的数据对象地址
};


/**
 * @brief					运行所有的测试例程
 * @author					barry
 */
void ExecuteTestCase();



#endif





