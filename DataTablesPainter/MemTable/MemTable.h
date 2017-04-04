#ifndef __MEM_TABLE__MEM_TABLE_H__
#define	__MEM_TABLE__MEM_TABLE_H__


#include <vector>
#include "MemRecord.h"
#include "../Infrastructure/Lock.h"
#include "../Infrastructure/Hash.h"


namespace MemoryCollection
{


/**
 * @class			IMemoryTable
 * @brief			�ڴ������ӿ�
 * @author			barry
 * @date			2017/4/1
 */
class IMemoryTable
{
public:

};

/**
 * @class			MemTable<��¼��������>
 * @brief			�ڴ����ݱ�ģ��
 * @author			barry
 * @date			2017/4/1
 */
template<class TYPE_RECORD>
class MemTable : public IMemoryTable
{
public:
	MemTable();

	/**
	 * @brief								��ǰ���ݱ���
	 */
	unsigned int							GetSize();

private:
	int										m_nLastStructID;		///< ����ǰ�����е����ݽṹID(δʹ��״̬Ϊ<0)
	CriticalObject							m_oCSLock;				///< �ڴ����
	std::vector<TYPE_RECORD>				m_vctTable;				///< �ڴ����ݱ�
};


template<class TYPE_RECORD>
MemTable<TYPE_RECORD>::MemTable()
{
	m_nLastStructID = -1;					///< ���Ϊδʹ��
}

template<class TYPE_RECORD>
unsigned int MemTable<TYPE_RECORD>::GetSize()
{
	return m_vctTable.size();
}


///< -----------------------------------------------------


template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM = 32>
class MemTableCollection
{
public:
	MemTableCollection();

	/**
	 * @brief				����ռ�ñ����
	 */
	void					IncUsedTableNum();

	/**
	 * @brief				ȡ��һ���µ����ݱ������ֵ
	 */
	unsigned int			GetUsedTableNumber();

	/**
	 * @brief				�����±�ȡ�����ݱ�
	 * @param[in]			nTablePos			���ݱ��±�
	 */
	TYPE_MemTable&			operator[]( unsigned int nTablePos );

private:
	unsigned int			m_nUsedTableNumber;						///< �Ѿ�ʹ�õ����ݱ�����
	TYPE_MemTable			m_ArrayOfMemTable[MAX_TABLE_NUM];		///< ���ݱ�Ԥ���б�
};

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::MemTableCollection()
 : m_nUsedTableNumber( 0 )
{
}

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
void MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::IncUsedTableNum()
{
	++m_nUsedTableNumber;
}

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
TYPE_MemTable& MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::operator[]( unsigned int nTablePos )
{
	return m_ArrayOfMemTable[nTablePos];
}

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
unsigned int MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::GetUsedTableNumber()
{
	return m_nUsedTableNumber;
}


typedef MemTableCollection<MemTable<ST_Record_64>>	TYPE_64BytesTables;
typedef MemTableCollection<MemTable<ST_Record_128>>	TYPE_128BytesTables;
typedef MemTableCollection<MemTable<ST_Record_256>>	TYPE_256BytesTables;
typedef MemTableCollection<MemTable<ST_Record_512>>	TYPE_512BytesTables;


///< -----------------------------------------------------


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
 * @class			MemTablePool
 * @brief			�ڴ��ά����ȡ��
 * @detail			��¼ÿ��messageid��Ӧ���ݽṹ�ĳ��Ⱥͷ���ĺ��ʵ����ݱ�λ��
					&&
					���Ҹ������ϼ�¼����ѡ��+��ȡ
 * @author			barry
 * @date			2017/4/2
 */
class MemTablePool
{
public:
	MemTablePool();

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
	bool						ConfigTable2Pool( unsigned int nMsgID, unsigned int nMsgSize );

	/**
	 * @brief					����MessageIDȡ���Ѿ����ڵĻ��߷���һ���µ��ڴ�������
	 * @detail					��������ÿ��messageidά��һ��Ψһ�Ҷ�Ӧ���ڴ������msgidֵ�����Ѿ����ڵģ����½��󷵻�
	 * @param[in]				nMsgID				MessageID
	 * @return					�����Ѿ����ڵ��ڴ����½����ڴ��
	 */
	IMemoryTable*				operator[]( unsigned int nMsgID );

private:
	TPostionHash				m_HashTableOfPostion;					///< ��ϡ��,msgid���ڵ�����ѡ������
	TYPE_64BytesTables			m_ArrayOfTable64Bytes;					///< Ԥ�������ݱ�
	TYPE_128BytesTables			m_ArrayOfTable128Bytes;					///< Ԥ�������ݱ�
	TYPE_256BytesTables			m_ArrayOfTable256Bytes;					///< Ԥ�������ݱ�
	TYPE_512BytesTables			m_ArrayOfTable512Bytes;					///< Ԥ�������ݱ�
};



}



#endif





