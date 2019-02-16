#pragma once

// win
#include <Windows.h>
#include <winternl.h>
#include <Xinput.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <comdef.h>
#include <minwindef.h>
#include <winbase.h>
#include <intrin.h>
#include <psapi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")

// stl
#include <vector>
#include <deque>
#include <list>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <clocale>
#include <algorithm>
#include <sstream>
#include <cmath>

extern DWORD dwImageSize;
extern DWORD dwImageBase;

// core
#include "memory.h"
#include "vmt.h"

// sdk
#include "sdk\vector.h"
#include "sdk\vmatrix.h"
#include "sdk\color.h"
#include "sdk\engine.h"
#include "sdk\baseentity.h"
#include "sdk\clientlist.h"
#include "sdk\trace.h"
#include "sdk\client.h"

// lib
#include "imgui/imgui.h"
#include "imgui_impl_dx11/imgui_impl_dx11.h"
#include "renderer/renderer.h"

// cheat
#include "interfaces.h"
#include "vars.h"
#include "radar.h"
#include "visuals.h"
#include "aimbot.h"
#include "menu.h"
#include "hooks.h"