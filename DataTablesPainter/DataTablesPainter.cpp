#include "targetver.h"
#include "UnitTest/UnitTest.h"
#include "DataTablesPainter.h"
#include "MemTable/MemDatabase.h"


extern "C"
{
	__declspec(dllexport) int  StartWork()
	{
		MemoryCollection::MemDatabase	db;

		return 0;
	}

	__declspec(dllexport) void	ExecuteUnitTest()
	{
		printf( "aaaaaa\n" );
		ExecuteTestCase();
	}

}




