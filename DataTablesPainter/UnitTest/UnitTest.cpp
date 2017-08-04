#include "UnitTest.h"
#include "shellapi.h"
#include "windows.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "../MemTable/MemTable.h"
#include "../DataTablesPainter.h"


///< --------------------- 单元测试类定义 --------------------------------


std::vector<TestTableOperation::T_Message_MarketInfo> TestTableOperation::m_vctMarketInfo;
std::vector<TestTableOperation::T_Message_NameTable> TestTableOperation::m_vctNameTable;
std::vector<TestTableOperation::T_Message_SnapTable> TestTableOperation::m_vctSnapTable;


TestTableOperation::TestTableOperation()
: m_pCurTablePtr( NULL )
{
}

void TestTableOperation::SetUpTestCase()
{
}

void TestTableOperation::TearDownTestCase()
{
}

void TestTableOperation::SetUp()
{
	TestCreateAllTable();

	if( true == m_vctMarketInfo.empty() )
	{
		m_vctMarketInfo.push_back( T_Message_MarketInfo( "marketinfo1", 16, 20101201, 93018 ) );
		m_vctMarketInfo.push_back( T_Message_MarketInfo( "marketinfo2", 36, 20121209, 103108 ) );
		m_vctMarketInfo.push_back( T_Message_MarketInfo( "marketinfo3", 56, 20161901, 123636 ) );
	}

	if( true == m_vctNameTable.empty() )
	{
		m_vctNameTable.push_back( T_Message_NameTable( "SR1441930", "某种商品名称", 10, 12, 1, 60 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "600000", "上海市场", 12, 0, 1, 2 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "1m075-C-3000", "中文商品名称", 8, 6, 1, 7 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "8M075-P-9000", "某种商品名称", 1, 8, 8, 2 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "6IH075-C-3000", "商品名称3", 2, 5, 3, 555 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "1SR001930", "中文商品名称4", 3, 3, 0, 100 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "SR231930", "中文商品名称6", 6, 321, 2, 9 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "aR441930", "a某种商品名称", 10, 12, 1, 60 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "b00000", "b上海市场", 12, 0, 1, 2 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "1c075-C-3000", "c中文商品名称", 8, 6, 1, 7 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "3d075-P-9000", "d某种商品名称", 1, 8, 8, 2 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "9e075-C-3000", "e商品名称3", 2, 5, 3, 555 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "f101930", "f中文商品名称4", 3, 3, 0, 100 ) );
		m_vctNameTable.push_back( T_Message_NameTable( "g131930", "g中文商品名称6", 6, 321, 2, 9 ) );
	}

	if( true == m_vctSnapTable.empty() )
	{
		m_vctSnapTable.push_back( T_Message_SnapTable( "SR1441930", 10000, 12000, 9000, 123456000.102, 23415435 ) );
		m_vctSnapTable.push_back( T_Message_SnapTable( "600000", 20000, 22000, 19000, 223456000.102, 33415435 ) );
		m_vctSnapTable.push_back( T_Message_SnapTable( "m2075-C-3000", 30000, 32000, 29000, 323456000.102, 43415435 ) );
		m_vctSnapTable.push_back( T_Message_SnapTable( "M2075-P-9000", 40000, 42000, 39000, 423456000.102, 53415435 ) );
		m_vctSnapTable.push_back( T_Message_SnapTable( "IH2075-C-3000", 50000, 52000, 49000, 523456000.102, 63415435 ) );
		m_vctSnapTable.push_back( T_Message_SnapTable( "SR1001930", 60000, 62000, 59000, 623456000.102, 73415435 ) );
		m_vctSnapTable.push_back( T_Message_SnapTable( "SR1231930", 70000, 72000, 69000, 723456000.102, 83415435 ) );

		for( unsigned int i = 1; i <= 1024*2; i++ )
		{
			char			pszCode[32] = { 0 };
			unsigned int	nNow = (i+1) * 1024;
			unsigned int	nHigh = nNow + 100 * i;
			unsigned int	nLow = nNow - 100 * i;

			::sprintf( pszCode, "UFO%u-AP-100", i );
			m_vctSnapTable.push_back( T_Message_SnapTable( pszCode, nNow, nHigh, nLow, nNow*123342.102, nNow*1024 ) );
		}

		for( unsigned int j = 1; j <= 1024*2; j++ )
		{
			char			pszCode[32] = { 0 };
			unsigned int	nNow = (j+1) * 1024;
			unsigned int	nHigh = nNow + 100 * j;
			unsigned int	nLow = nNow - 100 * j;

			::sprintf( pszCode, "%u-c-1b0a", 10000 + j );
			m_vctSnapTable.push_back( T_Message_SnapTable( pszCode, nNow, nHigh, nLow, nNow*123342.102, nNow*1024 ) );
		}
	}

	m_nMaxLoopNum = m_vctSnapTable.size();
}

void TestTableOperation::TearDown()
{
}

void TestTableOperation::TestCreateAllTable()
{
	for( int n = 0; n < 3; n++ )
	{
		ASSERT_EQ( true, UnitTestEnv::GetDatabasePtr()->CreateTable( T_Message_MarketInfo::GetID(), sizeof(T_Message_MarketInfo), 32 ) );
		ASSERT_EQ( true, UnitTestEnv::GetDatabasePtr()->CreateTable( T_Message_NameTable::GetID(), sizeof(T_Message_NameTable), 32 ) );
		ASSERT_EQ( true, UnitTestEnv::GetDatabasePtr()->CreateTable( T_Message_SnapTable::GetID(), sizeof(T_Message_SnapTable), 32 ) );
	}
}

void TestTableOperation::TestUpdateMarketInfo( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_MarketInfo&	refData = m_vctMarketInfo[nSeed % m_vctMarketInfo.size()];
	bool					bEmptyUpdate = (0==(nSeed % 7)) && bIsExist;

	if( false == bIsExist )
	{
		ASSERT_EQ( 0, m_pCurTablePtr->UpdateRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
	else
	{
		if( true == bEmptyUpdate )
		{
			ASSERT_EQ( 0, m_pCurTablePtr->UpdateRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
		}
		else
		{
			refData.MarketTime++;
			ASSERT_EQ( 1, m_pCurTablePtr->UpdateRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
		}
	}
}

void TestTableOperation::TestUpdateNameTable( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_NameTable&	refData = m_vctNameTable[nSeed % m_vctNameTable.size()];
	bool					bEmptyUpdate = (0==(nSeed % 7)) && bIsExist;

	if( false == bIsExist )
	{
		ASSERT_EQ( 0, m_pCurTablePtr->UpdateRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
	else
	{
		if( true == bEmptyUpdate )
		{
			ASSERT_EQ( 0, m_pCurTablePtr->UpdateRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
		}
		else
		{
			refData.Number1++;
			ASSERT_EQ( 1, m_pCurTablePtr->UpdateRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
		}
	}
}

void TestTableOperation::TestUpdateSnapTable( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_SnapTable&	refData = m_vctSnapTable[nSeed % m_vctSnapTable.size()];
	bool					bEmptyUpdate = (0==(nSeed % 7)) && bIsExist;

	if( false == bIsExist )
	{
		ASSERT_EQ( 0, m_pCurTablePtr->UpdateRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
	else
	{
		if( true == bEmptyUpdate )
		{
			ASSERT_EQ( 0, m_pCurTablePtr->UpdateRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
		}
		else
		{
			refData.Volume++;
			ASSERT_EQ( 1, m_pCurTablePtr->UpdateRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
		}
	}
}

void TestTableOperation::TestLocateMarketInfo( bool bIsExist )
{
	I_Table*	pTable = UnitTestEnv::GetDatabasePtr()->QueryTable( T_Message_MarketInfo::GetID() );

	m_pCurTablePtr = NULL;

	if( false == bIsExist )
	{
		ASSERT_EQ( pTable, (I_Table*)NULL );		///< 返回数据表指针为空
		return ;
	}

	ASSERT_NE( pTable, (I_Table*)NULL );			///< 返回数据表指针不能为空

	m_pCurTablePtr = pTable;
}

void TestTableOperation::TestLocateNameTable( bool bIsExist )
{
	I_Table*	pTable = UnitTestEnv::GetDatabasePtr()->QueryTable( T_Message_NameTable::GetID() );

	m_pCurTablePtr = NULL;

	if( false == bIsExist )
	{
		ASSERT_EQ( pTable, (I_Table*)NULL );		///< 返回数据表指针为空
		return ;
	}

	ASSERT_NE( pTable, (I_Table*)NULL );			///< 返回数据表指针不能为空

	m_pCurTablePtr = pTable;
}

void TestTableOperation::TestLocateSnapTable( bool bIsExist )
{
	I_Table*	pTable = UnitTestEnv::GetDatabasePtr()->QueryTable( T_Message_SnapTable::GetID() );

	m_pCurTablePtr = NULL;

	if( false == bIsExist )
	{
		ASSERT_EQ( pTable, (I_Table*)NULL );		///< 返回数据表指针为空
		return ;
	}

	ASSERT_NE( pTable, (I_Table*)NULL );			///< 返回数据表指针不能为空

	m_pCurTablePtr = pTable;
}

void TestTableOperation::TestInsertMarketInfo( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_MarketInfo&	refData = m_vctMarketInfo[nSeed % m_vctMarketInfo.size()];

	if( false == bIsExist )
	{
		ASSERT_EQ( 1, m_pCurTablePtr->InsertRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
	else
	{
		ASSERT_EQ( 0, m_pCurTablePtr->InsertRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
}

void TestTableOperation::TestInsertNameTable( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_NameTable&	refData = m_vctNameTable[nSeed % m_vctNameTable.size()];

	if( false == bIsExist )
	{
		ASSERT_EQ( 1, m_pCurTablePtr->InsertRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
	else
	{
		ASSERT_EQ( 0, m_pCurTablePtr->InsertRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
}

void TestTableOperation::TestInsertSnapTable( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_SnapTable&	refData = m_vctSnapTable[nSeed % m_vctSnapTable.size()];

	if( false == bIsExist )
	{
		ASSERT_EQ( 1, m_pCurTablePtr->InsertRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
	else
	{
		ASSERT_EQ( 0, m_pCurTablePtr->InsertRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
}

void TestTableOperation::TestDeleteMarketInfo( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_MarketInfo&	refData = m_vctMarketInfo[nSeed % m_vctMarketInfo.size()];

	if( true == bIsExist )
	{
		ASSERT_EQ( 1, m_pCurTablePtr->DeleteRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
	else
	{
		ASSERT_EQ( 0, m_pCurTablePtr->DeleteRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
}

void TestTableOperation::TestDeleteNameTable( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_NameTable&	refData = m_vctNameTable[nSeed % m_vctNameTable.size()];

	if( true == bIsExist )
	{
		ASSERT_EQ( 1, m_pCurTablePtr->DeleteRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
	else
	{
		ASSERT_EQ( 0, m_pCurTablePtr->DeleteRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
}

void TestTableOperation::TestDeleteSnapTable( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_SnapTable&	refData = m_vctSnapTable[nSeed % m_vctSnapTable.size()];

	if( true == bIsExist )
	{
		ASSERT_EQ( 1, m_pCurTablePtr->DeleteRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
	else
	{
		ASSERT_EQ( 0, m_pCurTablePtr->DeleteRecord( (char*)&refData, sizeof(refData), nSerialNo ) );
	}
}

void TestTableOperation::TestSelectMarketInfo( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_MarketInfo&	refData = m_vctMarketInfo[nSeed % m_vctMarketInfo.size()];

	if( false == bIsExist )
	{
		RecordBlock	record = m_pCurTablePtr->SelectRecord( (char*)&refData, sizeof(refData) );
		ASSERT_EQ( true, record.IsNone() );				///< 返回数据表
	}
	else
	{
		RecordBlock	record = m_pCurTablePtr->SelectRecord( (char*)&refData, sizeof(refData) );
		ASSERT_NE( true, record.IsNone() );				///< 返回数据表指针不能为空
		ASSERT_EQ( true, record.Compare( RecordBlock((char*)&refData, sizeof(refData)) ) );
	}
}

void TestTableOperation::TestSelectNameTable( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_NameTable&	refData = m_vctNameTable[nSeed % m_vctNameTable.size()];

	if( false == bIsExist )
	{
		RecordBlock	record = m_pCurTablePtr->SelectRecord( (char*)&refData, sizeof(refData) );
		ASSERT_EQ( true, record.IsNone() );				///< 返回数据表
	}
	else
	{
		RecordBlock	record = m_pCurTablePtr->SelectRecord( (char*)&refData, sizeof(refData) );
		ASSERT_EQ( false, record.IsNone() );			///< 返回数据表指针不能为空
		ASSERT_EQ( true, record.Compare( RecordBlock((char*)&refData, sizeof(refData)) ) );
	}
}

void TestTableOperation::TestSelectSnapTable( unsigned int nSeed, bool bIsExist )
{
	unsigned __int64		nSerialNo = 0;
	T_Message_SnapTable&	refData = m_vctSnapTable[nSeed % m_vctSnapTable.size()];

	if( false == bIsExist )
	{
		RecordBlock	record = m_pCurTablePtr->SelectRecord( (char*)&refData, sizeof(refData) );
		ASSERT_EQ( true, record.IsNone() );				///< 返回数据表
	}
	else
	{
		RecordBlock	record = m_pCurTablePtr->SelectRecord( (char*)&refData, sizeof(refData) );
		ASSERT_NE( true, record.IsNone() );				///< 返回数据表指针不能为空
		ASSERT_EQ( true, record.Compare( RecordBlock((char*)&refData, sizeof(refData)) ) );
	}
}

void TestTableOperation::TestDeleteAllTables()
{
	ASSERT_EQ( 0, UnitTestEnv::GetDatabasePtr()->DeleteTables() );
}

void TestTableOperation::TestDeleteOneTable( unsigned int nTableID )
{
	ASSERT_EQ( true, UnitTestEnv::GetDatabasePtr()->DeleteTable( nTableID ) );

	I_Table*	pTable = UnitTestEnv::GetDatabasePtr()->QueryTable( nTableID );
	ASSERT_EQ( NULL, pTable );
}


///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------


///< --------------- 对数据库进行空落盘/加载测试 --------------------------------------
TEST_F( TestTableOperation, DumpEmptyTablesAndLoad )
{
	::printf( "\n **************** 执行测试前, 先清空DataRecover目录 ******************* \n" );
	::Sleep( 3000 );
	::system( "del /f/s/q .\\DataRecover\\*" );

	TestDeleteAllTables();
	ASSERT_EQ( UnitTestEnv::GetDatabasePtr()->SaveToDisk( "./DataRecover/" ), false );
	ASSERT_LT( UnitTestEnv::GetDatabasePtr()->LoadFromDisk( "./DataRecover/" ), 0 );

	for( int n = 0; n < 2; n++ )
	{
		TestLocateMarketInfo( false );
		TestLocateNameTable( false );
		TestLocateSnapTable( false );
	}
}

///< ----------------- 测试单条记录的插入和删除测试 ------------------------------------
TEST_F( TestTableOperation, InsertOneRecordAndDelete )
{
	TestLocateMarketInfo();TestInsertMarketInfo( 0 );TestSelectMarketInfo( 0, true );
	TestDeleteMarketInfo( 0, true );TestSelectMarketInfo( 0, false );
	ASSERT_EQ( 0, ((MemoryCollection::DynamicTable*)m_pCurTablePtr)->GetRecordCount() );

	TestLocateNameTable();TestInsertNameTable( 0 );TestSelectNameTable( 0, true );
	TestDeleteNameTable( 0, true );TestSelectNameTable( 0, false );
	ASSERT_EQ( 0, ((MemoryCollection::DynamicTable*)m_pCurTablePtr)->GetRecordCount() );

	TestLocateSnapTable();TestInsertSnapTable( 0 );TestSelectSnapTable( 0, true );
	TestDeleteSnapTable( 0, true );TestSelectSnapTable( 0, false );
	ASSERT_EQ( 0, ((MemoryCollection::DynamicTable*)m_pCurTablePtr)->GetRecordCount() );
}

///< ------------------ 测试全部记录的插入+落盘+读盘测试 ----------------------------------------
TEST_F( TestTableOperation, InsertAllRecordAndDeleteAll )
{
	bool	bIsExistInsert = false;
	int		nMkListSize = m_vctMarketInfo.size();
	int		nNameTableSize = m_vctNameTable.size();
	int		nSnapTableSize = m_vctSnapTable.size();

	for( int n = 0; n < m_nMaxLoopNum*2; n++ )
	{
		bIsExistInsert = n>=nMkListSize?true:false;
		TestLocateMarketInfo();TestInsertMarketInfo( n, bIsExistInsert );TestSelectMarketInfo( n, true );
		bIsExistInsert = n>=nNameTableSize?true:false;
		TestLocateNameTable();TestInsertNameTable( n, bIsExistInsert );TestSelectNameTable( n, true );
		bIsExistInsert = n>=nSnapTableSize?true:false;
		TestLocateSnapTable();TestInsertSnapTable( n, bIsExistInsert );TestSelectSnapTable( n, true );
	}

	///< 顺着删除全部记录
	for( int m = 0; m < m_nMaxLoopNum*2; m++ )
	{
		bIsExistInsert = m<nMkListSize?true:false;
		TestLocateMarketInfo();TestDeleteMarketInfo( m, bIsExistInsert );TestSelectMarketInfo( m, false );
		bIsExistInsert = m<nNameTableSize?true:false;
		TestLocateNameTable();TestDeleteNameTable( m, bIsExistInsert );TestSelectNameTable( m, false );
		bIsExistInsert = m<nSnapTableSize?true:false;
		TestLocateSnapTable();TestDeleteSnapTable( m, bIsExistInsert );TestSelectSnapTable( m, false );
	}

	for( int nn = 0; nn < m_nMaxLoopNum*2; nn++ )
	{
		bIsExistInsert = nn>=nMkListSize?true:false;
		TestLocateMarketInfo();TestInsertMarketInfo( nn, bIsExistInsert );TestSelectMarketInfo( nn, true );
		bIsExistInsert = nn>=nNameTableSize?true:false;
		TestLocateNameTable();TestInsertNameTable( nn, bIsExistInsert );TestSelectNameTable( nn, true );
		bIsExistInsert = nn>=nSnapTableSize?true:false;
		TestLocateSnapTable();TestInsertSnapTable( nn, bIsExistInsert );TestSelectSnapTable( nn, true );
	}

	///< 倒着删除全部记录
	for( int a = nMkListSize - 1; a >= 0; a-- )
	{
		TestLocateMarketInfo();TestDeleteMarketInfo( a, bIsExistInsert );TestSelectMarketInfo( a, false );
	}
	for( int b = nNameTableSize - 1; b >= 0; b-- )
	{
		TestLocateNameTable();TestDeleteNameTable( b, bIsExistInsert );TestSelectNameTable( b, false );
	}
	for( int c = nSnapTableSize - 1; c >= 0; c-- )
	{
		TestLocateSnapTable();TestDeleteSnapTable( c, bIsExistInsert );TestSelectSnapTable( c, false );
	}

	for( int nn = 0; nn < m_nMaxLoopNum*2; nn++ )
	{
		bIsExistInsert = nn>=nMkListSize?true:false;
		TestLocateMarketInfo();TestInsertMarketInfo( nn, bIsExistInsert );TestSelectMarketInfo( nn, true );
		bIsExistInsert = nn>=nNameTableSize?true:false;
		TestLocateNameTable();TestInsertNameTable( nn, bIsExistInsert );TestSelectNameTable( nn, true );
		bIsExistInsert = nn>=nSnapTableSize?true:false;
		TestLocateSnapTable();TestInsertSnapTable( nn, bIsExistInsert );TestSelectSnapTable( nn, true );
	}
}

///< 对全部记录的数据表进行落盘/加载测试
TEST_F( TestTableOperation, DumpAllDataAndLoadAfterInsertAllRecords )
{
	///< 保存前全部记录校验
	for( int n = 0; n < m_nMaxLoopNum; n++ )
	{
		TestLocateMarketInfo();TestSelectMarketInfo( n, true );
		TestLocateNameTable();TestSelectNameTable( n, true );
		TestLocateSnapTable();TestSelectSnapTable( n, true );
	}

	ASSERT_EQ( UnitTestEnv::GetDatabasePtr()->SaveToDisk( "./DataRecover/" ), true );
	ASSERT_GT( UnitTestEnv::GetDatabasePtr()->LoadFromDisk( "./DataRecover/" ), 0 );

	///< 重载后全部记录校验
	for( int n = 0; n < m_nMaxLoopNum; n++ )
	{
		TestLocateMarketInfo();TestSelectMarketInfo( n, true );
		TestLocateNameTable();TestSelectNameTable( n, true );
		TestLocateSnapTable();TestSelectSnapTable( n, true );
	}
}

///< ---------------------- 删除部分记录+落盘+读盘 -----------------------------------------------
TEST_F( TestTableOperation, DeleteSomeRecordWithUpdate )
{
	///< 删除前检查校对数据
	for( int n = 0; n < m_nMaxLoopNum; n++ )
	{
		TestLocateMarketInfo();TestSelectMarketInfo( n, true );
		TestLocateNameTable();TestSelectNameTable( n, true );
		TestLocateSnapTable();TestSelectSnapTable( n, true );
	}

	TestLocateMarketInfo();TestDeleteMarketInfo( 1, true );TestSelectMarketInfo( 1, false );
	for( int n = 0; n < m_nMaxLoopNum; n++ )
	{
		bool bIsExist = true;
		switch( n%m_vctMarketInfo.size() )
		{
		case 1:
			bIsExist = false;
			break;
		}
		TestUpdateMarketInfo( n, bIsExist );
		TestSelectMarketInfo( n, bIsExist );
	}

	TestLocateNameTable();TestDeleteNameTable( 1, true );TestSelectNameTable( 1, false );
	TestLocateNameTable();TestDeleteNameTable( 5, true );TestSelectNameTable( 5, false );
	for( int a = 0; a < 10; a++ )
	{
		TestDeleteNameTable( 1, false );TestSelectNameTable( 1, false );
		TestDeleteNameTable( 5, false );TestSelectNameTable( 5, false );
	}
	for( int n = 0; n < m_nMaxLoopNum; n++ )
	{
		bool bIsExist = true;
		switch( n%m_vctNameTable.size() )
		{
		case 1:
		case 5:
			bIsExist = false;
			break;
		}
		TestUpdateNameTable( n, bIsExist );
		TestSelectNameTable( n, bIsExist );
	}

	TestLocateSnapTable();TestDeleteSnapTable( 0, true );TestSelectSnapTable( 0, false );
	TestLocateSnapTable();TestDeleteSnapTable( 3, true );TestSelectSnapTable( 3, false );
	TestLocateSnapTable();TestDeleteSnapTable( m_vctSnapTable.size()-1, true );TestSelectSnapTable( m_vctSnapTable.size()-1, false );
	for( int b = 0; b < 10; b++ )
	{
		TestDeleteSnapTable( 0, false );TestSelectSnapTable( 0, false );
		TestDeleteSnapTable( 3, false );TestSelectSnapTable( 3, false );
		TestDeleteSnapTable( m_vctSnapTable.size()-1, false );TestSelectSnapTable( m_vctSnapTable.size()-1, false );
	}
	for( int n = 0; n < m_nMaxLoopNum; n++ )
	{
		bool			bIsExist = true;
		unsigned int	nMod = n % m_vctSnapTable.size();
		switch( nMod )
		{
		case 0:
		case 3:
			bIsExist = false;
			break;
		default:
			if( nMod == (m_vctSnapTable.size()-1) )
			{
				bIsExist = false;
			}
			break;
		}
		TestUpdateSnapTable( n, bIsExist );
		TestSelectSnapTable( n, bIsExist );
	}
}

///< 对全部记录的数据表进行落盘/加载测试
TEST_F( TestTableOperation, DumpAllDataAndLoadAfterDeleteSomeRecords )
{
	ASSERT_EQ( UnitTestEnv::GetDatabasePtr()->SaveToDisk( "./DataRecover/" ), true );
	ASSERT_GT( UnitTestEnv::GetDatabasePtr()->LoadFromDisk( "./DataRecover/" ), 0 );

	TestLocateMarketInfo();
	for( int n = 0; n < m_nMaxLoopNum; n++ )
	{
		bool bIsExist = true;
		switch( n%m_vctMarketInfo.size() )
		{
		case 1:
			bIsExist = false;
			break;
		}
		TestUpdateMarketInfo( n, bIsExist );
		TestSelectMarketInfo( n, bIsExist );
	}

	TestLocateNameTable();
	for( int n = 0; n < m_nMaxLoopNum; n++ )
	{
		bool bIsExist = true;
		switch( n%m_vctNameTable.size() )
		{
		case 1:
		case 5:
			bIsExist = false;
			break;
		}
		TestUpdateNameTable( n, bIsExist );
		TestSelectNameTable( n, bIsExist );
	}

	TestLocateSnapTable();
	for( int n = 0; n < m_nMaxLoopNum; n++ )
	{
		bool			bIsExist = true;
		unsigned int	nMod = n % m_vctSnapTable.size();
		switch( nMod )
		{
		case 0:
		case 3:
			bIsExist = false;
			break;
		default:
			if( nMod == (m_vctSnapTable.size()-1) )
			{
				bIsExist = false;
			}
			break;
		}
		TestUpdateSnapTable( n, bIsExist );
		TestSelectSnapTable( n, bIsExist );
	}
}

///< 创建一堆数据库对象
TEST_F( TestTableOperation, CreateBundleOfDatabasePointer )
{
	for( int n = 0; n < 5; n++ )	{
		ASSERT_NE( GetFactoryObject().GrapDatabaseInterface(), (I_Database*)NULL );
	}
}

///< 对一数据库进行数据表的创建删除重复测试
TEST_F( TestTableOperation, CreateDeleteAllTablesTest )
{
	///< 创建所有数据表
	TestCreateAllTable();
	///< 创建所有数据表
	TestCreateAllTable();
	///< 创建所有数据表
	TestCreateAllTable();
	::printf( "test delete table operation ..........................................\n" );
	TestLocateNameTable( true );
	TestDeleteOneTable( T_Message_NameTable::GetID() );
	TestLocateMarketInfo( true );
	TestDeleteOneTable( T_Message_MarketInfo::GetID() );
	TestLocateSnapTable( true );
	TestDeleteOneTable( T_Message_SnapTable::GetID() );
	::printf( "happy ending  ........................................................\n" );
	///< 清空所有数据库
	TestDeleteAllTables();
	///< 随意定义一个不存在的数据表
	::printf( "query table in empty database.........................................\n" );
	TestLocateMarketInfo( false );
	TestLocateNameTable( false );
	TestLocateSnapTable( false );
	::printf( "happy ending..........................................................\n" );
	for( int a = 0; a < 10; a++ ) {
		///< 再次，创建所有数据表
		TestCreateAllTable();
		///< 再次，创建所有数据表
		TestCreateAllTable();
		///< 再次，创建所有数据表
		TestCreateAllTable();
		///< 再次,清空数据库
		TestDeleteAllTables();
		///< 再次,清空数据库
		TestDeleteAllTables();
		///< 再次,清空数据库
		TestDeleteAllTables();
		///< 再次,清空数据库
		TestDeleteAllTables();
		///< 再次,清空数据库
		TestDeleteAllTables();
	}
	///< 随意定义一个不存在的数据表
	::printf( "query table in empty database.........................................\n" );
	TestLocateMarketInfo( false );
	TestLocateNameTable( false );
	TestLocateSnapTable( false );
	::printf( "happy ending..........................................................\n" );
}


///< ---------------------- 单元测试初始化类定义 -------------------------


I_Database* UnitTestEnv::m_pIDatabase = NULL;

I_Database* UnitTestEnv::GetDatabasePtr()
{
	return m_pIDatabase;
}

void UnitTestEnv::SetUp()
{
	///< 创建一个数据库
	m_pIDatabase = GetFactoryObject().GrapDatabaseInterface();
	ASSERT_NE( m_pIDatabase, (I_Database*)NULL );
}

void UnitTestEnv::TearDown()
{
	///< 释放一堆数据库资源
	for( int n = 0; n < 10; n++ )	{
		ASSERT_EQ( GetFactoryObject().ReleaseAllDatabase(), true );
	}
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











