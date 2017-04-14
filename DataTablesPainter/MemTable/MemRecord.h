#ifndef __MEM_TABLE__MEM_RECORD_H__
#define	__MEM_TABLE__MEM_RECORD_H__


#pragma pack(1)
namespace MemoryCollection
{

/**
 * @class				RecordWithKey
 * @brief				带主键的数据体封装
 */
class RecordWithKey
{
public:
	/**
	 * @brief						构造记录结构封装
	 * @param[in]					pRecord							数据地址
	 * @param[in]					nRecordLen						数据长度
	 */
	RecordWithKey( char* pRecord, unsigned int nRecordLen );

	/**
	 * @brief						copy构造
	 */
	RecordWithKey( const RecordWithKey& obj );

	/**
	 * @brief						记录对象为无效
	 * @return						true							无效对象
	 */
	bool							IsNone() const;

	/**
	 * @brief						取得数据体在数据表中的索引位置值
	 */
	int								GetSerialInTable() const;

	/**
	 * @brief						记录体deeply copy
	 * @param[in]					refRecord						内存copy的源头
	 * @return						==1								影响的数量
									==0								不需要copy，目标和源头数据完全一样
									<0								出现错误
	 */
	int								CloneFrom( const RecordWithKey& refRecord );

	/**
	 * @brief						数据长度
	 */
	unsigned int					Length() const;

	/**
	 * @brief						将商品代码hash成数值并返回
	 * @param[in]					pszCode				商品代码（长度20）
	 * @param[in]					nCodeLen			最长代码长度
	 * @return						商品代码的hash值
	 */
	static unsigned __int64			GenerateHashKey( const char* pszCode, unsigned int nCodeLen );
private:
	char*							m_pRecordData;					///< 数据结构地址
	unsigned int					m_nRecordLen;					///< 数据结构长度
};

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

}
#pragma pack()



#endif





