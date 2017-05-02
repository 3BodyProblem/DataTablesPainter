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
 * @class							TestCreateNameTable1000_ZeroMemo
 * @brief							���Դ������ݱ�Ϳռ�¼(CodeΪ�գ��������Զ�Ϊ��)	(ID=1000)
 * @author							barry
 */
class TestCreateNameTable1000_ZeroMemo : public testing::Test
{
public:
	typedef struct
	{
		char				SecurityID[32];				///< ��Ʒ����
		char				SecurityName[64];			///< ��Ʒ����
		unsigned int		SecurityType;				///< ��Ʒ����
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
	static I_Table*										s_pTablePtr;			///< ���ݱ�ָ��
	static MemoryCollection::DynamicTable::TableMeta	s_oTableMeta;			///< ���ݱ�Ԫ��Ϣ
	static T_Message1000_NameTable						s_sMsg1000NameTable;	///< ���ṹ
};


/**
 * @class							TestCreateNameTable1000_Normal
 * @brief							���Դ������ݱ��һ�������ݵļ�¼	(ID=1000)
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
 * @brief							��Ϣ��ȡ�ӿ�
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
 * @brief							���������ݽӿ�,���Ը��ֱ仯�Ĵ���ֵ
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

		char				SecurityID[64];				///< ��Ʒ����
		char				SecurityName[128];			///< ��Ʒ����
		unsigned int		SecurityType;				///< ��Ʒ����
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
 * @brief							���ձ�������ݽӿ�,���Ը��ֱ仯����ֵ
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

		char				SecurityID[64];				///< ��Ʒ����
		unsigned int		High;
		unsigned int		Low;
		unsigned int		Now;
		double				Amount;						///< ��Ʒ�ɽ����
		unsigned __int64	Volume;						///< ��Ʒ�ܳɽ���
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
 * @brief							��ͬ���ݱ�MessageID�������ݽӿ�,���Ը��ֱ仯��MessageID��ֵ
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

		char				SecurityID[64];				///< ��Ʒ����
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
 * @brief							�����������͵���Ϣ�ṹ
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





