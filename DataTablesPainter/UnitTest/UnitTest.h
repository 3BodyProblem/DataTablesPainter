#ifndef __QLX_UNIT_TEST_H__
#define __QLX_UNIT_TEST_H__
#pragma warning(disable : 4996)
#pragma warning(disable : 4204)


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





