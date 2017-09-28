#ifndef __DATA_TABLE_PAINTER_DUMP_H__
#define	__DATA_TABLE_PAINTER_DUMP_H__


#include <windows.h>
#include <tchar.h>
#include <DbgHelp.h>


typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)
(
 IN HANDLE hProcess,
 IN DWORD ProcessId,
 IN HANDLE hFile,
 IN MINIDUMP_TYPE DumpType,
 IN CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
 OPTIONAL IN CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
 OPTIONAL IN CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL
 );


class CMiniDumper
{
public:
	CMiniDumper();
private:
	static LPTOP_LEVEL_EXCEPTION_FILTER s_pPrevFilter;
	static long WINAPI  UnhandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionInfo);
};


#endif





