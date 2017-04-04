#include "MemTable.h"


namespace MemoryCollection
{


MemTablePool::MemTablePool()
{
}

void MemTablePool::Clear()
{
	m_HashTableOfPostion.Clear();
}

bool MemTablePool::ConfigTable2Pool( unsigned int nMsgID, unsigned int nMsgSize )
{
	int		nResult = 0;

	if( nMsgSize < E_RECORD_64BYTES )
	{
		unsigned int	nNewTableIndex = m_ArrayOfTable64Bytes.GetUsedTableNumber()+1;
		if( (nResult=m_HashTableOfPostion.SetHashPair( nMsgID, struct T_TABLE_POS_INF(E_RECORD_64BYTES, nNewTableIndex, m_ArrayOfTable64Bytes[nNewTableIndex].GetSize()) )) >= 0 )
		{
			if( 0 == nResult ) {
				m_ArrayOfTable64Bytes.IncUsedTableNum();
			}

			return true;
		}
	}
	else if( nMsgSize < E_RECORD_128BYTES )
	{
		m_HashTableOfPostion.SetHashPair( nMsgID, struct T_TABLE_POS_INF(E_RECORD_128BYTES, 0, 0) );
	}
	else if( nMsgSize < E_RECORD_256BYTES )
	{
		m_HashTableOfPostion.SetHashPair( nMsgID, struct T_TABLE_POS_INF(E_RECORD_256BYTES, 0, 0) );
	}
	else if( nMsgSize < E_RECORD_512BYTES )
	{
		m_HashTableOfPostion.SetHashPair( nMsgID, struct T_TABLE_POS_INF(E_RECORD_512BYTES, 0, 0) );
	}

	return false;
}

IMemoryTable* MemTablePool::operator[]( unsigned int nMsgID )
{
	return NULL;
}




}







