#include "Hook.h"
#include "Drawing.h"

LPDIRECT3DDEVICE9 Hook::pDevice = nullptr; // Direct3D9 Device Object
tEndScene Hook::oEndScene = nullptr; // Pointer of the original EndScene function
tReset Hook::oReset = nullptr; // Pointer of the original Reset function
HWND Hook::window = nullptr; // Window of the current process
HMODULE Hook::hDDLModule = nullptr; // HMODULE of the DLL

int Hook::windowHeight = 0; // Height of the window
int Hook::windowWidth = 0; // Width of the window
void* Hook::d3d9Device[119]; // Array of pointer of the DirectX functions.
WNDPROC Hook::OWndProc = nullptr; // Pointer of the original window message handler.

/**
    @brief : Function that hook the Reset and EndScene function.
**/
void Hook::HookDirectX()
{
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
	{
		oEndScene = (tEndScene)d3d9Device[42];
		oReset = (tReset)d3d9Device[16];
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)oEndScene, Drawing::hkEndScene);
		DetourAttach(&(PVOID&)oReset, hkReset);
		DetourTransactionCommit();
	}
}

/**
    @brief : Function that unhook Reset and EnScene functions, and also disable the window message handler.
**/
void Hook::UnHookDirectX()
{
	if (Drawing::bInit)
	{
		UnHookWindow();
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	Drawing::bInit = FALSE;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)oEndScene, Drawing::hkEndScene);
	DetourDetach(&(PVOID&)oReset, hkReset);
	DetourTransactionCommit();
}

/**
    @brief : Function that enumerate windows.
**/
BOOL CALLBACK Hook::enumWind(const HWND handle, LPARAM lp)
{
	DWORD procID;
	GetWindowThreadProcessId(handle, &procID);
	if (GetCurrentProcessId() != procID)
		return TRUE;

	window = handle;
	return FALSE;
}

/**
    @brief : Function that retrieve the window of the current process.
    @retval : window of the current process.
**/
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

/**
    @brief : Function that get a Direct3D9 Device Object. (https://guidedhacking.com/threads/get-direct3d9-and-direct3d11-devices-dummy-device-method.11867/)
    @param  pTable : Array of functions pointer.
    @param  size   : Size of the array of pointers.
    @retval : True if the function succeed else False.
**/
BOOL Hook::GetD3D9Device(void** pTable, const size_t size)
{
	if (!pTable)
		return FALSE;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
		return FALSE;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = (GetWindowLongPtr(sd.OutputWindow, GWL_STYLE) & WS_POPUP) != 0 ? FALSE : TRUE;
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

/**
	@brief : Function that setup the WndProc callback function.
**/
void Hook::HookWindow()
{
	OWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
}


/**
    @brief : Function that disable the WndProc callback function.
**/
void Hook::UnHookWindow()
{
	SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)OWndProc);
}

/**
    @brief : A callback function, which you define in your application, that processes messages sent to a window. (https://learn.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-wndproc)
    @param  hWnd : A handle to the window.
    @param  msg : The message.
    @param  wParam : Additional message information.
    @param  lParam : Additional message information.
    @retval : The return value is the result of the message processing, and depends on the message sent.
**/
LRESULT WINAPI Hook::WndProc(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
	if (Drawing::bDisplay && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
		return true;	
	}

	if(Drawing::bInit)
		ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

	if (msg == WM_CLOSE)
	{
		UnHookDirectX();
		UnHookWindow();
		TerminateProcess(GetCurrentProcess(), 0);
	}

	if (ImGui::GetIO().WantCaptureMouse)
	{
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
			return true;
		return false;
	}
	
	return CallWindowProc(OWndProc, hWnd, msg, wParam, lParam);
}

/**
    @brief : Hook of the function IDirect3DDevice9::Reset, to handle resize, etc..
    @param  pPresentationParameters : parameters passed to the original function. 
    @retval : return value of the original function.
**/
HRESULT Hook::hkReset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	Drawing::bSetPos = true;
	UnHookWindow();
	Drawing::bInit = FALSE;
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	pDevice = nullptr;

	return oReset(pPresentationParameters);
}
