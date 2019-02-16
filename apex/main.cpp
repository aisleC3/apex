#include "apex.h"

vars var;

void Init()
{
	if (!interfaces.Init())
		MessageBoxA(NULL, "Interfaces: Failed to init.", NULL, NULL);

	if (!hooks.Init())
		MessageBoxA(NULL, "PresentHook: Failed to init.", NULL, NULL);
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