#include "pch.h"
#include "Hook.h"

LPDIRECT3DDEVICE9 Hook::pDevice = nullptr;
tEndScene Hook::oEndScene = nullptr;
HWND Hook::window = nullptr;

int Hook::windowHeight = 0;
int Hook::windowWidth = 0;
void* Hook::d3d9Device[119];
WNDPROC Hook::OWndProc = nullptr;

void Hook::HookEndScene()
{
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
	{
		oEndScene = (tEndScene)d3d9Device[42];
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)oEndScene, Drawing::hkEndScene);
		DetourTransactionCommit();
	}
}

void Hook::UnHookEndScene()
{
	Drawing::unhkEndScene();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)oEndScene, Drawing:: hkEndScene);
	DetourTransactionCommit();
}


BOOL CALLBACK Hook::enumWind(const HWND handle, LPARAM lp)
{
	DWORD procID;
	GetWindowThreadProcessId(handle, &procID);
	if (GetCurrentProcessId() != procID)
		return TRUE;

	window = handle;
	return FALSE;
}

HWND Hook::GetProcessWindow()
{
	window = nullptr;

	EnumWindows(enumWind, NULL);

	RECT size;
	if (window == nullptr)
		return nullptr;

	GetWindowRect(window, &size);

	windowWidth = size.right - size.left;
	windowHeight = size.bottom - size.top;

	windowHeight -= 29;
	windowWidth -= 5;

	return window;
}

BOOL Hook::GetD3D9Device(void** pTable, const size_t size)
{
	if (!pTable)
		return FALSE;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
		return FALSE;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	if (HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice); dummyDevCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;
		dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK)
		{
			pD3D->Release();
			return FALSE;
		}
	}

	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	return TRUE;
}

void Hook::HookWindow()
{
	OWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
}

void Hook::UnHookWindow()
{
	(WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)OWndProc); // https://guidedhacking.com/threads/imgui-unhooking-crashes.16760/post-104566
}

LRESULT WINAPI Hook::WndProc(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
	if (Drawing::bDisplay && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		ImGui::GetIO().MouseDrawCursor = Drawing::bDisplay;
		ImGui::GetIO().WantCaptureMouse = Drawing::bDisplay;
		return true;	
	}

	if(Drawing::bInit)
	{
		ImGui::GetIO().MouseDrawCursor = Drawing::bDisplay;
		ImGui::GetIO().WantCaptureMouse = Drawing::bDisplay;
	}
	
	return CallWindowProc(OWndProc, hWnd, msg, wParam, lParam);
}