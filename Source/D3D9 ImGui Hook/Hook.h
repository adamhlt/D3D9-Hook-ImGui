#ifndef HOOK_H
#define HOOK_H

#include "pch.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
using tEndScene = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice);
using tReset = HRESULT(APIENTRY*)(D3DPRESENT_PARAMETERS* pPresentationParameters);

class Hook
{
public:
	static IDirect3DDevice9* pDevice;
	static tEndScene oEndScene;
	static HWND window;

	static void HookDirectX();
	static void UnHookDirectX();
	static void HookWindow();
	static void UnHookWindow();

private:
	static int windowHeight, windowWidth;
	static void* d3d9Device[119];
	static WNDPROC OWndProc;
	static tReset oReset;


	static BOOL CALLBACK enumWind(HWND handle, LPARAM lp);
	static HWND GetProcessWindow();
	static BOOL GetD3D9Device(void** pTable, size_t size);
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static HRESULT APIENTRY hkReset(D3DPRESENT_PARAMETERS* pPresentationParameters);
};

#endif
