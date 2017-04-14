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
	 * @brief								构造函数
	 * @param[in]							nRecordWidth			记录长度设定
	 * @param[in]							nKeyStrLen				记录数据块，头nKeyStrLen位，为本数据表的主键字符串
	 */
	VariableRecordTable( unsigned int nRecordWidth, unsigned int nKeyStrLen = 20 );
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
	unsigned int							m_nMainStrKeyLen;		///< 索引字符主键缓存长度
	unsigned int							m_nRecordWidth;			///< 记录长度值
	unsigned int							m_nMaxBufferSize;		///< 记录集缓存最大长度
private:
	CriticalObject							m_oCSLock;				///< 内存表锁
	char*									m_pRecordsBuffer;		///< 记录集缓存
	unsigned int							m_nCurrentDataSize;		///< 当前有效数据的长度
};


///< -----------------------------------------------------


template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM = 32>
class MemTableCollection
{
public:
	MemTableCollection();

	/**
	 * @brief				增加占用表计数
	 */
	__inline void			IncreaseTableNum();

	/**
	 * @brief				创建的数据表数量
	 */
	unsigned int			Size();

	/**
	 * @brief				根据下标取得数据表
	 * @param[in]			nTablePos			数据表下标
	 */
	TYPE_MemTable&			operator[]( unsigned int nTablePos );

private:
	unsigned int			m_nUsedTableNumber;						///< 已经使用的数据表数量
	TYPE_MemTable			m_ArrayOfMemTable[MAX_TABLE_NUM];		///< 数据表预备列表
};

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::MemTableCollection()
 : m_nUsedTableNumber( 0 )
{
}

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
void MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::IncreaseTableNum()
{
	++m_nUsedTableNumber;
}

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
TYPE_MemTable& MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::operator[]( unsigned int nTablePos )
{
	return m_ArrayOfMemTable[nTablePos];
}

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
unsigned int MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::Size()
{
	return m_nUsedTableNumber;
}



}



#endif





