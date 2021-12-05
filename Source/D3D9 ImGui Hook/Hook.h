#ifndef HOOK_H
#define HOOK_H

#include "pch.h"
#include "Drawing.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Hook
{
public:
	static IDirect3DDevice9* pDevice;
	static tEndScene oEndScene;
	static HWND window;

	static void HookEndScene();
	static void UnHookEndScene();
	static void HookWindow();

private:
	static int windowHeight, windowWidth;
	static void* d3d9Device[119];
	static WNDPROC OWndProc;


	static BOOL CALLBACK enumWind(HWND handle, LPARAM lp);
	static HWND GetProcessWindow();
	static BOOL GetD3D9Device(void** pTable, size_t size);
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif