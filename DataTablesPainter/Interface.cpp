#include <vector>
#include "stddef.h"
#include <exception>
#include <algorithm>
#include <functional>
#include "Interface.h"
#include "Infrastructure/Lock.h"
#include "MemTable/MemDatabase.h"


RecordBlock::RecordBlock()
 : m_pRecordData( NULL ), m_nRecordLen( 0 )
{
}

RecordBlock::RecordBlock( const RecordBlock& record )
{
	m_pRecordData = record.m_pRecordData;
	m_nRecordLen = record.m_nRecordLen;
}

RecordBlock::RecordBlock( char* pRecord, unsigned int nRecordLen )
 : m_pRecordData( pRecord ), m_nRecordLen( nRecordLen )
{
}

unsigned int RecordBlock::Length() const
{
	return m_nRecordLen;
}

const char* RecordBlock::GetPtr() const
{
	return m_pRecordData;
}

bool RecordBlock::IsNone() const
{
	if( NULL == m_pRecordData || m_nRecordLen <= 0 )
	{
		return true;
	}

	return false;
}

bool RecordBlock::Compare( const RecordBlock& refRecord )
{
	if( true == IsNone() || refRecord.IsNone() )
	{
		return false;
	}

	if( GetPtr() == refRecord.GetPtr() && Length() == refRecord.Length() )
	{
		return true;
	}

	if( 0 == ::memcmp( GetPtr(), refRecord.GetPtr(), refRecord.Length() ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

int RecordBlock::CloneFrom( const RecordBlock& refRecord )
{
	if( true == refRecord.IsNone() )
	{
		return -1;
	}

	if( 0 != ::memcmp( m_pRecordData, refRecord.GetPtr(), refRecord.Length() ) )
	{
		memcpy( m_pRecordData, refRecord.GetPtr(), refRecord.Length() );

		return 1;
	}

	return 0;
}


I_Database::~I_Database()
{
}


///< ------------------------ Factory Object -------------------------------------


static std::vector<I_Database*>		s_vctIDatabase;			///< 数据库指针列表
static 	CriticalObject				s_oFactoryLock;			///< 工厂锁


DBFactory::DBFactory()
{
	s_vctIDatabase.reserve( 128*2 );
}

DBFactory& DBFactory::GetFactory()
{
	static DBFactory	objFactory;

	return objFactory;
}

I_Database* DBFactory::GrapDatabaseInterface()
{
	CriticalLock	lock( s_oFactoryLock );
	I_Database*		pIDatabase = new MemoryCollection::MemDatabase();

	if( NULL == pIDatabase )
	{
		return NULL;
	}
	else
	{
		s_vctIDatabase.push_back( pIDatabase );

		return pIDatabase;
	}
}

void DeleteDatabasePtr( I_Database* pIDatabase )
{
	if( NULL != pIDatabase )
	{
		delete pIDatabase;
	}
}

bool DBFactory::ReleaseAllDatabase()
{
	try
	{
		CriticalLock	lock( s_oFactoryLock );

		std::for_each( s_vctIDatabase.begin(), s_vctIDatabase.end(), DeleteDatabasePtr );
		s_vctIDatabase.clear();

		return true;
	}
	catch( std::exception& err )
	{
		::printf( "DBFactory::ReleaseAllDatabase() : %s\n", err.what() );
	}
	catch( ... )
	{
		::printf( "DBFactory::ReleaseAllDatabase() : unknow error occur in function.\n" );
	}

	return false;
}










