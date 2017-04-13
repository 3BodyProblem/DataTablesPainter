#ifndef __MEM_TABLE__MEM_RECORD_H__
#define	__MEM_TABLE__MEM_RECORD_H__


#pragma pack(1)
namespace MemoryCollection
{
	/**
	 * @class			ENUM_RECORD_TYPE
	 * @brief			���ݼ�¼���ȵ�ö��
	 * @author			barry
	 * @date			2017/4/2
	 */
	typedef enum
	{
		E_RECORD_NONE = 0,
		E_RECORD_64BYTES = 64,
		E_RECORD_128BYTES = 128,
		E_RECORD_256BYTES = 256,
		E_RECORD_512BYTES = 512
	} ENUM_RECORD_TYPE;

	typedef struct
	{
		unsigned int				nMessageID;					///< ��ϢID
		char						pszData[E_RECORD_64BYTES];	///< ��������
	} ST_Record_64;												///< 64�ֽڼ�¼��

	typedef struct
	{
		unsigned int				nMessageID;					///< ��ϢID
		char						pszData[E_RECORD_128BYTES];	///< ��������
	} ST_Record_128;											///< 128�ֽڼ�¼��

	typedef struct
	{
		unsigned int				nMessageID;					///< ��ϢID
		char						pszData[E_RECORD_256BYTES];	///< ��������
	} ST_Record_256;											///< 256�ֽڼ�¼��

	typedef struct
	{
		unsigned int				nMessageID;					///< ��ϢID
		char						pszData[E_RECORD_512BYTES];	///< ��������
	} ST_Record_512;											///< 512�ֽڼ�¼��

	/**
	 * @brief						����Ʒ����hash����ֵ������
	 * @param[in]					pszCode				��Ʒ���루����20��
	 * @param[in]					nMaxCodeLen			����볤��
	 * @return						��Ʒ�����hashֵ
	 */
	unsigned __int64				GenerateHashKey( const char* pszCode, unsigned int nMaxCodeLen );

}
#pragma pack()



#endif





