#ifndef __MEM_TABLE__MEM_TABLE_H__
#define	__MEM_TABLE__MEM_TABLE_H__


#pragma warning(disable:4244)
#include <vector>
#include "MemRecord.h"
#include "../Interface.h"
#include "../Infrastructure/Lock.h"
#include "../Infrastructure/Hash.h"


namespace MemoryCollection
{


typedef CollisionHash<unsigned int,unsigned int>	TRecordHash;	///< ��ϣ��(key,index of array)


/**
 * @class									DynamicTable
 * @brief									�䳤��¼��
 * @detail									���ݱ��м�¼�ĳ��ȿ����ڹ���ʱ����ָ��
 * @note									����ֻ֧�ֽṹͷ��Ϊstring���͵Ķ������������ݽṹ��
 */
class DynamicTable : public I_Table
{
public:
	/**
	 * @class				TableMeta
	 * @brief				���ݱ�Ԫ��Ϣ
	 */
	class TableMeta
	{
	public:
		TableMeta( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen = 20 );
		void			Clear();
		bool			IsEngaged();
	public:
		unsigned int	m_nBindID;			///< ���ⲿ��MessageID
		unsigned int	m_nRecordWidth;		///< ��¼�����趨
		unsigned int	m_nKeyStrLen;		///< ��¼���ݿ飬ͷnKeyStrLenλ��Ϊ�����ݱ�������ַ���
	};
public:
	DynamicTable();
	~DynamicTable();

	/**
	 * @brief								��ʼ�����ݱ�Ԫ��Ϣ
	 * @param[in]							refMeta					���ݱ�Ԫ��Ϣ�ṹ
	 */
	void									Initialize( const TableMeta& refMeta );

	/**
	 * @brief								�ͷ�
	 */
	void									Release();

public:
	/**
	 * @brief								׷��������
	 * @param[in]							refRecord				׷�ӵ�����
	 * @return								==0						���ӳɹ�
											!=0						ʧ��
	 */
	int										InsertRecord( const I_Record& refRecord ); 

	/**
	 * @brief								��������¼����
	 * @param[in]							Index					��¼����
	 * @return								���ؼ�¼����
	 */
	I_Record								SelectRecord( int nRecordIndex );

private:
	/**
	 * @brief								���������ڴ滺��ķ���
	 * @param[in]							nAllocItemNum			���ٷ����Ԫ������
	 * @return								true					����ɹ�
	 */
	bool									EnlargeBuffer( unsigned long nAllocItemNum = 1 );

private:
	TableMeta								m_oTableMeta;			///< ���ݱ�Ԫ��Ϣ
	CriticalObject							m_oCSLock;				///< �ڴ����
private:
	TRecordHash								m_oHashTableOfIndex;	///< ����Ʒ����ֵ����ϡ
	char*									m_pRecordsBuffer;		///< ��¼������
	unsigned int							m_nMaxBufferSize;		///< ��¼��������󳤶�
	unsigned int							m_nCurrentDataSize;		///< ��ǰ��Ч���ݵĳ���
};



}



#endif









