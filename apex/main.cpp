#include "apex.h"

vars var;
DWORD dwImageSize = NULL;
DWORD dwImageBase = NULL;

void Init()
{
	interfaces.Init();
	hooks.Init();
}

DWORD __stdcall DllMain(HMODULE dll, DWORD callreason, void* reserved)
{
	if (callreason == DLL_PROCESS_ATTACH)
	{
		Init();

		return 1;
	}

	return 0;
}