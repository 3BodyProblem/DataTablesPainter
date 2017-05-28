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
 * @class			GlobalSequenceNo
 * @brief			ȫ��������ˮ��������
 * @author			barry
 */
class GlobalSequenceNo
{
public:
	GlobalSequenceNo();

	/**
	 * @brief					ȡ�õ���
	 */
	static GlobalSequenceNo&	GetObj();

public:
	/**
	 * @brief					��������seed
	 */
	void						Reset();

	/**
	 * @brief					����ȫ��������ˮ��
	 */
	unsigned __int64			GenerateSeq();

	/**
	 * @brief					ȡ�õ�ǰ��ˮ��
	 */
	unsigned __int64			GetSeqNo();

protected:
	CriticalObject				m_oLock;				///< �ڴ����
	unsigned int				m_nBaseData;			///< ��ֵ
	unsigned __int64			m_nIncNum;				///< ����ֵ
};


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

	/**
	 * @brief								ȡ�����ݱ�Ԫ��Ϣ
	 */
	TableMeta								GetMeta();

	/**
	 * @brief								��ȡ���ݱ��м�¼����
	 */
	unsigned int							GetRecordCount();

public:
	/**
	 * @brief								׷��������
	 * @param[in]							pRecord					��¼���ַ
	 * @param[in]							nRecordLen				��¼�峤��
	 * @param[out]							nDbSerialNo				���ݿ����������²�����ˮ��
	 * @return								>0						���ӳɹ�
											=0						��¼�Ѿ����ڣ�����Ҫ����
											!=0						ʧ��
	 */
	int										InsertRecord( char* pRecord, unsigned int nRecordLen, unsigned __int64& nDbSerialNo ); 

	/**
	 * @brief								��������
	 * @param[in]							pRecord					��¼���ַ
	 * @param[in]							nRecordLen				��¼�峤��
	 * @param[out]							nDbSerialNo				���ݿ����������²�����ˮ��
	 * @return								>0						���ӳɹ�
											=0						��¼�Ѿ����ڣ�����Ҫ����
											<0						ʧ��
	 */
	int										UpdateRecord( char* pRecord, unsigned int nRecordLen, unsigned __int64& nDbSerialNo ); 

	/**
	 * @brief								��������¼����
	 * @param[in]							pKeyStr					������ַ
	 * @param[in]							nKeyLen					��������
	 * @return								���ؼ�¼����
	 */
	RecordBlock								SelectRecord( char* pKeyStr, unsigned int nKeyLen );

	/**
	 * @brief								ɾ��ĳ����¼
	 * @param[in]							pKeyStr					������ַ
	 * @param[in]							nKeyLen					��������
	 * @param[in,out]						nDbSerialNo				ȡ��>nDbSerialNo������(��Ϊ0,��ȫ��ȡ��) & ���������µ���ˮ��
	 * @return								>0						���ر��������ļ�¼��
											==0						δ�м�¼��ɾ��
											<0						ɾ��ʱ���ִ���
	 */
	int										DeleteRecord( char* pKeyStr, unsigned int nKeyLen, unsigned __int64& nDbSerialNo );

	/**
	 * @brief								�����ݱ������ԭ��copy������
	 * @param[in]							pBuffer					�����ַ
	 * @param[in]							nBufferSize				���泤��
	 * @param[in,out]						nDbSerialNo				ȡ��>nDbSerialNo������(��Ϊ0,��ȫ��ȡ��) & ���������µ���ˮ��
	 * @return								>=0						�������ݳ���
											<						����
	 */
	int										CopyToBuffer( char* pBuffer, unsigned int nBufferSize, unsigned __int64& nDbSerialNo );

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
	CollisionHash							m_oHashTableOfIndex;	///< ����Ʒ����ֵ����ϡ
	char*									m_pRecordsBuffer;		///< ��¼������
	unsigned int							m_nMaxBufferSize;		///< ��¼��������󳤶�
	unsigned int							m_nCurrentDataSize;		///< ��ǰ��Ч���ݵĳ���
};



}



#endif









