#include "MemDatabase.h"


namespace MemoryCollection
{


MemDatabase::MemDatabase()
	: m_nUsedTableNum( 0 )
{
}

void MemDatabase::FreeTables()
{
	m_nUsedTableNum = 0;					///< ���ù���
	m_HashTableOfPostion.Clear();			///< ��ϡ�����
}

bool MemDatabase::AllotNewTable( VariableRecordTable::TableMeta& refTableMeta )
{
	int		nResult = -1;

	///< �������ú����ݱ�������ϡ
	if( (nResult=m_HashTableOfPostion.SetHashPair( refTableMeta.m_nBindID, struct T_TABLE_POS_INF(m_nUsedTableNum, -1) )) == 0 )
	{
		m_arrayQuotationTables[m_nUsedTableNum].Initialize( refTableMeta );	///< ʹ�����ݱ�Ԫ��Ϣ�ṹ��ʼ���շ�������ݱ����
		++m_nUsedTableNum;													///< ���ü����һ
	}

	return (nResult >= 0) ? true : false;
}

VariableRecordTable* MemDatabase::QueryTableByID( unsigned int nBindID )
{
	struct T_TABLE_POS_INF	InfoPosition = m_HashTableOfPostion[nBindID];	///< ȡ�ù�ϡ������Ϣ

	if( true == InfoPosition.Empty() )										///< �����ݱ�������Ϣ������
	{
		return NULL;
	}

	return &m_arrayQuotationTables[InfoPosition.nTablePosition];			///< ���ض�Ӧ�����ݱ�
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








