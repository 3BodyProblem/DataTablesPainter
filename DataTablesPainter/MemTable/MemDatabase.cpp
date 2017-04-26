#include "MemDatabase.h"


namespace MemoryCollection
{


MemDatabase::MemDatabase()
	: m_nUsedTableNum( 0 )
{
}

void MemDatabase::DeleteTables()
{
	m_nUsedTableNum = 0;					///< 引用归零
	m_HashTableOfPostion.Clear();			///< 哈稀表归零
}

bool MemDatabase::CreateTable( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen )
{
	int				nResult = -1;
	CriticalLock	lock( m_oCSLock );

	///< 分配设置好数据表索引哈稀
	if( (nResult=m_HashTableOfPostion.SetHashPair( nBindID, struct T_TABLE_POS_INF(m_nUsedTableNum) )) == 0 )
	{	///< 使用数据表元信息结构初始化刚分配的数据表对象
		m_arrayQuotationTables[m_nUsedTableNum].Initialize( DynamicTable::TableMeta(nBindID,nRecordWidth,nKeyStrLen) );
		++m_nUsedTableNum;													///< 引用计算加一
	}

	return (nResult >= 0) ? true : false;
}

I_Table* MemDatabase::QueryTable( unsigned int nBindID )
{
	CriticalLock			lock( m_oCSLock );
	struct T_TABLE_POS_INF	InfoPosition = m_HashTableOfPostion[nBindID];	///< 取得哈稀索引信息

	if( true == InfoPosition.Empty() )										///< 该数据表索引信息不存在
	{
		return NULL;
	}

	return &m_arrayQuotationTables[InfoPosition.nTablePosition];			///< 返回对应的数据表
}

I_Table* MemDatabase::operator[]( unsigned int nTableIndex )
{
	CriticalLock	lock( m_oCSLock );

	if( nTableIndex >= m_nUsedTableNum || nTableIndex >= MAX_TABBLE_NO )
	{
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
		::printf( "%s \n", err.what() );
	}
	catch( ... )
	{
		::printf( "unknow exception\n" );
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
		::printf( "%s \n", err.what() );
	}
	catch( ... )
	{
		::printf( "unknow exception\n" );
	}

	return false;
}



}








