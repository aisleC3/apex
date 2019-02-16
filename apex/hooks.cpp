#include "apex.h"

using PresentType = long(__stdcall*)(IDXGISwapChain*, UINT, UINT);
PresentType present_original;

ID3D11Device* device;
ID3D11DeviceContext* context;
Renderer* renderer;
long __stdcall Present(IDXGISwapChain* swapchain, UINT syncinterval, UINT flags)
{
	if (!device)
	{
		swapchain->GetDevice(__uuidof(device), (void**)&device);
		if (device)
		{
			device->GetImmediateContext(&context);
			if (context)
			{
				ImGui_ImplDX11_Init((HWND)FindWindow("Respawn001", NULL), device, context);
				ImGui_ImplDX11_CreateDeviceObjects();

				renderer = new Renderer();
				renderer->Initialize();

				menu.Init();
			}
		}
	}

	D3D11_VIEWPORT vp;
	UINT nvp = 1;
	context->RSGetViewports(&nvp, &vp);

	var.screen_height = vp.Height;
	var.screen_width = vp.Width;

	ImGui_ImplDX11_NewFrame();

	menu.Render();

	renderer->BeginScene();

	visuals.Players(renderer);

	//visuals.Entities(renderer);

	aimbot.Frame(renderer);

	renderer->EndScene();

	ImGui::Render();

	return present_original(swapchain, syncinterval, flags);
}

using FnTraceRay = void*(__thiscall*)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
FnTraceRay traceray_original;
void __fastcall TraceRay(void* instance, Ray_t& ray, unsigned int mask, ITraceFilter* filter, trace_t* trace)
{
	/*auto lp = interfaces.entlist->GetClientEntity(interfaces.engine->GetLocalPlayerID());

	//ray.m_Start = lp->GetEyePos();
	//ray.m_Delta = Vector(0, 1000, 0);

	mask = 0x640400B;

	MessageBoxA(NULL, std::to_string((DWORD_PTR)&ray).c_str(), NULL, NULL);

	traceray_original(instance, ray, mask, filter, trace);

	//MessageBoxA(NULL, std::to_string((DWORD_PTR)trace).c_str(), NULL, NULL);*/

	trace_t trace2;
	Ray_t ray2;
	ray2.m_Start = Vector(0, 20, 0);
	ray2.m_Delta = Vector(1, 2, 1);
	CTraceFilter filter2;
	//filter2.pSkip = lp;

	traceray_original(instance, ray2, 0x640400B, &filter2, &trace2);

	traceray_original(instance, ray, mask, filter, trace);
}

WNDPROC oWindowProc = NULL;
extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (handle == GetFocus())
	{
		if (msg == WM_KEYDOWN && wparam == VK_INSERT)
			menu.is_open = !menu.is_open;

		if (menu.is_open && ImGui_ImplDX11_WndProcHandler(handle, msg, wparam, lparam))
			return true;
	}

	return CallWindowProc(oWindowProc, handle, msg, wparam, lparam);
}

bool Hooks::Init()
{
	/*VMTHook* hook = new VMTHook();
	hook->initialize(interfaces.swapchain);
	original_function = (PresentType)hook->replace_method(8, (ptr)&Present);
	hook->hook();*/

	if (!Memory::GetModuleBaseAddress("IGO64.dll"))
	{
		MessageBoxA(NULL, "PresentHook: Failed to find module. Please enable Origin overlay.", NULL, NULL);
		return false;
	}

	typedef signed __int64(__fastcall* FnOverlayHook)(PVOID oldfunc, PVOID newfunc, PVOID* original);
	FnOverlayHook OverlayHook = (FnOverlayHook)(Memory::SigScan("40 55 56 57 48 81 EC ? ? ? ? 48 8B 05", "IGO64.dll"));

	DWORD64* clienttable = (DWORD64*)*(DWORD64*)interfaces.swapchain;
	OverlayHook((PVOID)clienttable[8], (PVOID)Present, (PVOID*)&present_original);

	//DWORD64* tracetable = (DWORD64*)*(DWORD64*)interfaces.enginetrace;
	//OverlayHook((PVOID)tracetable[3], (PVOID)TraceRay, (PVOID*)&traceray_original);

	if (!oWindowProc)
		oWindowProc = (WNDPROC)SetWindowLongPtr((HWND)FindWindow("Respawn001", NULL), GWLP_WNDPROC, (LONG_PTR)WindowProc);

	//Hook* hook = new Hook(interfaces.swapchain, 8, &Present);
	//original_function = (PresentType)hook->ReplaceVirtual();

	return true;
}

Hooks hooks;