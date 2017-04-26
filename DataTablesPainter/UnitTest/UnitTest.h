#ifndef __QLX_UNIT_TEST_H__
#define __QLX_UNIT_TEST_H__
#pragma warning(disable : 4996)
#pragma warning(disable : 4204)


#include "gtest/gtest.h"


///< --------------------- DEMO���ֵĲ����ࣺ ��Ԫ�����ඨ�� --------------------------------


/**
 * @class		TestDemoStatic
 * @brief		�򵥲���������̬������ļӽ���
				&
				���Ե��������ӽ���
 */
class TestDemoStatic : public testing::Test
{
#define		MAX_STATIC_NUM		10
protected:
	static	void	SetUpTestCase();
	static	void	TearDownTestCase();

	void	SetUp();
	void	TearDown();

protected:
//	static	tagDemoStaticType	m_lstTagName[MAX_STATIC_NUM];
};


///< ------------ ��Ԫ���Գ�ʼ���ඨ�� ------------------------------------


/**
 * @class		QLXEnDeCodeTestEnv
 * @brief		ȫ���¼�(��ʼ������)
 */
class QLXEnDeCodeTestEnv : public testing::Environment
{
public:
	void	SetUp();
	void	TearDown();
};


/**
 * @brief		�������еĲ�������
 */
void ExecuteTestCase();



#endif





