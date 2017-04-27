#include "targetver.h"
#include "UnitTest/UnitTest.h"
#include "DataTablesPainter.h"



extern "C"
{
	__declspec(dllexport) DBFactory&  GetFactoryObject()
	{
		return DBFactory::GetFactory();
	}

	__declspec(dllexport) void	ExecuteUnitTest()
	{
		::printf( "\n\n---------------------- [Begin] -------------------------\n" );
		ExecuteTestCase();
		::printf( "----------------------  [End]  -------------------------\n\n\n" );
	}

}




