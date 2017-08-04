#ifndef __MEM_TABLE__MEM_DATABASE_H__
#define	__MEM_TABLE__MEM_DATABASE_H__


#include <set>
#include <vector>
#include "MemTable.h"
#include "../Interface.h"


namespace MemoryCollection
{


const unsigned int			MAX_TABBLE_NO = 64;				///< 最多可分配的数据表的数量


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
	 * @return					true				配置成功
	 */
	bool						CreateTable( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen );

	/**
	 * @brief					删除指定的数据表
	 * @param[in]				nBindID				数据表ID
	 * @return					true				删除成功
	 */
	bool						DeleteTable( unsigned int nBindID );

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
	 * @return					>0					数据库加载落盘文件日期
								<0					出错
	 */
	int							LoadFromDisk( const char* pszDataFile );

	/**
	 * @brief					将所有数据存盘
	 */
	bool						SaveToDisk( const char* pszDataFile );

	/**
	 * @brief					取得数据更新自增流水号
	 */
	unsigned __int64			GetUpdateSequence();

	/**
	 * @brief					取得数据表的数量
	 * @return					返回统计值
	 */
	unsigned int				GetTableCount();

	/**
	 * @brief					根据位置索引取得数据表元信息
	 * @param[in]				想获取数据表的位置
	 * @param[out]				nDataID				数据表ID
	 * @param[out]				nRecordLen			数据表记录长度
	 * @param[out]				nKeyStrLen			数据主键最大长度
	 * @return					true				获取成功
								false				获取出错
	 */
	bool						GetTableMetaByPos( unsigned int nPos, unsigned int& nDataID, unsigned int& nRecordLen, unsigned int& nKeyStrLen );

public:
	/**
	 * @brief					根据顺序索引值，取得数据表引用
	 * @return					NULL			无效的索引值会返回null
	 */
	I_Table*					operator[]( unsigned int nTableIndex );

private:
	CriticalObject				m_oCSLock;									///< 内存表锁
	CollisionHash				m_HashTableOfPostion;						///< 哈稀表,msgid所在的数据选择类型
	DynamicTable				m_arrayQuotationTables[MAX_TABBLE_NO];		///< 行情动态表集合
	unsigned int				m_nUsedTableNum;							///< 已经使用的数据表数组的数量
	std::set<unsigned int>		m_setTableID;								///< 数据表ID集合
private:
	char*						m_pQueryBuffer;								///< 数据查询缓存
};




}


#endif






