#include "UnitTest.h"
#include <string>
#include <iostream>
#include <algorithm>


void TestDemoStatic::SetUpTestCase()
{
/*	for( int n = 0; n < MAX_STATIC_NUM; n++ )
	{
		char		pszCode[7] = { 0 };

		m_lstTagName[n].Type = (n % 10) + 1;
		::sprintf( pszCode, "%u", 600001 + n );
		::memcpy( m_lstTagName[n].Code, pszCode, 6 );
		::strncpy( m_lstTagName[n].Name, "abcdefg", 8 );
	}*/
}

void TestDemoStatic::TearDownTestCase()
{
//	::memset( m_lstTagName, 0, sizeof(m_lstTagName) );
}

void TestDemoStatic::SetUp()
{
//	ASSERT_EQ( 0, g_pEncoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
//	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
}

void TestDemoStatic::TearDown()
{
}


///< ------------------------ ������������ ----------------------------------------------------


TEST_F( TestDemoStatic, Single )
{
/*	unsigned short			nMsgID = 0;
	tagDemoStaticType		value = { 0 };
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[0], sizeof(tagDemoStaticType) ) );
	g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[0], sizeof(value) ) );*/
}

TEST_F( TestDemoStatic, Loop )
{
/*	unsigned short			nMsgID = 0;
	tagDemoStaticType		value = { 0 };
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[0], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[1], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[2], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[2], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[1], sizeof(tagDemoStaticType) ) );
	int	nBufSize = g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[0], sizeof(tagDemoStaticType) );
	ASSERT_LT( 0, nBufSize );

	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, nBufSize ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[0], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[1], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[2], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[2], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[1], sizeof(value) ) );
	ASSERT_EQ( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[0], sizeof(value) ) );*/
}


///< ------------------------- ��Ԫ���Գ�ʼ���ඨ�� ----------------------------------------------


void QLXEnDeCodeTestEnv::SetUp()
{
	ASSERT_EQ( true, true );
/*	IPbOperation* pListEncodeOperation[] = { &EnMarket, &EnStatic, &EnSnap, &EnTrade, &EnIndex, &EnVP };///< ��Ҫע���protobuf��encode���Զ���
	IPbOperation* pListDecodeOperation[] = { &DeMarket, &DeStatic, &DeSnap, &DeTrade, &DeIndex, &DeVP };///< ��Ҫע���protobuf��encode���Զ���

	g_pGlobalBuffer = new char[GLOBAL_BUFFER_SIZE];							///< ����һ��encode��decode���õĴ󻺴�,�������δ�Ŷ��Messages
	ASSERT_NE( 0, (int)g_pGlobalBuffer );									///< ��⻺���Ƿ����ɹ�
	g_pEncoder = new Encode();
	ASSERT_NE( 0, (int)g_pEncoder );
	g_pDecoder = new Decode();
	ASSERT_NE( 0, (int)g_pDecoder );
	ASSERT_EQ( 0, g_pEncoder->Initialize( s_XmlTemplatePath.c_str(), pListEncodeOperation, sizeof(pListEncodeOperation)/sizeof(IPbOperation*) ) );
	ASSERT_EQ( 0, g_pDecoder->Initialize( s_XmlTemplatePath.c_str(), pListDecodeOperation, sizeof(pListDecodeOperation)/sizeof(IPbOperation*) ) );*/
}

void QLXEnDeCodeTestEnv::TearDown()
{
/*	delete [] g_pGlobalBuffer;
	g_pGlobalBuffer = NULL;

	delete g_pEncoder;
	delete g_pDecoder;
	g_pEncoder = NULL;
	g_pDecoder = NULL;*/
}


///< ------------------ ��Ԫ���Ե����������� -------------------------------


/**
 * @brief		��ʼ��gtest��������
 */
void ExecuteTestCase()
{
	static	bool	s_bInit = false;

	if( false == s_bInit )	{
		int			nArgc = 1;
		char*		pszArgv[32] = { "MemDatabase.dll", };

		s_bInit = true;
		testing::AddGlobalTestEnvironment( new QLXEnDeCodeTestEnv() );
		testing::InitGoogleTest( &nArgc, pszArgv );
		RUN_ALL_TESTS();
	}
}











