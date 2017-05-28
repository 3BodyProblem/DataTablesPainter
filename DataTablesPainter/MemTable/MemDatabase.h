#ifndef __MEM_TABLE__MEM_DATABASE_H__
#define	__MEM_TABLE__MEM_DATABASE_H__


#include <vector>
#include "MemTable.h"
#include "../Interface.h"


namespace MemoryCollection
{


const unsigned int						MAX_TABBLE_NO = 128*2;				///< 最多可分配的数据表的数量


/**
 * @class			MemDatabase
 * @brief			内存数据库(内存表维护存取池)
 * @detail			记录每个BindID(messageid)对应数据结构的长度和分配的合适的数据表位置
					&&
					并且跟据以上记录进行选择+存取
 * @author			barry
 * @date			2017/4/2
 */
class MemDatabase : public I_Database
{
public:
	MemDatabase();
	~MemDatabase();

public:///< Method Of Interface
	/**
	 * @brief					根据消息id和消息长度，进行合适的数据表配置（在预备表中配置对应的占用关系）
	 * @param[in]				nBindID				数据类形标识号
	 * @param[in]				nRecordWidth		数据长度
	 * @param[in]				nKeyStrLen			主键长度
	 * @return					=0					配置成功
								>0					忽略（成功）
								<0					配置出错
	 */
	bool						CreateTable( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen );

	/**
	 * @brief					根据MessageID取得已经存在的或者分配一个新的内存表的引用
	 * @detail					本函数对每个messageid维护一个唯一且对应的内存表，根据nBindID值返回已经存在的，或新建后返回
	 * @param[in]				nBindID				MessageID
	 * @return					返回已经存在的内存表或新建的内存表
	 */
	I_Table*					QueryTable( unsigned int nBindID );

	/**
	 * @brief					清理所有数据表
	 * @return					==0					成功
								!=					出错
	 */
	int							DeleteTables();

	/**
	 * @brief					从硬盘恢复所有数据
	 */
	bool						LoadFromDisk( const char* pszDataFile );

	/**
	 * @brief					将所有数据存盘
	 */
	bool						SaveToDisk( const char* pszDataFile );

	/**
	 * @brief					取得数据更新自增流水号
	 */
	unsigned __int64			GetUpdateSequence();

public:
	/**
	 * @brief					根据顺序索引值，取得数据表引用
	 * @return					NULL			无效的索引值会返回null
	 */
	I_Table*					operator[]( unsigned int nTableIndex );

	/**
	 * @brief					取得数据表的数量
	 * @return					返回统计值
	 */
	unsigned int				GetTableCount();

private:
	CriticalObject				m_oCSLock;									///< 内存表锁
	CollisionHash				m_HashTableOfPostion;						///< 哈稀表,msgid所在的数据选择类型
	DynamicTable				m_arrayQuotationTables[MAX_TABBLE_NO];		///< 行情动态表集合
	unsigned int				m_nUsedTableNum;							///< 已经使用珠数据表数量
private:
	char*						m_pQueryBuffer;								///< 数据查询缓存
};




}


#endif






