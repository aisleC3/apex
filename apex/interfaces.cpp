#include "apex.h"

bool Interfaces::Init()
{
	game_base = (DWORD_PTR)Memory::GetModuleBaseAddress(XorStr("r5apex.exe"));
	if (!game_base)
		return false;

	swapchain = *(IDXGISwapChain**)(game_base + 0x25DA81E8); //48 83 3D ? ? ? ? ? 41 8B D8 + 1
	if (!swapchain)
		return false;

	enginetrace = (CEngineTrace*)GetInterface(XorStr("EngineTraceClient004"), true);
	if (!enginetrace)
		return false;

	engine = *(CEngineClient**)(game_base + 0xC1891A8); //48 8B 0D ? ? ? ? 48 8D 54 24 ? E8 ? ? ? ? 48 8B 5C 24 ? + 1
	if (!engine)
		return false;

	entlist = (CClientEntityList*)GetInterface(XorStr("VClientEntityList003"), true);
	if (!entlist)
		return false;

	client = (IBaseClientDLL*)GetInterface(XorStr("VClient"));
	if (!client)
		return false;

	return true;
}

ptr Interfaces::GetInterface(const std::string& name, bool force)
{
	ptr interfaceptr;

	FnCreateInterface CreateInterface = (FnCreateInterface)(Memory::SigScan(XorStr("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B 1D ? ? ? ? 48 8B FA"), XorStr("r5apex.exe")));
	//if (!CreateInterface)
	//return NULL;

	if (!force)
	{
		for (int i = 0; i < 1000; i++)
		{
			char buffer[1024];

			if (i < 10)
				sprintf_s(buffer, "%s00%i", name, i);
			else if (i < 100)
				sprintf_s(buffer, "%s0%i", name, i);
			else if (i < 1000)
				sprintf_s(buffer, "%s%i", name, i);

			interfaceptr = (ptr)CreateInterface(buffer, 0);

			if (interfaceptr)
				break;
		}
	}
	else
		interfaceptr = (ptr)CreateInterface(name.c_str(), 0);

	return interfaceptr;
}

Interfaces interfaces;