#include "targetver.h"
#include <exception>
#include <algorithm>
#include <functional>
#include "UnitTest/UnitTest.h"
#include "DataTablesPainter.h"
#include "MemTable/MemDatabase.h"


static std::vector<I_Database*>		s_vctIDatabase;			///< 数据库指针列表
static 	CriticalObject				s_oFactoryLock;			///< 工厂锁


DBFactory::DBFactory()
{
	s_vctIDatabase.reserve( 128*2 );
}

IDBFactory& DBFactory::GetFactory()
{
	static DBFactory	objFactory;

	return objFactory;
}

I_Database* DBFactory::GrapDatabaseInterface()
{
	try
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
	catch( std::exception& err )
	{
		::printf( "DBFactory::GrapDatabaseInterface() : exception : %s\n", err.what() );
		return NULL;
	}
	catch( ... )
	{
		::printf( "DBFactory::GrapDatabaseInterface() : unknow exception\n" );
		return NULL;
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


extern "C"
{
	__declspec(dllexport) IDBFactory&  GetFactoryObject()
	{
		::printf( "[Version]  1.0.1 \n" );

		return DBFactory::GetFactory();
	}

	__declspec(dllexport) void	ExecuteUnitTest()
	{
		::printf( "\n\n---------------------- [Begin] -------------------------\n" );
		ExecuteTestCase();
		::printf( "----------------------  [End]  -------------------------\n\n\n" );
	}

}




