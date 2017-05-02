#ifndef __QLX_UNIT_TEST_H__
#define __QLX_UNIT_TEST_H__
#pragma warning(disable : 4996)
#pragma warning(disable : 4204)


#include <vector>
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


/**
 * @class							I_Message
 * @brief							消息获取接口
 * @author							barry
 */
class I_Message
{
public:
	I_Message( unsigned int nMessageID, unsigned int nMessageLength );
public:
	virtual char*										MessagePointer() = 0;
	virtual bool										MoveNext() = 0;
	virtual unsigned int								MessageID();
	virtual unsigned int								MessageLength();
	virtual void										Reset();
protected:
	unsigned int										m_nCurDataPos;
	unsigned int										m_nMessageID;
	unsigned int										m_nMessageLength;
};

/**
 * @class							NameTable4CodeTest
 * @brief							码表测试数据接口,测试各种变化的代码值
 * @author							barry
 */
class NameTable4CodeTest : public I_Message
{
public:
	struct T_Message1234_NameTable
	{
		T_Message1234_NameTable( char* pszCode, char* pszName, unsigned int nType )
		{
			::memset( SecurityID, 0, sizeof(SecurityID) );
			::memset( SecurityName, 0, sizeof(SecurityName) );
			SecurityType = nType;
			::strncpy( SecurityID, pszCode, ::strlen( pszCode ) );
			::strncpy( SecurityName, pszName, ::strlen( pszName ) );
		}

		char				SecurityID[64];				///< 商品代码
		char				SecurityName[128];			///< 商品名称
		unsigned int		SecurityType;				///< 商品类型
	};

	NameTable4CodeTest();

public:
	bool												MoveNext();
	char*												MessagePointer();

protected:
	std::vector<T_Message1234_NameTable>				m_vctMessage;
};


/**
 * @class							SnapTable4ValueTest
 * @brief							快照表测试数据接口,测试各种变化的数值
 * @author							barry
 */
class SnapTable4ValueTest : public I_Message
{
public:
	struct T_Message2003_SnapTable
	{
		T_Message2003_SnapTable( char* pszCode, unsigned int nNow, unsigned int nHigh, unsigned int nLow, double dAmount, unsigned __int64 nVolume )
		{
			::memset( SecurityID, 0, sizeof(SecurityID) );
			::strncpy( SecurityID, pszCode, ::strlen( pszCode ) );
			High = nHigh;
			Low = nLow;
			Now = nNow;
			Volume = nVolume;
			Amount = dAmount;
		}

		char				SecurityID[64];				///< 商品代码
		unsigned int		High;
		unsigned int		Low;
		unsigned int		Now;
		double				Amount;						///< 商品成交金额
		unsigned __int64	Volume;						///< 商品总成交量
	};

	SnapTable4ValueTest();

public:
	bool												MoveNext();
	char*												MessagePointer();

protected:
	std::vector<T_Message2003_SnapTable>				m_vctMessage;
};


/**
 * @class							VariableWidthTable4MsgIDTest
 * @brief							不同数据表MessageID测试数据接口,测试各种变化的MessageID数值
 * @author							barry
 */
class VariableWidthTable4MsgIDTest : public I_Message
{
public:
	struct T_MessageX_Table
	{
		T_MessageX_Table( char* pszCode, unsigned int nMsgID )
		{
			::memset( SecurityID, 0, sizeof(SecurityID) );
			::strncpy( SecurityID, pszCode, ::strlen( pszCode ) );
			MsgID = nMsgID;
			Value = 0;
		}

		char				SecurityID[64];				///< 商品代码
		unsigned int		MsgID;
		unsigned int		Value;
	};

	VariableWidthTable4MsgIDTest();

public:
	unsigned int										MessageID();
	bool												MoveNext();
	char*												MessagePointer();

protected:
	std::vector<T_MessageX_Table>						m_vctMessage;
};


/**
 * @class							TestAnyMessage_ID_X
 * @brief							测试任意类型的消息结构
 * @author							barry
 */
class TestAnyMessage_ID_X : public testing::Test
{
protected:
	void												TestLocateTable( unsigned int nBindID, I_Table** pTable, bool bIsExist );
	void												TestDeleteAllTables();
	void												TestCreateAllTable();
	void												TestInsert1Table( I_Message* pMessage, bool IsExist, int nAffectNum );
	void												TestUpdate1Table( I_Message* pMessage, bool IsExist, int nAffectNum );
	void												TestQuery1Table( I_Message* pMessage, bool IsExist );

protected:
	static	void										SetUpTestCase();
	static	void										TearDownTestCase();
	void												SetUp();
	void												TearDown();

protected:
	std::vector<I_Message*>								m_vctIMessage;
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





