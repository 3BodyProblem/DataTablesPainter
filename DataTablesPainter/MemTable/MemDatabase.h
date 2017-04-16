#ifndef __MEM_TABLE__MEM_DATABASE_H__
#define	__MEM_TABLE__MEM_DATABASE_H__


#include "MemTable.h"


namespace MemoryCollection
{


typedef MemTableCollection<MemTable<ST_Record_64>>	TYPE_64BytesTables;			///< ��¼����64Bytes�����ݱ�
typedef MemTableCollection<MemTable<ST_Record_128>>	TYPE_128BytesTables;		///< ��¼����128Bytes�����ݱ�
typedef MemTableCollection<MemTable<ST_Record_256>>	TYPE_256BytesTables;		///< ��¼����256Bytes�����ݱ�
typedef MemTableCollection<MemTable<ST_Record_512>>	TYPE_512BytesTables;		///< ��¼����512Bytes�����ݱ�


/**
 * @class			T_TABLE_POS_INF
 * @brief			���ݱ�λ����Ϣ�ṹ
 * @author			barry
 * @date			2017/4/2
 */
struct T_TABLE_POS_INF
{
	T_TABLE_POS_INF() { nDataPosition = 0; eRecordType = E_RECORD_NONE; }
	T_TABLE_POS_INF( ENUM_RECORD_TYPE eType, unsigned int nTablePos, unsigned int nDataPos ) { eRecordType = eType; nTablePosition = nTablePos; nDataPosition = nDataPos; }
	ENUM_RECORD_TYPE		eRecordType;			///< ��¼��������
	unsigned int			nTablePosition;			///< ʹ�����ݱ������λ��
	unsigned int			nDataPosition;			///< ��¼�����ݱ��е�λ��
};


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
	IMemoryTableOperator*		QueryTableByMsgID( unsigned int nMsgID );

private:
	TPostionHash				m_HashTableOfPostion;					///< ��ϡ��,msgid���ڵ�����ѡ������
	TYPE_64BytesTables			m_ArrayOfTable64Bytes;					///< Ԥ�������ݱ�
	TYPE_128BytesTables			m_ArrayOfTable128Bytes;					///< Ԥ�������ݱ�
	TYPE_256BytesTables			m_ArrayOfTable256Bytes;					///< Ԥ�������ݱ�
	TYPE_512BytesTables			m_ArrayOfTable512Bytes;					///< Ԥ�������ݱ�
};




}


#endif






