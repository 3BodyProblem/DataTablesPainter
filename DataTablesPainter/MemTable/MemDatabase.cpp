#include "MemDatabase.h"


namespace MemoryCollection
{


MemDatabase::MemDatabase()
	: m_nUsedTableNum( 0 )
{
}

void MemDatabase::DeleteTables()
{
	m_nUsedTableNum = 0;					///< ���ù���
	m_HashTableOfPostion.Clear();			///< ��ϡ�����
}

bool MemDatabase::CreateTable( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen )
{
	int				nResult = -1;
	CriticalLock	lock( m_oCSLock );

	///< �������ú����ݱ�������ϡ
	if( (nResult=m_HashTableOfPostion.SetHashPair( nBindID, struct T_TABLE_POS_INF(m_nUsedTableNum) )) == 0 )
	{	///< ʹ�����ݱ�Ԫ��Ϣ�ṹ��ʼ���շ�������ݱ����
		m_arrayQuotationTables[m_nUsedTableNum].Initialize( DynamicTable::TableMeta(nBindID,nRecordWidth,nKeyStrLen) );
		++m_nUsedTableNum;													///< ���ü����һ
	}

	return (nResult >= 0) ? true : false;
}

I_Table* MemDatabase::QueryTable( unsigned int nBindID )
{
	CriticalLock			lock( m_oCSLock );
	struct T_TABLE_POS_INF	InfoPosition = m_HashTableOfPostion[nBindID];	///< ȡ�ù�ϡ������Ϣ

	if( true == InfoPosition.Empty() )										///< �����ݱ�������Ϣ������
	{
		return NULL;
	}

	return &m_arrayQuotationTables[InfoPosition.nTablePosition];			///< ���ض�Ӧ�����ݱ�
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








