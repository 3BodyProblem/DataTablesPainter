#ifndef __MEM_TABLE__MEM_RECORD_H__
#define	__MEM_TABLE__MEM_RECORD_H__


#include "../Interface.h"


#pragma pack(1)
namespace MemoryCollection
{


/**
 * @class				CodeKey
 * @brief				主键类
 */
class CodeKey
{
public:
	/**
	 * @bref						构造主键类
	 */
	CodeKey( char* pszCode );

	/**
	 * @brief						取得数据体在数据表中的索引位置值
	 */
	__int64							GetKeyID() const;

	/**
	 * @brief						将商品代码hash成数值并返回
	 * @param[in]					pszCode				商品代码（长度20）
	 * @param[in]					nCodeLen			最长代码长度
	 * @return						商品代码的hash值
	 */
	static __int64					GenHashKey( const char* pszCode, unsigned int nCodeLen );

private:
	mutable __int64					m_nKeyID;					///< 主键值
	char*							m_pszCode;					///< 数据结构地址
};


/**
 * @class				DyncRecord
 * @brief				带主键的数据体封装
 */
class DyncRecord : public RecordBlock
{
public:
	/**
	 * @brief						构造记录结构封装
	 * @param[in]					pRecord							数据地址
	 * @param[in]					nRecordLen						数据长度
	 */
	DyncRecord( char* pRecord, unsigned int nRecordLen );

	/**
	 * @brief						copy构造
	 */
	DyncRecord( const DyncRecord& obj );

	/**
	 * @brief						取得索引号
	 */
	__int64							GetMainKey() const;

private:
	CodeKey							m_oCodeKey;						///< 主键对象
};


}
#pragma pack()



#endif





