#ifndef __MEM_TABLE__MEM_TABLE_H__
#define	__MEM_TABLE__MEM_TABLE_H__


#pragma warning(disable:4244)
#include <vector>
#include "MemRecord.h"
#include "../Interface.h"
#include "../Infrastructure/Lock.h"
#include "../Infrastructure/Hash.h"


namespace MemoryCollection
{


/**
 * @class			GlobalSequenceNo
 * @brief			全局自增流水号生产类
 * @author			barry
 */
class GlobalSequenceNo
{
public:
	GlobalSequenceNo();

	/**
	 * @brief					取得单键
	 */
	static GlobalSequenceNo&	GetObj();

public:
	/**
	 * @brief					重置自增seed
	 */
	void						Reset();

	/**
	 * @brief					产生全局自增流水号
	 */
	unsigned __int64			GenerateSeq();

	/**
	 * @brief					取得当前流水号
	 */
	unsigned __int64			GetSeqNo();

protected:
	CriticalObject				m_oLock;				///< 内存表锁
	unsigned int				m_nBaseData;			///< 基值
	unsigned __int64			m_nIncNum;				///< 自增值
};


/**
 * @class									DynamicTable
 * @brief									变长记录表
 * @detail									数据表中记录的长度可以在构造时具体指定
 * @note									本表只支持结构头部为string类型的定长主键的数据结构体
 */
class DynamicTable : public I_Table
{
public:
	/**
	 * @class				TableMeta
	 * @brief				数据表元信息
	 */
	class TableMeta
	{
	public:
		TableMeta( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen = 20 );
		void			Clear();
		bool			IsEngaged();
	public:
		unsigned int	m_nBindID;			///< 即外部的MessageID
		unsigned int	m_nRecordWidth;		///< 记录长度设定
		unsigned int	m_nKeyStrLen;		///< 记录数据块，头nKeyStrLen位，为本数据表的主键字符串
	};
public:
	DynamicTable();
	~DynamicTable();

	/**
	 * @brief								初始化数据表元信息
	 * @param[in]							refMeta					数据表元信息结构
	 */
	void									Initialize( const TableMeta& refMeta );

	/**
	 * @brief								释放
	 */
	void									Release();

	/**
	 * @brief								取得数据表元信息
	 */
	TableMeta								GetMeta();

	/**
	 * @brief								获取数据表中记录个数
	 */
	unsigned int							GetRecordCount();

public:
	/**
	 * @brief								追加新数据
	 * @param[in]							pRecord					记录体地址
	 * @param[in]							nRecordLen				记录体长度
	 * @param[out]							nDbSerialNo				数据库新增，更新操作流水号
	 * @return								>0						增加成功
											=0						记录已经存在，不需要增加
											!=0						失败
	 */
	int										InsertRecord( char* pRecord, unsigned int nRecordLen, unsigned __int64& nDbSerialNo ); 

	/**
	 * @brief								更新数据
	 * @param[in]							pRecord					记录体地址
	 * @param[in]							nRecordLen				记录体长度
	 * @param[out]							nDbSerialNo				数据库新增，更新操作流水号
	 * @return								>0						增加成功
											=0						记录已经存在，不需要增加
											<0						失败
	 */
	int										UpdateRecord( char* pRecord, unsigned int nRecordLen, unsigned __int64& nDbSerialNo ); 

	/**
	 * @brief								索引出记录对象
	 * @param[in]							pKeyStr					主键地址
	 * @param[in]							nKeyLen					主键长度
	 * @return								返回记录对象
	 */
	RecordBlock								SelectRecord( char* pKeyStr, unsigned int nKeyLen );

	/**
	 * @brief								删除某条记录
	 * @param[in]							pKeyStr					主键地址
	 * @param[in]							nKeyLen					主键长度
	 * @param[in,out]						nDbSerialNo				取出>nDbSerialNo的数据(若为0,则全部取出) & 将回填最新的流水号
	 * @return								>0						返回被操作到的记录数
											==0						未有记录被删除
											<0						删除时出现错误
	 */
	int										DeleteRecord( char* pKeyStr, unsigned int nKeyLen, unsigned __int64& nDbSerialNo );

	/**
	 * @brief								将数据表的数据原样copy到缓存
	 * @param[in]							pBuffer					缓存地址
	 * @param[in]							nBufferSize				缓存长度
	 * @param[in,out]						nDbSerialNo				取出>nDbSerialNo的数据(若为0,则全部取出) & 将回填最新的流水号
	 * @return								>=0						返回数据长度
											<						出错
	 */
	int										CopyToBuffer( char* pBuffer, unsigned int nBufferSize, unsigned __int64& nDbSerialNo );

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
	CollisionHash							m_oHashTableOfIndex;	///< 对商品主键值作哈稀
	char*									m_pRecordsBuffer;		///< 记录集缓存
	unsigned int							m_nMaxBufferSize;		///< 记录集缓存最大长度
	unsigned int							m_nCurrentDataSize;		///< 当前有效数据的长度
};



}



#endif









