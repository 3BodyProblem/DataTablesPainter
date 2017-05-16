#include "UnitTest.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "../DataTablesPainter.h"


///< --------------------- ��Ԫ�����ඨ�� --------------------------------


I_Table* TestCreateNameTable1000_ZeroMemo::s_pTablePtr = NULL;
MemoryCollection::DynamicTable::TableMeta TestCreateNameTable1000_ZeroMemo::s_oTableMeta( 0, 0, 0 );
TestCreateNameTable1000_ZeroMemo::T_Message1000_NameTable TestCreateNameTable1000_ZeroMemo::s_sMsg1000NameTable = { 0 };

void TestCreateNameTable1000_ZeroMemo::SetUpTestCase()
{
	///< ��̬�򣺳�ʼ�����ݱ�ָ��
	s_pTablePtr = NULL;
	///< ��̬�򣺳�ʼ�����ݱ�Ԫ��Ϣ
	s_oTableMeta = MemoryCollection::DynamicTable::TableMeta( 1000, sizeof(T_Message1000_NameTable), sizeof(s_sMsg1000NameTable.SecurityID) );
	///< ��̬���������
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

	ASSERT_NE( pTable, (I_Table*)NULL );			///< �������ݱ�ָ�벻��Ϊ��

	if( NULL == s_pTablePtr )
	{
		s_pTablePtr = pTable;						///< ��һ�β�ѯ�����ݱ�ָ��
	}
	else
	{
		ASSERT_EQ( s_pTablePtr, pTable );			///< ֮��ÿ�β�ѯ�õ���ָ�붼Ӧ�õȼ��ڵ�һ�εĲ�ѯָ��
	}
}

void TestCreateNameTable1000_ZeroMemo::TestInsertRecord()
{
	for( int n = 0; n < 10; n++ ) {
		unsigned __int64	nSerialNo = 0;
		ASSERT_LE( 0, s_pTablePtr->InsertRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable), nSerialNo ) );
	}
}

void TestCreateNameTable1000_ZeroMemo::TestSelectRecord()
{
	for( int n = 0; n < 100; n++ ) {
		RecordBlock	record = s_pTablePtr->SelectRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable) );
		ASSERT_NE( true, record.IsNone() );				///< �������ݱ�ָ�벻��Ϊ��
		ASSERT_EQ( true, record.Compare( RecordBlock((char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable)) ) );
	}
}

void TestCreateNameTable1000_Normal::SetUpTestCase()
{
	TestCreateNameTable1000_ZeroMemo::SetUpTestCase();

	///< ��̬���������
	::sprintf( s_sMsg1000NameTable.SecurityID, "OK123456-C-%u", 2800 );
	::sprintf( s_sMsg1000NameTable.SecurityName, "�������� %d �ŵ�Ԫ", 1 );
	s_sMsg1000NameTable.SecurityType = 1024;
	s_sMsg1000NameTable.Number1 = 1;
	s_sMsg1000NameTable.Number2 = 2;
	s_sMsg1000NameTable.Number3 = 3;
}

void TestCreateNameTable1000_Normal::TestInsertRecord()
{
	for( int n = 0; n < 10; n++ ) {
		unsigned __int64	nSerialNo = 0;
		ASSERT_LE( 0, s_pTablePtr->InsertRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable), nSerialNo ) );
	}
}

void TestCreateNameTable1000_Normal::TestUpdateRecord()
{
	static unsigned long		nnn = 0;

	s_sMsg1000NameTable.Number1 = 100 * nnn;
	s_sMsg1000NameTable.Number2 = 200 * nnn;
	s_sMsg1000NameTable.Number3 = 300 * nnn;
	s_sMsg1000NameTable.SecurityType = 1024*3;
	::sprintf( s_sMsg1000NameTable.SecurityName, "�������� %d �ŵ�Ԫ", 2 );
	nnn++;

	for( int n = 0; n < 10; n++ )
	{
		unsigned __int64	nSerialNo = 0;

		if( 0 == n )
		{
			ASSERT_LT( 0, s_pTablePtr->UpdateRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable), nSerialNo ) );
		}
		else
		{
			ASSERT_EQ( 0, s_pTablePtr->UpdateRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable), nSerialNo ) );
		}
	}
}

void TestCreateNameTable1000_Normal::TestSelectRecord()
{
	for( int n = 0; n < 100; n++ ) {
		RecordBlock	record = s_pTablePtr->SelectRecord( (char*)&s_sMsg1000NameTable, sizeof(s_sMsg1000NameTable) );
		ASSERT_NE( true, record.IsNone() );				///< �������ݱ�ָ�벻��Ϊ��
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

		m_vctMessage.push_back( T_Message1234_NameTable( "SR1831930", "������Ʒ����1", 123 ) );
		m_vctMessage.push_back( T_Message1234_NameTable( "m2075-C-3000", "������Ʒ����2", 100 ) );
		m_vctMessage.push_back( T_Message1234_NameTable( "m2075-P-9000", "��Ʒ����3", 200 ) );
		m_vctMessage.push_back( T_Message1234_NameTable( "IH2075-C-3000", "������Ʒ����4", 300 ) );
		m_vctMessage.push_back( T_Message1234_NameTable( "SR1001930", "��Ʒ����5", 125 ) );
		m_vctMessage.push_back( T_Message1234_NameTable( "SR1231930", "������Ʒ����6", 126 ) );
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
	T_Message1234_NameTable&		refData = m_vctMessage[m_nCurDataPos];
	static	unsigned int			sInt = 0;

	refData.SecurityType = sInt++;

	return (char*)(&refData);
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
	T_Message2003_SnapTable&		refData = m_vctMessage[m_nCurDataPos];
	static	unsigned int			sInt = 0;

	refData.Now = sInt++;

	return (char*)(&refData);
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

	return refData.MsgID;
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
	T_MessageX_Table&		refData = m_vctMessage[m_nCurDataPos];
	static	unsigned int	sInt = 0;

	refData.Value = sInt++;

	return (char*)(&refData);
}


void TestAnyMessage_ID_X::SetUpTestCase()
{
}

void TestAnyMessage_ID_X::TearDownTestCase()
{
}

void TestAnyMessage_ID_X::TestCreateAllTable()
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

void TestAnyMessage_ID_X::TestLocateTable( unsigned int nBindID, I_Table** pTable, bool bIsExist )
{
	*pTable = UnitTestEnv::GetDatabasePtr()->QueryTable( nBindID );

	if( true == bIsExist )
	{
		ASSERT_NE( *pTable, (I_Table*)NULL );			///< �������ݱ�ָ�벻��Ϊ��
	}
	else
	{
		ASSERT_EQ( *pTable, (I_Table*)NULL );			///< �������ݱ�ָ�벻��Ϊ��
	}
}

void TestAnyMessage_ID_X::TestInsert1Table( I_Message* pMessage, bool IsExist, int nAffectNum )
{
	I_Table*			pTable = NULL;

	if( false == IsExist )
	{
		ASSERT_EQ( pTable, (I_Table*)NULL );
		return;
	}

	TestLocateTable( pMessage->MessageID(), &pTable, true );
	ASSERT_NE( pTable, (I_Table*)NULL );

	do
	{
		unsigned __int64	nSerialNo = 0;
		char				pszTmpData[512] = { 0 };

		::memcpy( pszTmpData, pMessage->MessagePointer(), pMessage->MessageLength() );
		ASSERT_EQ( nAffectNum, pTable->InsertRecord( pszTmpData, pMessage->MessageLength(), nSerialNo ) );

		RecordBlock	record = pTable->SelectRecord( pszTmpData, pMessage->MessageLength() );
		ASSERT_NE( true, record.IsNone() );				///< �������ݱ�ָ�벻��Ϊ��
		ASSERT_EQ( true, record.Compare( RecordBlock(pszTmpData, pMessage->MessageLength()) ) );
	} while( true == pMessage->MoveNext() );
}

void TestAnyMessage_ID_X::TestUpdate1Table( I_Message* pMessage, bool IsExist, int nAffectNum )
{
	I_Table*			pTable = NULL;

	TestLocateTable( pMessage->MessageID(), &pTable, IsExist );

	if( false == IsExist )
	{
		ASSERT_EQ( pTable, (I_Table*)NULL );
		return;
	}

	do
	{
		unsigned __int64	nSerialNo = 0;
		char				pszTmpData[512] = { 0 };

		::memcpy( pszTmpData, pMessage->MessagePointer(), pMessage->MessageLength() );
		ASSERT_EQ( nAffectNum, pTable->UpdateRecord( pszTmpData, pMessage->MessageLength(), nSerialNo ) );

		RecordBlock	record = pTable->SelectRecord( pszTmpData, pMessage->MessageLength() );
		if( 0 != nAffectNum )	{
			ASSERT_NE( true, record.IsNone() );				///< �������ݱ�ָ�벻��Ϊ��
			ASSERT_EQ( true, record.Compare( RecordBlock(pszTmpData, pMessage->MessageLength()) ) );
		}
	} while( true == pMessage->MoveNext() );
}

void TestAnyMessage_ID_X::TestQuery1Table( I_Message* pMessage, bool IsExist )
{
	I_Table*			pTable = NULL;
	TestLocateTable( pMessage->MessageID(), &pTable, IsExist );

	RecordBlock	record = pTable->SelectRecord( pMessage->MessagePointer(), pMessage->MessageLength() );

	if( true == IsExist )
	{
		ASSERT_NE( true, record.IsNone() );				///< �������ݱ�ָ�벻��Ϊ��
		ASSERT_EQ( true, record.Compare( RecordBlock(pMessage->MessagePointer(), pMessage->MessageLength()) ) );
	}
	else
	{
		ASSERT_NE( false, record.IsNone() );			///< �������ݱ�ָ�벻��Ϊ��
		ASSERT_EQ( false, record.Compare( RecordBlock(pMessage->MessagePointer(), pMessage->MessageLength()) ) );
	}
}

void TestAnyMessage_ID_X::TestDeleteAllTables()
{
	UnitTestEnv::GetDatabasePtr()->DeleteTables();
}

void TestAnyMessage_ID_X::SetUp()
{
	if( m_vctIMessage.size() == 0 )						///< ע�������õ�����Ϣ����
	{
		static	NameTable4CodeTest					s_objNameTable4CodeTest;
		static	SnapTable4ValueTest					s_objSnapTable4ValueTest;
		static	VariableWidthTable4MsgIDTest		s_objVariableWidthMsg4MsgIDTest;

		m_vctIMessage.push_back( &s_objVariableWidthMsg4MsgIDTest );
		m_vctIMessage.push_back( &s_objNameTable4CodeTest );
		m_vctIMessage.push_back( &s_objSnapTable4ValueTest );
	}
}

void TestAnyMessage_ID_X::TearDown()
{
}


///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------
///< ------------------------ ������������ ----------------------------------------------------

///< �սṹ����
TEST_F( TestCreateNameTable1000_ZeroMemo, InsertTest_ZeroMemoryOfNameTable )
{
	TestLocateTable();TestInsertRecord();
	TestLocateTable();TestSelectRecord();
}

///< ���������ַ����ṹ����
TEST_F( TestCreateNameTable1000_Normal, InsertUpdateTestOfNormalNameTable )
{
	TestLocateTable();TestInsertRecord();
	TestLocateTable();TestSelectRecord();
	TestUpdateRecord();TestSelectRecord();
}

///< ����һ�����ݿ����
TEST_F( TestAnyMessage_ID_X, CreateBundleOfDatabasePointer )
{
	for( int n = 0; n < 5; n++ )	{
		ASSERT_NE( GetFactoryObject().GrapDatabaseInterface(), (I_Database*)NULL );
	}
}

///< ��һ���ݿ�������ݱ�Ĵ���ɾ���ظ�����
TEST_F( TestAnyMessage_ID_X, CreateDeleteAllTablesTest )
{
	I_Table*			pTable = NULL;
	unsigned int		nCount = m_vctIMessage.size();

	///< �����������ݱ�
	TestCreateAllTable();
	///< �����������ݱ�
	TestCreateAllTable();
	///< �����������ݱ�
	TestCreateAllTable();
	///< ����������ݿ�
	TestDeleteAllTables();
	///< ���ⶨ��һ�������ڵ����ݱ�
	::printf( "query table in empty database.........................................\n" );
	for( unsigned int i = 0; i < nCount; i++ )	{	TestLocateTable( m_vctIMessage[i]->MessageID(), &pTable, false );	}
	::printf( "happy ending..........................................................\n" );
	for( int a = 0; a < 10; a++ ) {
		///< �ٴΣ������������ݱ�
		TestCreateAllTable();
		///< �ٴΣ������������ݱ�
		TestCreateAllTable();
		///< �ٴΣ������������ݱ�
		TestCreateAllTable();
		///< �ٴ�,������ݿ�
		TestDeleteAllTables();
		///< �ٴ�,������ݿ�
		TestDeleteAllTables();
		///< �ٴ�,������ݿ�
		TestDeleteAllTables();
		///< �ٴ�,������ݿ�
		TestDeleteAllTables();
		///< �ٴ�,������ݿ�
		TestDeleteAllTables();
	}
	///< ���ⶨ��һ�������ڵ����ݱ�
	::printf( "query table in empty database.........................................\n" );
	for( unsigned int ii = 0; ii < nCount; ii++ )	{	TestLocateTable( m_vctIMessage[ii]->MessageID(), &pTable, false );	}
	::printf( "happy ending..........................................................\n" );
}

///< ��һ���ݿ����в�����²���(�ɹ�״̬)
TEST_F( TestAnyMessage_ID_X, InsertUpdateTest )
{
	I_Table*			pTable = NULL;
	unsigned int		nCount = m_vctIMessage.size();

	///< �����������ݱ�
	TestCreateAllTable();
	///< �ٴδ����������ݱ�
	TestCreateAllTable();
	///< �ظ������������ݱ�
	TestCreateAllTable();
	///< �ظ������������ݱ�
	TestCreateAllTable();
	///< �ظ������������ݱ�
	TestCreateAllTable();
	///< ���ⶨ��һ�����ڵ����ݱ�
	for( unsigned int j = 0; j < nCount; j++ )	{
		TestLocateTable( m_vctIMessage[j]->MessageID(), &pTable, true );
	}
	///< �����ݱ��¼����ѭ��
	for( unsigned int n = 0; n < nCount; n++ )	{
		TestInsert1Table( m_vctIMessage[n], true, 1 );
	}
	///< �����ݱ��¼����ѭ��
	for( unsigned int m = 0; m < nCount; m++ )	{
		TestUpdate1Table( m_vctIMessage[m], true, 1 );
	}
}

///< �Կ����ݱ�����¼�ո��²�������
TEST_F( TestAnyMessage_ID_X, UpdateBeforeInsert )
{
	I_Table*			pTable = NULL;
	unsigned int		nCount = m_vctIMessage.size();

	///< ����������ݿ�
	TestDeleteAllTables();
	///< �����������ݱ�
	TestCreateAllTable();
	///< ���ⶨ��һ�����ڵ����ݱ�
	for( unsigned int j = 0; j < nCount; j++ )	{
		TestLocateTable( m_vctIMessage[j]->MessageID(), &pTable, true );
	}

	///< �����ݱ��¼����ѭ��
	for( unsigned int m = 0; m < nCount; m++ )	{
		TestUpdate1Table( m_vctIMessage[m], true, 0 );
	}
}


///< ---------------------- ��Ԫ���Գ�ʼ���ඨ�� -------------------------


I_Database* UnitTestEnv::m_pIDatabase = NULL;

I_Database* UnitTestEnv::GetDatabasePtr()
{
	return m_pIDatabase;
}

void UnitTestEnv::SetUp()
{
	///< ����һ�����ݿ�
	m_pIDatabase = GetFactoryObject().GrapDatabaseInterface();
	ASSERT_NE( m_pIDatabase, (I_Database*)NULL );
}

void UnitTestEnv::TearDown()
{
	///< �ͷ�һ�����ݿ���Դ
	for( int n = 0; n < 10; n++ )	{
		ASSERT_EQ( GetFactoryObject().ReleaseAllDatabase(), true );
	}
}









///< ---------------- ��Ԫ���Ե����������� -------------------------------


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











