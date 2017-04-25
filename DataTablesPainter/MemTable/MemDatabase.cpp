#include "MemDatabase.h"


namespace MemoryCollection
{


MemDatabase::MemDatabase()
	: m_nUsedTableNum( 0 )
{
}

void MemDatabase::FreeTables()
{
	m_nUsedTableNum = 0;					///< 引用归零
	m_HashTableOfPostion.Clear();			///< 哈稀表归零
}

bool MemDatabase::AllotNewTable( VariableRecordTable::TableMeta& refTableMeta )
{
	int		nResult = -1;

	///< 分配设置好数据表索引哈稀
	if( (nResult=m_HashTableOfPostion.SetHashPair( refTableMeta.m_nBindID, struct T_TABLE_POS_INF(m_nUsedTableNum, -1) )) == 0 )
	{
		m_arrayQuotationTables[m_nUsedTableNum].Initialize( refTableMeta );	///< 使用数据表元信息结构初始化刚分配的数据表对象
		++m_nUsedTableNum;													///< 引用计算加一
	}

	return (nResult >= 0) ? true : false;
}

VariableRecordTable* MemDatabase::QueryTableByID( unsigned int nBindID )
{
	struct T_TABLE_POS_INF	InfoPosition = m_HashTableOfPostion[nBindID];	///< 取得哈稀索引信息

	if( true == InfoPosition.Empty() )										///< 该数据表索引信息不存在
	{
		return NULL;
	}

	return &m_arrayQuotationTables[InfoPosition.nTablePosition];			///< 返回对应的数据表
}

bool MemDatabase::LoadFromDisk()
{
	try
	{

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

bool MemDatabase::SaveToDisk()
{
	try
	{

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








