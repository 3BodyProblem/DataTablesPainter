#include "targetver.h"
#include "DataTablesPainter.h"
#include "MemTable\MemDatabase.h"


extern "C"
{
	__declspec(dllexport) int  StartWork()
	{
		MemoryCollection::MemDatabase	db;

		return 0;
	}

}




