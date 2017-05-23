#include "MemDatabase.h"
#include "../Infrastructure/DataDump.h"
#include "../Infrastructure/DateTime.h"


namespace MemoryCollection
{

#define		MAX_QUERY_BUFFER_LEN			(1024*1024*10)			///< 最大查询缓存大小

MemDatabase::MemDatabase()
 : m_nUsedTableNum( 0 ), m_pQueryBuffer( NULL )
{
	m_pQueryBuffer = new char[MAX_QUERY_BUFFER_LEN];
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

int MemDatabase::DeleteTables()
{
	try
	{
		std::for_each( m_arrayQuotationTables, m_arrayQuotationTables+MAX_TABBLE_NO, std::mem_fun_ref(&DynamicTable::Release) );

		m_nUsedTableNum = 0;					///< 引用归零
		m_HashTableOfPostion.Clear();			///< 哈稀表归零

		return 0;
	}
	catch( std::exception& err )
	{
		::printf( "MemDatabase::DeleteTables() : exception : %s\n", err.what() );
		return -1;
	}
	catch( ... )
	{
		::printf( "MemDatabase::DeleteTables() : unknow exception\n" );
		return -2;
	}
}

bool MemDatabase::CreateTable( unsigned int nBindID, unsigned int nRecordWidth, unsigned int nKeyStrLen )
{
	try
	{
		int						nResult = -1;
		CriticalLock			lock( m_oCSLock );

		if( NULL == m_pQueryBuffer )
		{
			::printf( "MemDatabase::CreateTable() : invalid query buffer pointer(NULL)\n" );
			return false;
		}

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
		struct T_TABLE_POS_INF*		pInfoPosition = m_HashTableOfPostion[nBindID];	///< 取得哈稀索引信息

		if( NULL == pInfoPosition )
		{
			::printf( "MemDatabase::QueryTable() : 1 invalid BindID, subscript out of range of memo-tables list, BindID=%u\n", nBindID );
			return NULL;
		}

		if( true == pInfoPosition->Empty() )											///< 该数据表索引信息不存在
		{
			::printf( "MemDatabase::QueryTable() : 2 invalid BindID, subscript out of range of memo-tables list, BindID=%u\n", nBindID );
			return NULL;
		}

		return &m_arrayQuotationTables[pInfoPosition->nTablePosition];				///< 返回对应的数据表
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
		int						nErrorCode = 0;
		unsigned int			s_nLastPos = 0;
		CriticalLock			lock( m_oCSLock );

		GlobalSequenceNo::GetObj().Reset();				///< 重置自增流水号
		if( 0 != (nErrorCode=DeleteTables()) )	{
			::printf( "MemDatabase::LoadFromDisk() : failed 2 clean all tables in mem-database \n", nErrorCode );
			return false;
		}

		MemoDumper<char>		fileMeta( true, JoinPath( pszDataFile, "meta.dump" ).c_str(), 0 );
		if( false == fileMeta.IsOpen() )	{
			::printf( "MemDatabase::LoadFromDisk() : failed 2 load meta info. \n" );
			return false;
		}

		int						nDataLen = fileMeta.Read( m_pQueryBuffer, MAX_QUERY_BUFFER_LEN );
		for( unsigned int nOffset = 0; nOffset < nDataLen; nOffset++ )
		{
			if( ',' == m_pQueryBuffer[nOffset] )
			{
				I_Table*			pTable = NULL;
				char				pszRecord[512] = { 0 };
				unsigned __int64	nSerialNo = 0;
				unsigned int		nRecordWidth = 0;
				std::string			sFileName( m_pQueryBuffer+s_nLastPos, nOffset-s_nLastPos );
				unsigned int		nDataID = ::atol( sFileName.c_str() );
				MemoDumper<char>	fileDump( true, JoinPath( pszDataFile, (sFileName+".dump").c_str() ).c_str(), 0 );

				if( false == fileDump.IsOpen() )	{
					::printf( "MemDatabase::LoadFromDisk() : failed 2 load dump file, %s \n", sFileName.c_str() );
					return false;
				}

				if( fileDump.Read( (char*)&nRecordWidth, sizeof(nRecordWidth) ) < 0 )	{
					::printf( "MemDatabase::LoadFromDisk() : failed 2 read dump file \n" );
					return false;
				}

				for( int n = 0; true; n++ )
				{
					if( fileDump.Read( pszRecord, nRecordWidth ) <= 0 )	{
						break;
					}

					if( 0 == n )	{
						if( false == CreateTable( nDataID, nRecordWidth, 32 ) )
						{
							::printf( "MemDatabase::LoadFromDisk() : failed 2 create table, table id = %d\n", nDataID );
							return false;
						}
					}

					if( NULL == (pTable = QueryTable( nDataID )) )
					{
						::printf( "MemDatabase::LoadFromDisk() : failed 2 query table, table id = %d\n", nDataID );
						return false;
					}

					if( 0 > pTable->InsertRecord( pszRecord, nRecordWidth, nSerialNo ) )
					{
						::printf( "MemDatabase::LoadFromDisk() : failed 2 insert into table, table id = %d\n", nDataID );
						return false;
					}
				}

				s_nLastPos = nOffset + 1;
			}
		}

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
		CriticalLock				lock( m_oCSLock );

		if( NULL == m_pQueryBuffer )	{
			::printf( "MemDatabase::SaveToDisk() : invalid query buffer pointer(NULL) \n" );
			return false;
		}

		for( unsigned int n = 0; n < GetTableCount(); n++ )
		{
			unsigned __int64		nSerialNo = 0;
			DynamicTable*			pTable = (DynamicTable*)(*this)[n];
			char					pszTmpFileName[64] = { 0 };
			int						nDataSize = pTable->CopyToBuffer( m_pQueryBuffer, MAX_QUERY_BUFFER_LEN, nSerialNo );

			if( nDataSize <= 0 && NULL == pTable )
			{
				::printf( "MemDatabase::SaveToDisk() : failed 2 copy table data, errorcode=%d \n", nDataSize );
				return false;
			}

			::sprintf( pszTmpFileName, "%d.dump", pTable->GetMeta().m_nBindID );
			MemoDumper<char>	fileDump( false, JoinPath( pszDataFile, pszTmpFileName ).c_str(), DateTime::Now().DateToLong() );
			unsigned int		nRecordWidth = pTable->GetMeta().m_nRecordWidth;

			if( fileDump.Write( (char*)&nRecordWidth, sizeof(nRecordWidth) ) < 0 )
			{
				::printf( "MemDatabase::SaveToDisk() : failed 2 dump table data (a) \n" );
				return false;
			}

			if( fileDump.Write( m_pQueryBuffer, nDataSize ) < 0 )
			{
				::printf( "MemDatabase::SaveToDisk() : failed 2 dump table data (b) \n" );
				return false;
			}
		}

		MemoDumper<char>		fileMeta( false, JoinPath( pszDataFile, "meta.dump" ).c_str(), DateTime::Now().DateToLong() );

		for( unsigned int i = 0; i < GetTableCount(); i++ )
		{
			char				pszDataID[64] = { 0 };

			::sprintf( pszDataID, "%d,", ((DynamicTable*)(*this)[i])->GetMeta().m_nBindID );
			fileMeta.Write( pszDataID, ::strlen(pszDataID) );
		}

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

unsigned __int64 MemDatabase::GetUpdateSequence()
{
	return GlobalSequenceNo::GetObj().GetSeqNo();
}



}








