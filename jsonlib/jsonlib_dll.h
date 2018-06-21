#ifndef _JSONLIB_DLL_H
#define _JSONLIB_DLL_H

#include "jsonlib.h"
#include "assert.h"
#include "windows.h"

typedef JSON *(*PTR_ParseFromFile)(const char *filename);
typedef void (*PTR_PrintJSON)(JSON *j);
typedef void (*PTR_FreeJSON)(JSON *j);
typedef JSON *(*PTR_ParseFromStr)(const char *str);
typedef void (*PTR_ShowWheatherMemoryLeak)();

PTR_ParseFromFile ParseFromFile;
PTR_PrintJSON PrintJSON;
PTR_FreeJSON FreeJSON;
PTR_ParseFromStr ParseFromStr;
PTR_ShowWheatherMemoryLeak ShowWheatherMemoryLeak;

void LoadAllFunc()
{
	HMODULE jsonlib_dll = LoadLibrary(LPCSTR("jsonlib_dll.dll"));

	assert(jsonlib_dll);

	ParseFromFile = reinterpret_cast<PTR_ParseFromFile>(GetProcAddress(jsonlib_dll,"DLL_ParseFromFile"));
	PrintJSON =  reinterpret_cast<PTR_PrintJSON>(GetProcAddress(jsonlib_dll,"DLL_PrintJSON"));
	FreeJSON =  reinterpret_cast<PTR_FreeJSON>(GetProcAddress(jsonlib_dll,"DLL_FreeJSON"));
	ParseFromStr = reinterpret_cast<PTR_ParseFromStr>(GetProcAddress(jsonlib_dll,"DLL_ParseFromStr"));
	ShowWheatherMemoryLeak = reinterpret_cast<PTR_ShowWheatherMemoryLeak>(GetProcAddress(jsonlib_dll,"DLL_ShowWheatherMemoryLeak"));
	
	assert(ParseFromFile && PrintJSON && FreeJSON && ParseFromStr && ShowWheatherMemoryLeak);

	return ;
}

#endif