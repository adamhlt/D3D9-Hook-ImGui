#include "VTable.h"

// Dummy device technique: https://guidedhacking.com/threads/get-direct3d9-and-direct3d11-devices-dummy-device-method.11867/
// A private hidden window is used so we never depend on the window of the host process.

namespace
{
	constexpr wchar_t kDummyClassName[] = L"D3D9HookImGuiDummyWindow";

	struct DummyWindow
	{
		HINSTANCE instance = GetModuleHandleW(nullptr);
		HWND handle = nullptr;

		DummyWindow()
		{
			WNDCLASSEXW wc = { sizeof(wc) };
			wc.lpfnWndProc = DefWindowProcW;
			wc.hInstance = instance;
			wc.lpszClassName = kDummyClassName;
			RegisterClassExW(&wc);

			handle = CreateWindowExW(0, kDummyClassName, L"", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, nullptr, nullptr, instance, nullptr);
		}

		~DummyWindow()
		{
			if (handle)
				DestroyWindow(handle);
			UnregisterClassW(kDummyClassName, instance);
		}
	};
}

bool D3D9::GetDeviceVTable(DeviceVTable& table)
{
	DummyWindow window;
	if (!window.handle)
		return false;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
		return false;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = window.handle;
	d3dpp.Windowed = TRUE;

	IDirect3DDevice9* pDummyDevice = nullptr;
	const HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, window.handle, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);
	pD3D->Release();
	if (FAILED(hr) || !pDummyDevice)
		return false;

	const void* const* vtable = *reinterpret_cast<void***>(pDummyDevice);
	for (size_t i = 0; i < table.size(); ++i)
		table[i] = const_cast<void*>(vtable[i]);

	pDummyDevice->Release();
	return true;
}
