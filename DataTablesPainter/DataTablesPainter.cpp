#include "targetver.h"
#include "UnitTest/UnitTest.h"
#include "DataTablesPainter.h"
#include "MemTable/MemDatabase.h"


extern "C"
{
	__declspec(dllexport) DBFactory&  GetFactoryObject()
	{
		static	DBFactory		objFactory;

		return objFactory;
	}

	__declspec(dllexport) void	ExecuteUnitTest()
	{
		::printf( "\n\n---------------------- [Begin] -------------------------\n" );
		ExecuteTestCase();
		::printf( "----------------------  [End]  -------------------------\n\n\n" );
	}

}




