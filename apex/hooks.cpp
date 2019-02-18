#include "apex.h"

using PresentType = long(__stdcall*)(IDXGISwapChain*, UINT, UINT);
PresentType present_original;

DWORD64* dwOriginalSwapchain = 0x0;
#define INTERFACE_LENGTH (208 + 150)
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
				ImGui_ImplDX11_Init((HWND)FindWindow(XorStr("Respawn001"), NULL), device, context);
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

bool HookPresent(IDXGISwapChain* pSwapChain)
{
	DWORD64 myFunc = (DWORD64)Present;
	DWORD64* engineInterface = (DWORD64*)*(DWORD64*)pSwapChain;

	if (engineInterface[8] != myFunc)
	{
		static DWORD64 myEngineInterface[INTERFACE_LENGTH];

		DWORD64* engineInterface = (DWORD64*)*(DWORD64*)pSwapChain;
		if (engineInterface && engineInterface != NULL)
			dwOriginalSwapchain = engineInterface;

		Memory::myMemCpy(&myEngineInterface, engineInterface, sizeof(myEngineInterface));
		*(DWORD64*)pSwapChain = (DWORD64)&myEngineInterface;

		present_original = (PresentType)myEngineInterface[8];
		myEngineInterface[8] = (DWORD64)myFunc;

		return true;
	}

	return false;
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
	HookPresent(interfaces.swapchain);

	if (!oWindowProc)
		oWindowProc = (WNDPROC)SetWindowLongPtr((HWND)FindWindow(XorStr("Respawn001"), NULL), GWLP_WNDPROC, (LONG_PTR)WindowProc);

	return true;
}

Hooks hooks;