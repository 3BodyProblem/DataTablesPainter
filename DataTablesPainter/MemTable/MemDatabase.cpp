#include "MemDatabase.h"


namespace MemoryCollection
{


MemDatabase::MemDatabase( unsigned int nMaxTablesNum )
	: m_nUsedTableNum( 0 )
{
}

void MemDatabase::Clear()
{
	m_nUsedTableNum = 0;
	m_HashTableOfPostion.Clear();
}

bool MemDatabase::AllotTable4MsgID( unsigned int nMsgID, unsigned int nMsgSize )
{
	int		nResult = -1;

	if( (nResult=m_HashTableOfPostion.SetHashPair( nMsgID, struct T_TABLE_POS_INF(m_nUsedTableNum, m_arrayQuotationTables[m_nUsedTableNum].size()) )) == 0 )
	{
		++m_nUsedTableNum;
	}

	return (nResult >= 0) ? true : false;
}

VariableRecordTable* MemDatabase::QueryTableByMsgID( unsigned int nMsgID )
{
	struct T_TABLE_POS_INF	InfoPosition = m_HashTableOfPostion[nMsgID];

//	return &m_vctQuotationTables[InfoPosition.nTablePosition];

	return NULL;
}




}



