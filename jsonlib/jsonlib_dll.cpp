#include "jsonlib_lib.h"

extern "C"
{
	__declspec(dllexport) JSON *DLL_ParseFromFile(const char *filename)
	{
		return ParseFromFile(filename);
	}
	__declspec(dllexport) void DLL_PrintJSON(JSON *j)
	{
		PrintJSON(j);
	}
	__declspec(dllexport) void DLL_FreeJSON(JSON *j)
	{
		FreeJSON(j);
	}

	__declspec(dllexport) JSON *DLL_ParseFromStr(const char *str)
	{
		return ParseFromStr(str);
	}
	__declspec(dllexport) void DLL_ShowWheatherMemoryLeak()
	{
		ShowWheatherMemoryLeak();
	}
}