#ifndef __MEM_TABLE__MEM_RECORD_H__
#define	__MEM_TABLE__MEM_RECORD_H__


#pragma pack(1)
namespace MemoryCollection
{
	/**
	 * @class			ENUM_RECORD_TYPE
	 * @brief			数据记录长度的枚举
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
		unsigned int				nMessageID;					///< 消息ID
		char						pszData[E_RECORD_64BYTES];	///< 数据内容
	} ST_Record_64;												///< 64字节记录体

	typedef struct
	{
		unsigned int				nMessageID;					///< 消息ID
		char						pszData[E_RECORD_128BYTES];	///< 数据内容
	} ST_Record_128;											///< 128字节记录体

	typedef struct
	{
		unsigned int				nMessageID;					///< 消息ID
		char						pszData[E_RECORD_256BYTES];	///< 数据内容
	} ST_Record_256;											///< 256字节记录体

	typedef struct
	{
		unsigned int				nMessageID;					///< 消息ID
		char						pszData[E_RECORD_512BYTES];	///< 数据内容
	} ST_Record_512;											///< 512字节记录体

	/**
	 * @brief						将商品代码hash成数值并返回
	 * @param[in]					pszCode				商品代码（长度20）
	 * @param[in]					nMaxCodeLen			最长代码长度
	 * @return						商品代码的hash值
	 */
	unsigned __int64				GenerateHashKey( const char* pszCode, unsigned int nMaxCodeLen );

}
#pragma pack()



#endif





