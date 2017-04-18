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
 * @brief									变长记录表
 * @detail									数据表中记录的长度可以在构造时具体指定
 * @note									本表只支持结构头部为string类型的定长主键的数据结构体
 */
class VariableRecordTable
{
public:
	/**
	 * @class				TableMeta
	 * @brief				数据表元信息
	 */
	class TableMeta
	{
	public:
		TableMeta( unsigned int nRecordWidth, unsigned int nKeyStrLen = 20 );
		void			Clear();
	public:
		unsigned int	m_nRecordWidth;		///< 记录长度设定
		unsigned int	m_nKeyStrLen;		///< 记录数据块，头nKeyStrLen位，为本数据表的主键字符串
	};
public:
	VariableRecordTable();
	~VariableRecordTable();

	/**
	 * @brief								追加新数据
	 * @param[in]							refRecord				追加的数据
	 * @return								==0						增加成功
											!=0						失败
	 */
	int										PushBack( const RecordWithKey& refRecord ); 

	/**
	 * @brief								索引出记录对象
	 * @param[in]							Index					记录索引
	 * @return								返回记录对象
	 */
	RecordWithKey							operator[]( int nIndex );

private:
	/**
	 * @brief								扩大现有内存缓存的分配
	 * @param[in]							nAllocItemNum			需再分配的元素数量
	 * @return								true					分配成功
	 */
	bool									EnlargeBuffer( unsigned long nAllocItemNum = 1 );

private:
	TableMeta								m_oTableMeta;			///< 数据表元信息
	CriticalObject							m_oCSLock;				///< 内存表锁
private:
	char*									m_pRecordsBuffer;		///< 记录集缓存
	unsigned int							m_nMaxBufferSize;		///< 记录集缓存最大长度
	unsigned int							m_nCurrentDataSize;		///< 当前有效数据的长度
};



}



#endif









