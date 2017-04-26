#ifndef __QLX_UNIT_TEST_H__
#define __QLX_UNIT_TEST_H__
#pragma warning(disable : 4996)
#pragma warning(disable : 4204)


#include "gtest/gtest.h"


///< --------------------- DEMO部分的测试类： 单元测试类定义 --------------------------------


/**
 * @class		TestDemoStatic
 * @brief		简单测试样例静态数据类的加解码
				&
				测试单个或多个加解码
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


///< ------------ 单元测试初始化类定义 ------------------------------------


/**
 * @class		QLXEnDeCodeTestEnv
 * @brief		全局事件(初始化引擎)
 */
class QLXEnDeCodeTestEnv : public testing::Environment
{
public:
	void	SetUp();
	void	TearDown();
};


/**
 * @brief		运行所有的测试例程
 */
void ExecuteTestCase();



#endif





