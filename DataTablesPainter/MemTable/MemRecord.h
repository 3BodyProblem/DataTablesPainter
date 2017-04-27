#ifndef __MEM_TABLE__MEM_RECORD_H__
#define	__MEM_TABLE__MEM_RECORD_H__


#include "../Interface.h"


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
	__int64							GetKeyID() const;

	/**
	 * @brief						����Ʒ����hash����ֵ������
	 * @param[in]					pszCode				��Ʒ���루����20��
	 * @param[in]					nCodeLen			����볤��
	 * @return						��Ʒ�����hashֵ
	 */
	static __int64					GenHashKey( const char* pszCode, unsigned int nCodeLen );

private:
	mutable __int64					m_nKeyID;					///< ����ֵ
	char*							m_pszCode;					///< ���ݽṹ��ַ
};


/**
 * @class				DyncRecord
 * @brief				���������������װ
 */
class DyncRecord : public RecordBlock
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
	 * @brief						ȡ��������
	 */
	__int64							GetMainKey() const;

private:
	CodeKey							m_oCodeKey;						///< ��������
};


}
#pragma pack()



#endif





