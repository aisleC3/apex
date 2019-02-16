#include "apex.h"

vars var;
DWORD dwImageSize = NULL;
DWORD dwImageBase = NULL;

void Init()
{
	dwImageBase = (DWORD64)Memory::GetModuleBaseAddress("r5apex.exe");

	PIMAGE_NT_HEADERS nthdr = Memory::GetNTHeader(Memory::GetModuleBaseAddress(("r5apex.exe")));
	if (nthdr == nullptr)
		return false;

	dwImageSize = (DWORD64)nthdr->OptionalHeader.SizeOfImage;

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