#ifndef __MEM_TABLE__MEM_DATABASE_H__
#define	__MEM_TABLE__MEM_DATABASE_H__


#include <vector>
#include "MemTable.h"


namespace MemoryCollection
{


/**
 * @class			T_TABLE_POS_INF
 * @brief			���ݱ�λ����Ϣ�ṹ
 * @author			barry
 * @date			2017/4/2
 */
struct T_TABLE_POS_INF
{
	T_TABLE_POS_INF() { nTablePosition = -1; nDataPosition = -1; }
	T_TABLE_POS_INF( unsigned int nTablePos, unsigned int nDataPos ) { nTablePosition = nTablePos; nDataPosition = nDataPos; }
	int				nTablePosition;			///< ʹ�����ݱ������λ��
	int				nDataPosition;			///< ��¼�����ݱ��е�λ��
};


const unsigned int MAX_TABBLE_NO = 128;										///< ���ɷ�������ݱ������
typedef CollisionHash<unsigned int, struct T_TABLE_POS_INF>	TPostionHash;	///< ��ϣ��


/**
 * @class			MemDatabase
 * @brief			�ڴ����ݿ�(�ڴ��ά����ȡ��)
 * @detail			��¼ÿ��messageid��Ӧ���ݽṹ�ĳ��Ⱥͷ���ĺ��ʵ����ݱ�λ��
					&&
					���Ҹ������ϼ�¼����ѡ��+��ȡ
 * @author			barry
 * @date			2017/4/2
 */
class MemDatabase
{
public:
	MemDatabase( unsigned int nMaxTablesNum = 64 );

	/**
	 * @brief					������������
	 */
	void						Clear();

	/**
	 * @brief					������Ϣid����Ϣ���ȣ����к��ʵ����ݱ����ã���Ԥ���������ö�Ӧ��ռ�ù�ϵ��
	 * @param[in]				nMsgID				Message ID
	 * @param[in]				nMsgSize			Message����
	 * @return					=0					���óɹ�
								>0					���ԣ��ɹ���
								<0					���ó���
	 */
	bool						AllotTable4MsgID( unsigned int nMsgID, unsigned int nMsgSize );

	/**
	 * @brief					����MessageIDȡ���Ѿ����ڵĻ��߷���һ���µ��ڴ�������
	 * @detail					��������ÿ��messageidά��һ��Ψһ�Ҷ�Ӧ���ڴ������msgidֵ�����Ѿ����ڵģ����½��󷵻�
	 * @param[in]				nMsgID				MessageID
	 * @return					�����Ѿ����ڵ��ڴ����½����ڴ��
	 */
	VariableRecordTable*		QueryTableByMsgID( unsigned int nMsgID );

private:
	TPostionHash				m_HashTableOfPostion;						///< ��ϡ��,msgid���ڵ�����ѡ������
	VariableRecordTable			m_arrayQuotationTables[MAX_TABBLE_NO];		///< ���鶯̬����
	unsigned int				m_nUsedTableNum;							///< �Ѿ�ʹ�������ݱ�����
};




}


#endif






