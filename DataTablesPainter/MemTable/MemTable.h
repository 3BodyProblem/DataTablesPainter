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


/**
 * @class			T_RECORD_POS
 * @brief			���ݼ�¼λ����Ϣ�ṹ
 * @author			barry
 * @date			2017/4/2
 */
struct T_RECORD_POS
{
	T_RECORD_POS() { nRecordPos = -1; }
	T_RECORD_POS( unsigned int nPos ) { nRecordPos = nPos; }
	void			Clear()	{	nRecordPos = -1;	}
	bool			Empty() const	{	if( nRecordPos < 0 )	return true;	else	return false;	}
	int				nRecordPos;			///< ʹ�����ݱ������λ��
};


typedef CollisionHash<unsigned __int64,T_RECORD_POS>	TRecordHash;	///< ��ϣ��(key,index of array)


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
	 * @param[in]							pRecord					��¼���ַ
	 * @param[in]							nRecordLen				��¼�峤��
	 * @return								>0						���ӳɹ�
											=0						��¼�Ѿ����ڣ�����Ҫ����
											!=0						ʧ��
	 */
	int										InsertRecord( char* pRecord, unsigned int nRecordLen ); 

	/**
	 * @brief								��������
	 * @param[in]							pRecord					��¼���ַ
	 * @param[in]							nRecordLen				��¼�峤��
	 * @return								>0						���ӳɹ�
											=0						��¼�Ѿ����ڣ�����Ҫ����
											<0						ʧ��
	 */
	virtual int								UpdateRecord( char* pRecord, unsigned int nRecordLen ); 

	/**
	 * @brief								��������¼����
	 * @param[in]							pKeyStr					������ַ
	 * @param[in]							nKeyLen					��������
	 * @return								���ؼ�¼����
	 */
	RecordBlock								SelectRecord( char* pKeyStr, unsigned int nKeyLen );

	/**
	 * @brief								�����ݱ������ԭ��copy������
	 * @param[in]							pBuffer					�����ַ
	 * @param[in]							nBufferSize				���泤��
	 * @return								>=0						�������ݳ���
											<						����
	 */
	int										CopyToBuffer( char* pBuffer, unsigned int nBufferSize );

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









