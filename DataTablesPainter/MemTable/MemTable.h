#ifndef __MEM_TABLE__MEM_TABLE_H__
#define	__MEM_TABLE__MEM_TABLE_H__


#pragma warning(disable:4244)
#include <vector>
#include "MemRecord.h"
#include "../Infrastructure/Lock.h"
#include "../Infrastructure/Hash.h"


namespace MemoryCollection
{


/**
 * @class									VariableRecordTable
 * @brief									�䳤��¼��
 * @detail									���ݱ��м�¼�ĳ��ȿ����ڹ���ʱ����ָ��
 * @note									����ֻ֧�ֽṹͷ��Ϊstring���͵Ķ������������ݽṹ��
 */
class VariableRecordTable
{
public:
	/**
	 * @brief								���캯��
	 * @param[in]							nRecordWidth			��¼�����趨
	 * @param[in]							nKeyStrLen				��¼���ݿ飬ͷnKeyStrLenλ��Ϊ�����ݱ�������ַ���
	 */
	VariableRecordTable( unsigned int nRecordWidth, unsigned int nKeyStrLen = 20 );
	~VariableRecordTable();

	/**
	 * @brief								׷��������
	 * @param[in]							refRecord				׷�ӵ�����
	 * @return								==0						���ӳɹ�
											!=0						ʧ��
	 */
	int										PushBack( const RecordWithKey& refRecord ); 

	/**
	 * @brief								��������¼����
	 * @param[in]							Index					��¼����
	 * @return								���ؼ�¼����
	 */
	RecordWithKey							operator[]( int nIndex );

private:
	/**
	 * @brief								���������ڴ滺��ķ���
	 * @param[in]							nAllocItemNum			���ٷ����Ԫ������
	 * @return								true					����ɹ�
	 */
	bool									EnlargeBuffer( unsigned long nAllocItemNum = 1 );

private:
	unsigned int							m_nMainStrKeyLen;		///< �����ַ��������泤��
	unsigned int							m_nRecordWidth;			///< ��¼����ֵ
	unsigned int							m_nMaxBufferSize;		///< ��¼��������󳤶�
private:
	CriticalObject							m_oCSLock;				///< �ڴ����
	char*									m_pRecordsBuffer;		///< ��¼������
	unsigned int							m_nCurrentDataSize;		///< ��ǰ��Ч���ݵĳ���
};


///< -----------------------------------------------------


template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM = 32>
class MemTableCollection
{
public:
	MemTableCollection();

	/**
	 * @brief				����ռ�ñ����
	 */
	__inline void			IncreaseTableNum();

	/**
	 * @brief				���������ݱ�����
	 */
	unsigned int			Size();

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
void MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::IncreaseTableNum()
{
	++m_nUsedTableNumber;
}

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
TYPE_MemTable& MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::operator[]( unsigned int nTablePos )
{
	return m_ArrayOfMemTable[nTablePos];
}

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
unsigned int MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::Size()
{
	return m_nUsedTableNumber;
}



}



#endif





