#include "MemDatabase.h"


namespace MemoryCollection
{


MemDatabase::MemDatabase()
{
}

void MemDatabase::Clear()
{
	m_HashTableOfPostion.Clear();
}

bool MemDatabase::AllotTable4MsgID( unsigned int nMsgID, unsigned int nMsgSize )
{
	int		nResult = -1;

	if( nMsgSize < E_RECORD_64BYTES )
	{
		unsigned int	nNewTableIndex = m_ArrayOfTable64Bytes.Size()+1;
		if( (nResult=m_HashTableOfPostion.SetHashPair( nMsgID, struct T_TABLE_POS_INF(E_RECORD_64BYTES, nNewTableIndex, m_ArrayOfTable64Bytes[nNewTableIndex].Size()) )) == 0 )
		{
			m_ArrayOfTable64Bytes.IncreaseTableNum();
		}
	}
	else if( nMsgSize < E_RECORD_128BYTES )
	{
		unsigned int	nNewTableIndex = m_ArrayOfTable128Bytes.Size()+1;
		if( (nResult=m_HashTableOfPostion.SetHashPair( nMsgID, struct T_TABLE_POS_INF(E_RECORD_128BYTES, nNewTableIndex, m_ArrayOfTable128Bytes[nNewTableIndex].Size()) )) == 0 )
		{
			m_ArrayOfTable128Bytes.IncreaseTableNum();
		}
	}
	else if( nMsgSize < E_RECORD_256BYTES )
	{
		unsigned int	nNewTableIndex = m_ArrayOfTable256Bytes.Size()+1;
		if( (nResult=m_HashTableOfPostion.SetHashPair( nMsgID, struct T_TABLE_POS_INF(E_RECORD_256BYTES, nNewTableIndex, m_ArrayOfTable256Bytes[nNewTableIndex].Size()) )) == 0 )
		{
			m_ArrayOfTable256Bytes.IncreaseTableNum();
		}
	}
	else if( nMsgSize < E_RECORD_512BYTES )
	{
		unsigned int	nNewTableIndex = m_ArrayOfTable512Bytes.Size()+1;
		if( (nResult=m_HashTableOfPostion.SetHashPair( nMsgID, struct T_TABLE_POS_INF(E_RECORD_512BYTES, nNewTableIndex, m_ArrayOfTable512Bytes[nNewTableIndex].Size()) )) == 0 )
		{
			m_ArrayOfTable512Bytes.IncreaseTableNum();
		}
	}

	return (nResult >= 0) ? true : false;
}

IMemoryTableOperator* MemDatabase::QueryTableByMsgID( unsigned int nMsgID )
{
	struct T_TABLE_POS_INF	InfoPosition = m_HashTableOfPostion[nMsgID];

	switch( InfoPosition.eRecordType )
	{
	case E_RECORD_64BYTES:
		return &m_ArrayOfTable64Bytes[InfoPosition.nTablePosition];
	case E_RECORD_128BYTES:
		return &m_ArrayOfTable128Bytes[InfoPosition.nTablePosition];
	case E_RECORD_256BYTES:
		return &m_ArrayOfTable256Bytes[InfoPosition.nTablePosition];
	case E_RECORD_512BYTES:
		return &m_ArrayOfTable512Bytes[InfoPosition.nTablePosition];
	}

	return NULL;
}




}



