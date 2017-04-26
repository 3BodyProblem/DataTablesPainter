#ifndef __MEM_TABLE__MEM_RECORD_H__
#define	__MEM_TABLE__MEM_RECORD_H__


#pragma pack(1)
namespace MemoryCollection
{


/**
 * @class				CodeKey
 * @brief				������
 */
class CodeKey
{
public:
	/**
	 * @bref						����������
	 */
	CodeKey( char* pszCode );

	/**
	 * @brief						ȡ�������������ݱ��е�����λ��ֵ
	 */
	int								GetKeyID() const;

	/**
	 * @brief						����Ʒ����hash����ֵ������
	 * @param[in]					pszCode				��Ʒ���루����20��
	 * @param[in]					nCodeLen			����볤��
	 * @return						��Ʒ�����hashֵ
	 */
	static unsigned __int64			GenHashKey( const char* pszCode, unsigned int nCodeLen );
private:
	int								m_nKeyID;					///< ����ֵ
	char*							m_pszCode;					///< ���ݽṹ��ַ
};


/**
 * @class				DyncRecord
 * @brief				���������������װ
 */
class DyncRecord
{
public:
	/**
	 * @brief						�����¼�ṹ��װ
	 * @param[in]					pRecord							���ݵ�ַ
	 * @param[in]					nRecordLen						���ݳ���
	 */
	DyncRecord( char* pRecord, unsigned int nRecordLen );

	/**
	 * @brief						copy����
	 */
	DyncRecord( const DyncRecord& obj );

	/**
	 * @brief						��¼����Ϊ��Ч
	 * @return						true							��Ч����
	 */
	bool							IsNone() const;

	/**
	 * @brief						��¼��deeply copy
	 * @param[in]					refRecord						�ڴ�copy��Դͷ
	 * @return						==1								Ӱ�������
									==0								����Ҫcopy��Ŀ���Դͷ������ȫһ��
									<0								���ִ���
	 */
	int								CloneFrom( const DyncRecord& refRecord );

	/**
	 * @brief						���ݳ���
	 */
	unsigned int					Length() const;

	/**
	 * @brief						ȡ��������
	 */
	int								GetSerial() const;

private:
	char*							m_pRecordData;					///< ���ݽṹ��ַ
	unsigned int					m_nRecordLen;					///< ���ݽṹ����
	CodeKey							m_oCodeKey;						///< ��������
};


}
#pragma pack()



#endif





