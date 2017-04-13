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

	/**
	 * @brief								׷��������
	 * @param[in]							pszData					׷�ӵ����ݵ�ַ
	 * @param[in]							nDataLen				�����峤��
	 * @return								==0						���ӳɹ�
											!=0						ʧ��
	 */
	int										PushBack( const char* pszData, unsigned int nDataLen ); 

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


/**
 * @class			IMemoryTableOperator
 * @brief			�ڴ������ӿ�
 * @author			barry
 * @date			2017/4/1
 */
class IMemoryTableOperator
{
public:
	/**
	 * @brief								��ǰ���ݱ���
	 */
	virtual unsigned int					Size() = 0;

	/**
	 * @brief								������¼
	 * @param[in]							pszData				��¼���ַ
	 * @param[in]							nDataLen			��¼�峤��
	 * @return								true				�����ɹ�
											false				����ʧ��
	 */
	virtual bool							NewRecord( char* pszData, unsigned int nDataLen ) = 0;

	/**
	 * @brief								���¼�¼
	 * @param[in]							pszData				��¼���ַ
	 * @param[in]							nDataLen			��¼�峤��
	 * @return								true				���³ɹ�
											false				����ʧ��
	 */
	virtual bool							UpdateRecord( char* pszData, unsigned int nDataLen ) = 0;
};

/**
 * @class			MemTable<��¼��������>
 * @brief			�ڴ����ݱ�ģ��
 * @author			barry
 * @date			2017/4/1
 */
template<class TYPE_RECORD>
class MemTable : public IMemoryTableOperator
{
public:
	MemTable();

	/**
	 * @brief								��ǰ���ݱ���
	 */
	unsigned int							Size();

	/**
	 * @brief								������¼
	 * @param[in]							pszData				��¼���ַ
	 * @param[in]							nDataLen			��¼�峤��
	 * @return								true				�����ɹ�
											false				����ʧ��
	 */
	bool									NewRecord( char* pszData, unsigned int nDataLen );

	/**
	 * @brief								���¼�¼
	 * @param[in]							pszData				��¼���ַ
	 * @param[in]							nDataLen			��¼�峤��
	 * @return								true				���³ɹ�
											false				����ʧ��
	 */
	bool									UpdateRecord( char* pszData, unsigned int nDataLen );

private:
	CriticalObject							m_oCSLock;				///< �ڴ����
	std::vector<TYPE_RECORD>				m_vctTable;				///< �ڴ����ݱ�
};


template<class TYPE_RECORD>
MemTable<TYPE_RECORD>::MemTable()
{
}

template<class TYPE_RECORD>
unsigned int MemTable<TYPE_RECORD>::Size()
{
	return m_vctTable.size();
}

template<class TYPE_RECORD>
bool MemTable<TYPE_RECORD>::NewRecord( char* pszData, unsigned int nDataLen )
{
	m_vctTable.push_back( *((TYPE_RECORD*)pszData) );

	return true;
}

template<class TYPE_RECORD>
bool MemTable<TYPE_RECORD>::UpdateRecord( char* pszData, unsigned int nDataLen )
{
/*	unsigned __int64	nKey = GenerateHashKey( (char(&)[20])*pszData );
	TYPE_RECORD&		refRecord = m_vctTable[nKey];

	if( 0 != ::memcmp( &refRecord, pszData, sizeof(TYPE_RECORD) ) )
	{
		m_vctTable[nKey] = *((TYPE_RECORD*)pszData);
	}
*/
	return true;
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





