#include "UnitTest.h"
#include <string>
#include <iostream>
#include <algorithm>


///< --------------------- 单元测试类定义 --------------------------------


I_Table* TestCreateNameTable1000_ZeroMemo::s_pTablePtr = NULL;
MemoryCollection::DynamicTable::TableMeta TestCreateNameTable1000_ZeroMemo::s_oTableMeta( 0, 0, 0 );
TestCreateNameTable1000_ZeroMemo::T_Message1000_NameTable TestCreateNameTable1000_ZeroMemo::s_sMsg1000NameTable = { 0 };

void TestCreateNameTable1000_ZeroMemo::SetUpTestCase()
{
	///< 静态域：初始化数据表指针
	s_pTablePtr = NULL;
	///< 静态域：初始化数据表元信息
	s_oTableMeta = MemoryCollection::DynamicTable::TableMeta( 1000, sizeof(T_Message1000_NameTable), sizeof(s_sMsg1000NameTable.SecurityID) );
	///< 静态域：码表清零
	::memset( &s_sMsg1000NameTable, 0, sizeof(s_sMsg1000NameTable) );
}

void TestCreateNameTable1000_ZeroMemo::TearDownTestCase()
{
}

void TestCreateNameTable1000_ZeroMemo::SetUp()
{
	for( int n = 0; n < 10; n++ ) {
		ASSERT_EQ( true, UnitTestEnv::GetDatabasePtr()->CreateTable( s_oTableMeta.m_nBindID, s_oTableMeta.m_nRecordWidth, s_oTableMeta.m_nKeyStrLen ) );
	}
}

void TestCreateNameTable1000_ZeroMemo::TearDown()
{
	TestLocateTable();
}

void TestCreateNameTable1000_ZeroMemo::TestLocateTable()
{
	I_Table*	pTable = UnitTestEnv::GetDatabasePtr()->QueryTable( s_oTableMeta.m_nBindID );

	ASSERT_NE( pTable, (I_Table*)NULL );			///< 返回数据表指针不能为空

	if( NULL == s_pTablePtr )
	{
		s_pTablePtr = pTable;						///< 第一次查询到数据表指针
	}
	else
	{
		ASSERT_EQ( s_pTablePtr, pTable );			///< 之后每次查询得到的指针都应该等价于第一次的查询指针
	}
}

void TestCreateNameTable1000_ZeroMemo::TestInsertRecord()
{
	for( int n = 0; n < 10; n++ ) {
		ASSERT_EQ( 0, s_pTablePtr->InsertRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable) ) );
	}
}

void TestCreateNameTable1000_ZeroMemo::TestSelectRecord()
{
	for( int n = 0; n < 100; n++ ) {
		RecordBlock	record = s_pTablePtr->SelectRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable) );
		ASSERT_NE( true, record.IsNone() );				///< 返回数据表指针不能为空
		ASSERT_EQ( true, record.Compare( RecordBlock((char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable)) ) );
	}
}

void TestCreateNameTable1000_Normal::SetUpTestCase()
{
	TestCreateNameTable1000_ZeroMemo::SetUpTestCase();

	///< 静态域：码表清零
	::sprintf( s_sMsg1000NameTable.SecurityID, "SR123456-C-%u", 2800 );
	::sprintf( s_sMsg1000NameTable.SecurityName, "测试名称 %d 号单元", 3 );
	s_sMsg1000NameTable.SecurityType = 1024;
	s_sMsg1000NameTable.Number1 = 1;
	s_sMsg1000NameTable.Number2 = 2;
	s_sMsg1000NameTable.Number3 = 3;
}

void TestCreateNameTable1000_Normal::TestInsertRecord()
{
	for( int n = 0; n < 10; n++ ) {
		ASSERT_EQ( 0, s_pTablePtr->InsertRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable) ) );
	}
}

void TestCreateNameTable1000_Normal::TestSelectRecord()
{
	for( int n = 0; n < 100; n++ ) {
		RecordBlock	record = s_pTablePtr->SelectRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable) );
		ASSERT_NE( true, record.IsNone() );				///< 返回数据表指针不能为空
		ASSERT_EQ( true, record.Compare( RecordBlock((char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable)) ) );

		if( (n+1) % 20 == 0 )	{
			T_Message1000_NameTable*	pNameTable = (T_Message1000_NameTable*)record.GetPtr();
			::printf( "[%u] Code=%s,Name=%s,Type=%u,Num1=%u,Num2=%u,Num3=%u\n"
					, (n+1), pNameTable->SecurityID, pNameTable->SecurityName, pNameTable->SecurityType
					, pNameTable->Number1, pNameTable->Number2, pNameTable->Number3 );
		}
	}
}


///< ------------------------ 测试用例定义 ----------------------------------------------------


TEST_F( TestCreateNameTable1000_ZeroMemo, InsertOption_ZeroMemoryOfNameTable )
{
	TestLocateTable();
	TestInsertRecord();
	TestLocateTable();
	TestSelectRecord();
}


TEST_F( TestCreateNameTable1000_Normal, InsertOptionOfNameTable )
{
	TestLocateTable();
	TestInsertRecord();
	TestLocateTable();
	TestSelectRecord();
}


///< ---------------------- 单元测试初始化类定义 -------------------------


I_Database* UnitTestEnv::m_pIDatabase = NULL;


UnitTestEnv::UnitTestEnv()
{
}

I_Database* UnitTestEnv::GetDatabasePtr()
{
	return m_pIDatabase;
}

void UnitTestEnv::SetUp()
{
	m_pIDatabase = DBFactory::GetFactory().GrapDatabaseInterface();
	ASSERT_NE( m_pIDatabase, (I_Database*)NULL );
}

void UnitTestEnv::TearDown()
{
	ASSERT_EQ( DBFactory::GetFactory().ReleaseAllDatabase(), true );
}


///< ---------------- 单元测试导出函数定义 -------------------------------


void ExecuteTestCase()
{
	static	bool	s_bInit = false;

	if( false == s_bInit )	{
		int			nArgc = 1;
		char*		pszArgv[32] = { "MemDatabase.dll", };

		s_bInit = true;
		testing::AddGlobalTestEnvironment( new UnitTestEnv() );
		testing::InitGoogleTest( &nArgc, pszArgv );
		RUN_ALL_TESTS();
	}
}











