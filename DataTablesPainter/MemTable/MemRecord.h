#ifndef __MEM_TABLE__MEM_RECORD_H__
#define	__MEM_TABLE__MEM_RECORD_H__


#pragma pack(1)
namespace MemoryCollection
{

/**
 * @class				RecordWithKey
 * @brief				���������������װ
 */
class RecordWithKey
{
public:
	/**
	 * @brief						�����¼�ṹ��װ
	 * @param[in]					pRecord							���ݵ�ַ
	 * @param[in]					nRecordLen						���ݳ���
	 */
	RecordWithKey( char* pRecord, unsigned int nRecordLen );

	/**
	 * @brief						copy����
	 */
	RecordWithKey( const RecordWithKey& obj );

	/**
	 * @brief						��¼����Ϊ��Ч
	 * @return						true							��Ч����
	 */
	bool							IsNone() const;

	/**
	 * @brief						ȡ�������������ݱ��е�����λ��ֵ
	 */
	int								GetSerialInTable() const;

	/**
	 * @brief						��¼��deeply copy
	 * @param[in]					refRecord						�ڴ�copy��Դͷ
	 * @return						==1								Ӱ�������
									==0								����Ҫcopy��Ŀ���Դͷ������ȫһ��
									<0								���ִ���
	 */
	int								CloneFrom( const RecordWithKey& refRecord );

	/**
	 * @brief						���ݳ���
	 */
	unsigned int					Length() const;

	/**
	 * @brief						����Ʒ����hash����ֵ������
	 * @param[in]					pszCode				��Ʒ���루����20��
	 * @param[in]					nCodeLen			����볤��
	 * @return						��Ʒ�����hashֵ
	 */
	static unsigned __int64			GenerateHashKey( const char* pszCode, unsigned int nCodeLen );
private:
	char*							m_pRecordData;					///< ���ݽṹ��ַ
	unsigned int					m_nRecordLen;					///< ���ݽṹ����
};


}
#pragma pack()



#endif





