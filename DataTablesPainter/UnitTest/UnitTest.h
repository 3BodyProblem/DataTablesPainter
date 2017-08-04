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
 * @class							TestTableOperation
 * @brief							测试数据表操作业务
 * @author							barry
 */
class TestTableOperation : public testing::Test
{
public:
	TestTableOperation();

public:
	///< -------------- message definition -------------------------
	struct T_Message_MarketInfo
	{
		T_Message_MarketInfo( char* pszKey, unsigned int nMarketID, unsigned int nDate, unsigned int nTime )
		{
			::strcpy( MainKey, pszKey );
			MarketID = nMarketID;
			MarketDate = nDate;
			MarketTime = nTime;
		}

		static unsigned int GetID() {	return 128;	}

		char				MainKey[32];
		unsigned int		MarketID;
		unsigned int		MarketDate;
		unsigned int		MarketTime;
	};

	struct T_Message_NameTable
	{
		T_Message_NameTable( char* pszCode, char* pszName, unsigned int nType, unsigned int nNum1 = 0, unsigned int nNum2 = 1, unsigned int nNum3 = 2 )
		{
			::strcpy( SecurityID, pszCode );
			::strcpy( SecurityName, pszName );
			SecurityType = nType;
			Number1 = nNum1;
			Number2 = nNum2;
			Number3 = nNum3;
		}

		static unsigned int GetID() {	return 1024+128;	}

		char				SecurityID[32];				///< 商品代码
		char				SecurityName[64];			///< 商品名称
		unsigned int		SecurityType;				///< 商品类型
		unsigned int		Number1;
		unsigned int		Number2;
		unsigned int		Number3;
	};

	struct T_Message_SnapTable
	{
		T_Message_SnapTable( char* pszCode, unsigned int nNow, unsigned int nHigh, unsigned int nLow, double dAmount, unsigned __int64 nVolume )
		{
			::memset( SecurityID, 0, sizeof(SecurityID) );
			::strncpy( SecurityID, pszCode, ::strlen( pszCode ) );
			High = nHigh;
			Low = nLow;
			Now = nNow;
			Volume = nVolume;
			Amount = dAmount;
		}

		static unsigned int GetID() {	return 2*1024+128;	}

		char				SecurityID[32];				///< 商品代码
		unsigned int		High;
		unsigned int		Low;
		unsigned int		Now;
		double				Amount;						///< 商品成交金额
		unsigned __int64	Volume;						///< 商品总成交量
	};

protected:
	virtual void										TestLocateMarketInfo( bool bIsExist = true );
	virtual void										TestInsertMarketInfo( unsigned int nSeed, bool bIsExist = false );
	virtual void										TestDeleteMarketInfo( unsigned int nSeed, bool bIsExist );
	virtual void										TestSelectMarketInfo( unsigned int nSeed, bool bIsExist );
	virtual void										TestUpdateMarketInfo( unsigned int nSeed, bool bIsExist );

	virtual void										TestLocateNameTable( bool bIsExist = true );
	virtual void										TestInsertNameTable( unsigned int nSeed, bool bIsExist = false );
	virtual void										TestDeleteNameTable( unsigned int nSeed, bool bIsExist );
	virtual void										TestSelectNameTable( unsigned int nSeed, bool bIsExist );
	virtual void										TestUpdateNameTable( unsigned int nSeed, bool bIsExist );

	virtual void										TestLocateSnapTable( bool bIsExist = true );
	virtual void										TestInsertSnapTable( unsigned int nSeed, bool bIsExist = false );
	virtual void										TestDeleteSnapTable( unsigned int nSeed, bool bIsExist );
	virtual void										TestSelectSnapTable( unsigned int nSeed, bool bIsExist );
	virtual void										TestUpdateSnapTable( unsigned int nSeed, bool bIsExist );

protected:
	static	void										SetUpTestCase();
	static	void										TearDownTestCase();
	void												SetUp();				///< 预告建立所有需要的数据表
	void												TearDown();
	void												TestCreateAllTable();
	void												TestDeleteAllTables();
	void												TestDeleteOneTable( unsigned int nTableID );

protected:
	I_Table*											m_pCurTablePtr;			///< 市场信息列表
	int													m_nMaxLoopNum;			///< 最大数据队列长度
	static std::vector<T_Message_MarketInfo>			m_vctMarketInfo;		///< 市场信息列表
	static std::vector<T_Message_NameTable>				m_vctNameTable;			///< 市场码表列表
	static std::vector<T_Message_SnapTable>				m_vctSnapTable;			///< 市场快照列表
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





