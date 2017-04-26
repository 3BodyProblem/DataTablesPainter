#include "Interface.h"
#include "stddef.h"


bool I_Record::IsNone() const
{
	return true;
}

int I_Record::CloneFrom( const I_Record& refRecord )
{
	return -1;
}

const char* I_Record::GetPtr() const
{
	return NULL;
}

unsigned int I_Record::Length() const
{
	return 0;
}

int I_Record::GetSerial() const
{
	return 0;
}

DBFactory::DBFactory()
{
}




