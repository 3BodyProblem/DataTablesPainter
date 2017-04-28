#include "UnitTest.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "../DataTablesPainter.h"


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
		ASSERT_LE( 0, s_pTablePtr->InsertRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable) ) );
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
	::sprintf( s_sMsg1000NameTable.SecurityID, "OK123456-C-%u", 2800 );
	::sprintf( s_sMsg1000NameTable.SecurityName, "测试名称 %d 号单元", 1 );
	s_sMsg1000NameTable.SecurityType = 1024;
	s_sMsg1000NameTable.Number1 = 1;
	s_sMsg1000NameTable.Number2 = 2;
	s_sMsg1000NameTable.Number3 = 3;
}

void TestCreateNameTable1000_Normal::TestInsertRecord()
{
	for( int n = 0; n < 10; n++ ) {
		ASSERT_LE( 0, s_pTablePtr->InsertRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable) ) );
	}
}

void TestCreateNameTable1000_Normal::TestUpdateRecord()
{
	s_sMsg1000NameTable.Number1 = 1000;
	s_sMsg1000NameTable.Number2 = 2000;
	s_sMsg1000NameTable.Number3 = 3000;
	s_sMsg1000NameTable.SecurityType = 1024*3;
	::sprintf( s_sMsg1000NameTable.SecurityName, "测试名称 %d 号单元", 2 );

	for( int n = 0; n < 10; n++ ) {
		ASSERT_LT( 0, s_pTablePtr->UpdateRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable) ) );
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


I_Message::I_Message( unsigned int nMessageID, unsigned int nMessageLength )
 : m_nCurDataPos( 0 ), m_nMessageID( nMessageID ), m_nMessageLength( nMessageLength )
{
}

unsigned int I_Message::MessageID()
{
	return m_nMessageID;
}

unsigned int I_Message::MessageLength()
{
	return m_nMessageLength;
}

void I_Message::Reset()
{
	m_nCurDataPos = 0;
}

NameTable4CodeTest::NameTable4CodeTest()
 : I_Message( 1234, sizeof(T_Message1234_NameTable) )
{
	if( m_vctMessage.capacity() == 0 )
	{
		m_vctMessage.reserve( 128 );

		m_vctMessage.push_back( T_Message1234_NameTable( "SR1831930", "中文商品名称1", 123 ) );
		m_vctMessage.push_back( T_Message1234_NameTable( "m2075-C-3000", "中文商品名称2", 100 ) );
		m_vctMessage.push_back( T_Message1234_NameTable( "m2075-P-9000", "商品名称3", 200 ) );
		m_vctMessage.push_back( T_Message1234_NameTable( "IH2075-C-3000", "中文商品名称4", 300 ) );
		m_vctMessage.push_back( T_Message1234_NameTable( "SR1001930", "商品名称5", 125 ) );
		m_vctMessage.push_back( T_Message1234_NameTable( "SR1231930", "中文商品名称6", 126 ) );
	}
}

bool NameTable4CodeTest::MoveNext()
{
	if( 0 == m_vctMessage.size() )	{	return false;	}
	++m_nCurDataPos;

	if( m_nCurDataPos >= m_vctMessage.size() )	{
		m_nCurDataPos = 0;
		return false;
	}

	return true;
}

char* NameTable4CodeTest::MessagePointer()
{
	return (char*)&(m_vctMessage[m_nCurDataPos]);
}

SnapTable4ValueTest::SnapTable4ValueTest()
 : I_Message( 2003, sizeof(T_Message2003_SnapTable) )
{
	if( m_vctMessage.capacity() == 0 )
	{
		m_vctMessage.reserve( 128 );

		m_vctMessage.push_back( T_Message2003_SnapTable( "SR1831930", 9100, 12000, 8000, 123456.011, 987655443 ) );
		m_vctMessage.push_back( T_Message2003_SnapTable( "m2075-C-3000", 2000, 3000, 1000, 8723.001, 8893240 ) );
		m_vctMessage.push_back( T_Message2003_SnapTable( "m2075-P-9000", 10000, 12000, 9000, 324.002, 988765 ) );
		m_vctMessage.push_back( T_Message2003_SnapTable( "IH2075-C-3000", 100, 200, 50, 123209.1, 93024 ) );
		m_vctMessage.push_back( T_Message2003_SnapTable( "SR1001930", 5000, 5600, 4700, 3242515.106, 899912312 ) );
		m_vctMessage.push_back( T_Message2003_SnapTable( "SR1231930", 6100, 8000, 5600, 32432.12, 9121211 ) );
	}
}

bool SnapTable4ValueTest::MoveNext()
{
	if( 0 == m_vctMessage.size() )	{	return false;	}
	++m_nCurDataPos;

	if( m_nCurDataPos >= m_vctMessage.size() )	{
		m_nCurDataPos = 0;
		return false;
	}

	return true;
}

char* SnapTable4ValueTest::MessagePointer()
{
	return (char*)&(m_vctMessage[m_nCurDataPos]);
}

VariableWidthTable4MsgIDTest::VariableWidthTable4MsgIDTest()
 : I_Message( 0, sizeof(T_MessageX_Table) )
{
	if( m_vctMessage.capacity() == 0 )
	{
		m_vctMessage.reserve( 132 );

		for( int n = 0; n < 128; n++ )
		{
			char			pszCode[64] = { 0 };

			::sprintf( pszCode, "m2017-C-%u", n*100 );
			m_vctMessage.push_back( T_MessageX_Table( pszCode, n*218 ) );
		}
	}
}

unsigned int VariableWidthTable4MsgIDTest::MessageID()
{
	unsigned int	nPos = m_nCurDataPos;

	if( m_nCurDataPos >= m_vctMessage.size() )
	{
		nPos = 0;
	}

	T_MessageX_Table&	refData = m_vctMessage[nPos];
	unsigned int		nMsgID = ::time(NULL) % (refData.MsgID+1);

	return nMsgID;
}

bool VariableWidthTable4MsgIDTest::MoveNext()
{
	if( 0 == m_vctMessage.size() )	{	return false;	}
	++m_nCurDataPos;

	if( m_nCurDataPos >= m_vctMessage.size() )	{
		m_nCurDataPos = 0;
		return false;
	}

	return true;
}

char* VariableWidthTable4MsgIDTest::MessagePointer()
{
	return (char*)&(m_vctMessage[m_nCurDataPos]);
}



TestAnyMessage_ID_X::TestAnyMessage_ID_X()
{
	if( m_vctIMessage.size() == 0 )
	{
		static	NameTable4CodeTest					s_objNameTable4CodeTest;
		static	SnapTable4ValueTest					s_objSnapTable4ValueTest;
		static	VariableWidthTable4MsgIDTest		s_objVariableWidthMsg4MsgIDTest;

		m_vctIMessage.push_back( &s_objVariableWidthMsg4MsgIDTest );
		m_vctIMessage.push_back( &s_objNameTable4CodeTest );
		m_vctIMessage.push_back( &s_objSnapTable4ValueTest );
	}
}

void TestAnyMessage_ID_X::SetUpTestCase()
{
}

void TestAnyMessage_ID_X::TearDownTestCase()
{
}

void TestAnyMessage_ID_X::CreateAllTable()
{
	unsigned int		nCount = m_vctIMessage.size();

	for( unsigned int n = 0; n < nCount; n++ )
	{
		I_Message*			pMessage = m_vctIMessage[n];

		do
		{
			ASSERT_EQ( true, UnitTestEnv::GetDatabasePtr()->CreateTable( pMessage->MessageID(), pMessage->MessageLength(), 32 ) );
		} while( true == pMessage->MoveNext() );
	}
}

void TestAnyMessage_ID_X::TestLocateTable( unsigned int nBindID, I_Table** pTable )
{
	*pTable = UnitTestEnv::GetDatabasePtr()->QueryTable( nBindID );

	ASSERT_NE( *pTable, (I_Table*)NULL );			///< 返回数据表指针不能为空
}

void TestAnyMessage_ID_X::Insert1Table( I_Message* pMessage, bool IsExist )
{
	I_Table*			pTable = NULL;
	TestLocateTable( pMessage->MessageID(), &pTable );

	if( false == IsExist )
	{
		ASSERT_EQ( pTable, (I_Table*)NULL );
		return;
	}

	ASSERT_NE( pTable, (I_Table*)NULL );

	do
	{
		ASSERT_LT( 0, pTable->InsertRecord( pMessage->MessagePointer(), pMessage->MessageLength() ) );

		RecordBlock	record = pTable->SelectRecord( pMessage->MessagePointer(), pMessage->MessageLength() );
		ASSERT_NE( true, record.IsNone() );				///< 返回数据表指针不能为空
		ASSERT_EQ( true, record.Compare( RecordBlock(pMessage->MessagePointer(), pMessage->MessageLength()) ) );
	} while( true == pMessage->MoveNext() );
}

void TestAnyMessage_ID_X::Update1Table( I_Message* pMessage, bool IsExist )
{
	I_Table*			pTable = NULL;
	TestLocateTable( pMessage->MessageID(), &pTable );

	do
	{
		if( true == IsExist )
		{
			ASSERT_EQ( 1, pTable->UpdateRecord( pMessage->MessagePointer(), pMessage->MessageLength() ) );
		}
		else
		{
			ASSERT_EQ( 0, pTable->UpdateRecord( pMessage->MessagePointer(), pMessage->MessageLength() ) );
		}

		RecordBlock	record = pTable->SelectRecord( pMessage->MessagePointer(), pMessage->MessageLength() );
		ASSERT_NE( true, record.IsNone() );				///< 返回数据表指针不能为空
		ASSERT_EQ( true, record.Compare( RecordBlock(pMessage->MessagePointer(), pMessage->MessageLength()) ) );
	} while( true == pMessage->MoveNext() );
}

void TestAnyMessage_ID_X::Query1Table( I_Message* pMessage, bool IsExist )
{
	I_Table*			pTable = NULL;
	TestLocateTable( pMessage->MessageID(), &pTable );

	RecordBlock	record = pTable->SelectRecord( pMessage->MessagePointer(), pMessage->MessageLength() );

	if( true == IsExist )
	{
		ASSERT_NE( true, record.IsNone() );				///< 返回数据表指针不能为空
		ASSERT_EQ( true, record.Compare( RecordBlock(pMessage->MessagePointer(), pMessage->MessageLength()) ) );
	}
	else
	{
		ASSERT_NE( false, record.IsNone() );				///< 返回数据表指针不能为空
		ASSERT_EQ( false, record.Compare( RecordBlock(pMessage->MessagePointer(), pMessage->MessageLength()) ) );
	}
}

void TestAnyMessage_ID_X::SetUp()
{
	CreateAllTable();

	unsigned int		nCount = m_vctIMessage.size();

	for( unsigned int n = 0; n < nCount; n++ )
	{
		Insert1Table( m_vctIMessage[n], true );
	}

	for( unsigned int m = 0; m < nCount; m++ )
	{
		Update1Table( m_vctIMessage[m], true );
	}
}

void TestAnyMessage_ID_X::TearDown()
{
//	TestLocateTable();
}


///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------


TEST_F( TestCreateNameTable1000_ZeroMemo, InsertOption_ZeroMemoryOfNameTable )
{
	TestLocateTable();TestInsertRecord();
	TestLocateTable();TestSelectRecord();
}

TEST_F( TestCreateNameTable1000_ZeroMemo, InsertOption_ZeroMemoryOfNameTable_Repeat )
{
	TestLocateTable();TestInsertRecord();
	TestLocateTable();TestSelectRecord();
}

TEST_F( TestCreateNameTable1000_Normal, InsertOptionOfNormalNameTable )
{
	TestLocateTable();TestInsertRecord();
	TestLocateTable();TestSelectRecord();
}

TEST_F( TestCreateNameTable1000_Normal, InsertOptionOfNormalNameTable_Repeat )
{
	TestInsertRecord();TestSelectRecord();
}

TEST_F( TestCreateNameTable1000_Normal, UpdateOptionOfNormalNameTable )
{
	TestUpdateRecord();TestSelectRecord();
}

TEST_F( TestCreateNameTable1000_Normal, UpdateOptionOfNormalNameTable_Repeat )
{
	TestUpdateRecord();TestSelectRecord();
}

TEST_F( TestAnyMessage_ID_X, AllWorkFlowTest )
{
}



///< ---------------------- 单元测试初始化类定义 -------------------------


I_Database* UnitTestEnv::m_pIDatabase = NULL;

I_Database* UnitTestEnv::GetDatabasePtr()
{
	return m_pIDatabase;
}

void UnitTestEnv::SetUp()
{
	m_pIDatabase = GetFactoryObject().GrapDatabaseInterface();
	ASSERT_NE( m_pIDatabase, (I_Database*)NULL );
}

void UnitTestEnv::TearDown()
{
	ASSERT_EQ( GetFactoryObject().ReleaseAllDatabase(), true );
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











