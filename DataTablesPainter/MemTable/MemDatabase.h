#ifndef __MEM_TABLE__MEM_DATABASE_H__
#define	__MEM_TABLE__MEM_DATABASE_H__


#include "MemTable.h"


namespace MemoryCollection
{


typedef MemTableCollection<MemTable<ST_Record_64>>	TYPE_64BytesTables;			///< 记录长度64Bytes的数据表
typedef MemTableCollection<MemTable<ST_Record_128>>	TYPE_128BytesTables;		///< 记录长度128Bytes的数据表
typedef MemTableCollection<MemTable<ST_Record_256>>	TYPE_256BytesTables;		///< 记录长度256Bytes的数据表
typedef MemTableCollection<MemTable<ST_Record_512>>	TYPE_512BytesTables;		///< 记录长度512Bytes的数据表


/**
 * @class			T_TABLE_POS_INF
 * @brief			数据表位置信息结构
 * @author			barry
 * @date			2017/4/2
 */
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
 * @class			MemDatabase
 * @brief			内存数据库(内存表维护存取池)
 * @detail			记录每个messageid对应数据结构的长度和分配的合适的数据表位置
					&&
					并且跟据以上记录进行选择+存取
 * @author			barry
 * @date			2017/4/2
 */
class MemDatabase
{
public:
	MemDatabase( unsigned int nMaxTablesNum = 64 );

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
	bool						AllotTable4MsgID( unsigned int nMsgID, unsigned int nMsgSize );

	/**
	 * @brief					根据MessageID取得已经存在的或者分配一个新的内存表的引用
	 * @detail					本函数对每个messageid维护一个唯一且对应的内存表，根据msgid值返回已经存在的，或新建后返回
	 * @param[in]				nMsgID				MessageID
	 * @return					返回已经存在的内存表或新建的内存表
	 */
	IMemoryTableOperator*		QueryTableByMsgID( unsigned int nMsgID );

private:
	TPostionHash				m_HashTableOfPostion;					///< 哈稀表,msgid所在的数据选择类型
	TYPE_64BytesTables			m_ArrayOfTable64Bytes;					///< 预备的数据表
	TYPE_128BytesTables			m_ArrayOfTable128Bytes;					///< 预备的数据表
	TYPE_256BytesTables			m_ArrayOfTable256Bytes;					///< 预备的数据表
	TYPE_512BytesTables			m_ArrayOfTable512Bytes;					///< 预备的数据表
};




}


#endif






