#ifndef __QLX_UNIT_TEST_H__
#define __QLX_UNIT_TEST_H__
#pragma warning(disable : 4996)
#pragma warning(disable : 4204)


#include <vector>
#include "gtest/gtest.h"
#include "../Interface.h"
#include "../MemTable/MemTable.h"


///< --------------------- ��Ԫ�����ඨ�� --------------------------------


/**
 * @class							TestTableOperation
 * @brief							�������ݱ����ҵ��
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

		char				SecurityID[32];				///< ��Ʒ����
		char				SecurityName[64];			///< ��Ʒ����
		unsigned int		SecurityType;				///< ��Ʒ����
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

		char				SecurityID[32];				///< ��Ʒ����
		unsigned int		High;
		unsigned int		Low;
		unsigned int		Now;
		double				Amount;						///< ��Ʒ�ɽ����
		unsigned __int64	Volume;						///< ��Ʒ�ܳɽ���
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
	void												SetUp();				///< Ԥ�潨��������Ҫ�����ݱ�
	void												TearDown();
	void												TestCreateAllTable();
	void												TestDeleteAllTables();
	void												TestDeleteOneTable( unsigned int nTableID );

protected:
	I_Table*											m_pCurTablePtr;			///< �г���Ϣ�б�
	int													m_nMaxLoopNum;			///< ������ݶ��г���
	static std::vector<T_Message_MarketInfo>			m_vctMarketInfo;		///< �г���Ϣ�б�
	static std::vector<T_Message_NameTable>				m_vctNameTable;			///< �г�����б�
	static std::vector<T_Message_SnapTable>				m_vctSnapTable;			///< �г������б�
};


///< ------------ ��Ԫ���Գ�ʼ���ඨ�� ------------------------------------


/**
 * @class					QLXEnDeCodeTestEnv
 * @brief					ȫ���¼�(��ʼ������)
 * @author					barry
 */
class UnitTestEnv : public testing::Environment
{
public:
	/**
	 * @brief				ȡ�����ݿ����ָ��,�Ӷ�����ָ������
	 */
	static	I_Database*		GetDatabasePtr();
	void					SetUp();
	void					TearDown();

private:
	static I_Database*		m_pIDatabase;		///< ����ѹ�����������ݶ����ַ
};


/**
 * @brief					�������еĲ�������
 * @author					barry
 */
void ExecuteTestCase();



#endif





