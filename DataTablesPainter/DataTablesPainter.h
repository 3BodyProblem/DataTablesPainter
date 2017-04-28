#ifndef __DATA_TABLE_PAINTER_H__
#define	__DATA_TABLE_PAINTER_H__


#include "Interface.h"
#include "Infrastructure/Lock.h"


/**
 * @class							DBFactory
 * @brief							�ڴ����ݷ����������
 */
class DBFactory : public IDBFactory
{
private:
	DBFactory();

public:
	/**
	 * @brief						ȡ�õ���
	 */
	static	IDBFactory&				GetFactory();

public:
	/**
	 * @brief						�������������ݿ����ָ��
	 * @return						�������ݿ�ָ��ĵ�ַ
	 */
	I_Database*						GrapDatabaseInterface();

	/**
	 * @brief						�ͷŷ�����������ݿ����
	 */
	bool							ReleaseAllDatabase();

};



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
	__declspec(dllexport) IDBFactory&		GetFactoryObject();

	/**
	 * @brief								��Ԫ���Ե�������
	 */
	__declspec(dllexport) void				ExecuteUnitTest();
}




#endif





