#include "MemDatabase.h"


namespace MemoryCollection
{


MemDatabase::MemDatabase()
	: m_nUsedTableNum( 0 )
{
}

void MemDatabase::Clear()
{
	m_nUsedTableNum = 0;
	m_HashTableOfPostion.Clear();
}

bool MemDatabase::AllotTable4MsgID( VariableRecordTable::TableMeta& refTableMeta )
{
	int		nResult = -1;

	if( (nResult=m_HashTableOfPostion.SetHashPair( refTableMeta.m_nBindID, struct T_TABLE_POS_INF(m_nUsedTableNum, -1) )) == 0 )
	{
		++m_nUsedTableNum;
	}

	return (nResult >= 0) ? true : false;
}

VariableRecordTable* MemDatabase::QueryTableByMsgID( unsigned int nMsgID )
{
	struct T_TABLE_POS_INF	InfoPosition = m_HashTableOfPostion[nMsgID];

	return &m_arrayQuotationTables[InfoPosition.nTablePosition];
}




}



