#ifndef __MEM_TABLE__MEM_TABLE_H__
#define	__MEM_TABLE__MEM_TABLE_H__


#include <vector>
#include "MemRecord.h"
#include "../Infrastructure/Lock.h"
#include "../Infrastructure/Hash.h"


namespace MemoryCollection
{


/**
 * @class			IMemoryTable
 * @brief			内存表操作接口
 * @author			barry
 * @date			2017/4/1
 */
class IMemoryTable
{
public:

};

/**
 * @class			MemTable<记录长度类型>
 * @brief			内存数据表模板
 * @author			barry
 * @date			2017/4/1
 */
template<class TYPE_RECORD>
class MemTable : public IMemoryTable
{
public:
	MemTable();

	/**
	 * @brief								当前数据表长度
	 */
	unsigned int							GetSize();

private:
	int										m_nLastStructID;		///< 最后或当前所持有的数据结构ID(未使用状态为<0)
	CriticalObject							m_oCSLock;				///< 内存表锁
	std::vector<TYPE_RECORD>				m_vctTable;				///< 内存数据表
};


template<class TYPE_RECORD>
MemTable<TYPE_RECORD>::MemTable()
{
	m_nLastStructID = -1;					///< 标记为未使用
}

template<class TYPE_RECORD>
unsigned int MemTable<TYPE_RECORD>::GetSize()
{
	return m_vctTable.size();
}


///< -----------------------------------------------------


template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM = 32>
class MemTableCollection
{
public:
	MemTableCollection();

	/**
	 * @brief				增加占用表计数
	 */
	void					IncUsedTableNum();

	/**
	 * @brief				取得一个新的数据表的索引值
	 */
	unsigned int			GetUsedTableNumber();

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
void MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::IncUsedTableNum()
{
	++m_nUsedTableNumber;
}

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
TYPE_MemTable& MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::operator[]( unsigned int nTablePos )
{
	return m_ArrayOfMemTable[nTablePos];
}

template<class TYPE_MemTable, const unsigned MAX_TABLE_NUM>
unsigned int MemTableCollection<TYPE_MemTable, MAX_TABLE_NUM>::GetUsedTableNumber()
{
	return m_nUsedTableNumber;
}


typedef MemTableCollection<MemTable<ST_Record_64>>	TYPE_64BytesTables;
typedef MemTableCollection<MemTable<ST_Record_128>>	TYPE_128BytesTables;
typedef MemTableCollection<MemTable<ST_Record_256>>	TYPE_256BytesTables;
typedef MemTableCollection<MemTable<ST_Record_512>>	TYPE_512BytesTables;


///< -----------------------------------------------------


struct T_TABLE_POS_INF
{
	T_TABLE_POS_INF() { nDataPosition = 0; eRecordType = E_RECORD_NONE; }
	T_TABLE_POS_INF( ENUM_RECORD_TYPE eType, unsigned int nTablePos, unsigned int nDataPos ) { eRecordType = eType; nTablePosition = nTablePos; nDataPosition = nDataPos; }
	ENUM_RECORD_TYPE		eRecordType;			///< 记录长度类型
	unsigned int			nTablePosition;			///< 使用数据表的索引位置
	unsigned int			nDataPosition;			///< 记录在数据表中的位置
};


typedef CollisionHash<unsigned int, struct T_TABLE_POS_INF>	TPostionHash;	///< 哈希表


/**
 * @class			MemTablePool
 * @brief			内存表维护存取池
 * @detail			记录每个messageid对应数据结构的长度和分配的合适的数据表位置
					&&
					并且跟据以上记录进行选择+存取
 * @author			barry
 * @date			2017/4/2
 */
class MemTablePool
{
public:
	MemTablePool();

	/**
	 * @brief					清理所有数据
	 */
	void						Clear();

	/**
	 * @brief					根据消息id和消息长度，进行合适的数据表配置（在预备表中配置对应的占用关系）
	 * @param[in]				nMsgID				Message ID
	 * @param[in]				nMsgSize			Message长度
	 * @return					=0					配置成功
								>0					忽略（成功）
								<0					配置出错
	 */
	bool						ConfigTable2Pool( unsigned int nMsgID, unsigned int nMsgSize );

	/**
	 * @brief					根据MessageID取得已经存在的或者分配一个新的内存表的引用
	 * @detail					本函数对每个messageid维护一个唯一且对应的内存表，根据msgid值返回已经存在的，或新建后返回
	 * @param[in]				nMsgID				MessageID
	 * @return					返回已经存在的内存表或新建的内存表
	 */
	IMemoryTable*				operator[]( unsigned int nMsgID );

private:
	TPostionHash				m_HashTableOfPostion;					///< 哈稀表,msgid所在的数据选择类型
	TYPE_64BytesTables			m_ArrayOfTable64Bytes;					///< 预备的数据表
	TYPE_128BytesTables			m_ArrayOfTable128Bytes;					///< 预备的数据表
	TYPE_256BytesTables			m_ArrayOfTable256Bytes;					///< 预备的数据表
	TYPE_512BytesTables			m_ArrayOfTable512Bytes;					///< 预备的数据表
};



}



#endif





