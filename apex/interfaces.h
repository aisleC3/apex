#pragma once

typedef void* (*FnCreateInterface)(const char* name, int* pReturnCode);

class Interfaces
{
public:
	DWORD64 game_base;
	IDXGISwapChain* swapchain;
	CEngineClient* engine;
	CEngineTrace* enginetrace;
	IBaseClientDLL* client;
	CClientEntityList* entlist;

	bool Init();
	ptr GetInterface(const std::string& name, bool force = false);
};

extern Interfaces interfaces;