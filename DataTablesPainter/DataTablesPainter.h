#ifndef __DATA_TABLE_PAINTER_H__
#define	__DATA_TABLE_PAINTER_H__


#include "Interface.h"


/**
 * @brief				DLL�����ӿ�
 * @author				barry
 * @date				2017/4/1
 */
extern "C"
{
	/**
	 * @brief								ȡ�ù�����������,���ڴ���/����/�ջ��ڴ����ݿ����
	 * @return								���ع�����������
	 */
	__declspec(dllexport) DBFactory&		GetFactoryObject();

	/**
	 * @brief								��Ԫ���Ե�������
	 */
	__declspec(dllexport) void				ExecuteUnitTest();
}




#endif





