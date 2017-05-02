#include "MemDatabase.h"


namespace MemoryCollection
{


MemDatabase::MemDatabase()
	: m_nUsedTableNum( 0 )
{
}

MemDatabase::~MemDatabase()
{
	try
	{
		DeleteTables();
	}
	catch( std::exception& err )
	{
		::printf( "MemDatabase::~MemDatabase() : exception : %s\n", err.what() );
	}
	catch( ... )
	{
		::printf( "MemDatabase::~MemDatabase() : unknow exception\n" );
	}
}

void MemDatabase::DeleteTables()
{
	try
	{
		std::for_each( m_arrayQuotationTables, m_arrayQuotationTables+MAX_TABBLE_NO, std::mem_fun_ref(&DynamicTable::Release) );

		m_nUsedTableNum = 0;					///< 引用归零
		m_HashTableOfPostion.Clear();			///< 哈稀表归零
	}
	catch( std::exception& err )
	{
		::printf( "MemDatabase::DeleteTables() : exception : %s\n", err.what() );
	}
	catch( ... )
	{
		::printf( "MemDatabase::DeleteTables() : unknow exception\n" );
	}
}

bool MemDatabase::CreateTable( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen )
{
	try
	{
		int						nResult = -1;
		CriticalLock			lock( m_oCSLock );

		///< 分配设置好数据表索引哈稀
		if( (nResult=m_HashTableOfPostion.NewKey( nBindID, struct T_TABLE_POS_INF(m_nUsedTableNum) )) > 0 )
		{	///< 使用数据表元信息结构初始化刚分配的数据表对象
			if( m_nUsedTableNum >= (MAX_TABBLE_NO-1) )
			{
				::printf( "MemDatabase::CreateTable() : no more space\n" );
				return false;
			}

			m_arrayQuotationTables[m_nUsedTableNum].Initialize( DynamicTable::TableMeta(nBindID,nRecordWidth,nKeyStrLen) );
			++m_nUsedTableNum;													///< 引用计算加一
		}

		return (nResult >= 0) ? true : false;
	}
	catch( std::exception& err )
	{
		::printf( "MemDatabase::CreateTable() : exception : %s\n", err.what() );
	}
	catch( ... )
	{
		::printf( "MemDatabase::CreateTable() : unknow exception\n" );
	}

	return false;
}

I_Table* MemDatabase::QueryTable( unsigned int nBindID )
{
	try
	{
		CriticalLock				lock( m_oCSLock );
		struct T_TABLE_POS_INF		infoPosition = m_HashTableOfPostion[nBindID];	///< 取得哈稀索引信息

		if( true == infoPosition.Empty() )											///< 该数据表索引信息不存在
		{
			::printf( "MemDatabase::QueryTable() : invalid BindID, subscript out of range of memo-tables list\n" );
			return NULL;
		}

		return &m_arrayQuotationTables[infoPosition.nTablePosition];				///< 返回对应的数据表
	}
	catch( std::exception& err )
	{
		::printf( "MemDatabase::QueryTable() : exception : %s\n", err.what() );
	}
	catch( ... )
	{
		::printf( "MemDatabase::QueryTable() : unknow exception\n" );
	}

	return NULL;
}

I_Table* MemDatabase::operator[]( unsigned int nTableIndex )
{
	CriticalLock	lock( m_oCSLock );

	if( nTableIndex >= m_nUsedTableNum || nTableIndex >= MAX_TABBLE_NO )
	{
		::printf( "MemDatabase::operator[]() : subscript out of range of memo-tables list\n" );
		return NULL;
	}

	return &(m_arrayQuotationTables[nTableIndex]);
}

unsigned int MemDatabase::GetTableCount()
{
	CriticalLock	lock( m_oCSLock );

	return m_nUsedTableNum;
}

bool MemDatabase::LoadFromDisk( const char* pszDataFile )
{
	try
	{
		CriticalLock	lock( m_oCSLock );

		return true;
	}
	catch( std::exception& err )
	{
		::printf( "MemDatabase::LoadFromDisk() : %s \n", err.what() );
	}
	catch( ... )
	{
		::printf( "MemDatabase::LoadFromDisk() : unknow exception\n" );
	}

	return false;
}

bool MemDatabase::SaveToDisk( const char* pszDataFile )
{
	try
	{
		CriticalLock	lock( m_oCSLock );

		return true;
	}
	catch( std::exception& err )
	{
		::printf( "MemDatabase::SaveToDisk() : %s \n", err.what() );
	}
	catch( ... )
	{
		::printf( "MemDatabase::SaveToDisk() : unknow exception\n" );
	}

	return false;
}



}








