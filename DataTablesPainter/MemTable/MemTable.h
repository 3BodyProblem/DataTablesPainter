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
	 * @class				TableMeta
	 * @brief				���ݱ�Ԫ��Ϣ
	 */
	class TableMeta
	{
	public:
		TableMeta( unsigned int nRecordWidth, unsigned int nKeyStrLen = 20 );
		void			Clear();
	public:
		unsigned int	m_nRecordWidth;		///< ��¼�����趨
		unsigned int	m_nKeyStrLen;		///< ��¼���ݿ飬ͷnKeyStrLenλ��Ϊ�����ݱ�������ַ���
	};
public:
	VariableRecordTable();
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
	TableMeta								m_oTableMeta;			///< ���ݱ�Ԫ��Ϣ
	CriticalObject							m_oCSLock;				///< �ڴ����
private:
	char*									m_pRecordsBuffer;		///< ��¼������
	unsigned int							m_nMaxBufferSize;		///< ��¼��������󳤶�
	unsigned int							m_nCurrentDataSize;		///< ��ǰ��Ч���ݵĳ���
};



}



#endif









