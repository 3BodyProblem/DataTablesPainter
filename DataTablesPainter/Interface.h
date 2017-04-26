#ifndef __MEM_DATABASE_INTERFACE_H__
#define	__MEM_DATABASE_INTERFACE_H__


/**
 * @classs			I_Record
 * @brief			记录操作接口
 */
class I_Record
{
public:
	/**
	 * @brief						记录对象为无效
	 * @return						true							无效对象
	 */
	virtual bool					IsNone() const;

	/**
	 * @brief						记录体deeply copy
	 * @param[in]					refRecord						内存copy的源头
	 * @return						==1								影响的数量
									==0								不需要copy，目标和源头数据完全一样
									<0								出现错误
	 */
	virtual int						CloneFrom( const I_Record& refRecord );

	/**
	 * @brief						取得数据地址
	 */
	virtual const char*				GetPtr() const;

	/**
	 * @brief						数据长度
	 */
	virtual unsigned int			Length() const;

	/**
	 * @brief						取得索引号
	 */
	virtual int						GetSerial() const;
};


/**
 * @class			I_Table
 * @brief			数据表操作接口
 */
class I_Table
{
public:
	/**
	 * @brief								追加新数据
	 * @param[in]							refRecord				追加的数据
	 * @return								==0						增加成功
											!=0						失败
	 */
	virtual int								InsertRecord( const I_Record& refRecord ) = 0; 

	/**
	 * @brief								索引出记录对象
	 * @param[in]							Index					记录索引
	 * @return								返回记录对象
	 */
	virtual I_Record						SelectRecord( int nRecordIndex ) = 0;
};


/**
 * @class			I_Database
 * @brief			数据库操作接口
 */
class I_Database
{
public:
	/**
	 * @brief					根据消息id和消息长度，进行合适的数据表配置（在预备表中配置对应的占用关系）
	 * @param[in]				nBindID				数据类形标识号
	 * @param[in]				nRecordWidth		数据长度
	 * @param[in]				nKeyStrLen			主键长度
	 * @return					=0					配置成功
								>0					忽略（成功）
								<0					配置出错
	 */
	virtual bool				CreateTable( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen ) = 0;

	/**
	 * @brief					根据MessageID取得已经存在的或者分配一个新的内存表的引用
	 * @detail					本函数对每个messageid维护一个唯一且对应的内存表，根据nBindID值返回已经存在的，或新建后返回
	 * @param[in]				nBindID				MessageID
	 * @return					返回已经存在的内存表或新建的内存表
	 */
	virtual I_Table*			QueryTable( unsigned int nBindID ) = 0;

	/**
	 * @brief					清理所有数据表
	 */
	virtual void				DeleteTables() = 0;

	/**
	 * @brief					从硬盘恢复所有数据
	 */
	virtual bool				LoadFromDisk( const char* pszDataFile ) = 0;

	/**
	 * @brief					将所有数据存盘
	 */
	virtual bool				SaveToDisk( const char* pszDataFile ) = 0;
};


/**
 * @class				DBFactory
 * @brief				内存数据分配管理工厂类
 */
class DBFactory
{
public:
	DBFactory();
};





#endif





