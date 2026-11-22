#include "Hook.h"
#include "Detour.h"
#include "d3d9/VTable.h"
#include "input/Input.h"
#include "ui/Menu.h"
#include <atomic>

namespace
{
	using tEndScene = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice);
	using tReset = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

	tEndScene oEndScene = nullptr; // Original EndScene (trampoline once hooked).
	tReset oReset = nullptr;       // Original Reset (trampoline once hooked).
	HMODULE g_module = nullptr;

	std::atomic<bool> g_installed = false;
	std::atomic<bool> g_unloadRequested = false;
	std::atomic<bool> g_tornDown = false;

	HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 pDevice);
	HRESULT APIENTRY hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

	/** @brief : Release ImGui and restore the window procedure. Runs once. **/
	void TearDownUi()
	{
		if (g_tornDown.exchange(true))
			return;

		Input::Uninstall();
		Menu::Shutdown();
	}

	/** @brief : Remove the detours. Runs once. **/
	void DetachHooks()
	{
		if (!g_installed.exchange(false))
			return;

		Detour::Transaction transaction;
		transaction.Detach(oEndScene, hkEndScene);
		transaction.Detach(oReset, hkReset);
		transaction.Commit();
	}

	/** @brief : Detach the hooks, give in-flight calls time to leave them, then free the DLL. **/
	DWORD WINAPI UnloadThread(LPVOID)
	{
		DetachHooks();
		Sleep(200);
		FreeLibraryAndExitThread(g_module, 0);
	}

	bool InitUi(IDirect3DDevice9* pDevice)
	{
		D3DDEVICE_CREATION_PARAMETERS params = {};
		if (FAILED(pDevice->GetCreationParameters(&params)) || !params.hFocusWindow)
			return false;

		if (!Menu::Init(pDevice, params.hFocusWindow))
			return false;

		if (!Input::Install(params.hFocusWindow))
		{
			Menu::Shutdown();
			return false;
		}

		return true;
	}

	/**
	    @brief : Hook of IDirect3DDevice9::EndScene.
	    @param  pDevice : Current Direct3D9 Device Object.
	    @retval : Result of the original EndScene function.
	**/
	HRESULT APIENTRY hkEndScene(const LPDIRECT3DDEVICE9 pDevice)
	{
		if (g_unloadRequested)
		{
			// First frame after the request: tear down on the render thread, then free the DLL.
			if (!g_tornDown)
			{
				TearDownUi();
				if (HANDLE thread = CreateThread(nullptr, 0, UnloadThread, nullptr, 0, nullptr))
					CloseHandle(thread);
			}
			return oEndScene(pDevice);
		}

		if (!Menu::IsInitialized() && !InitUi(pDevice))
			return oEndScene(pDevice);

		Menu::Render();
		return oEndScene(pDevice);
	}

	/**
	    @brief : Hook of IDirect3DDevice9::Reset, to handle resize / fullscreen changes.
	    @retval : Result of the original Reset function.
	**/
	HRESULT APIENTRY hkReset(const LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		Menu::OnDeviceLost();
		const HRESULT hr = oReset(pDevice, pPresentationParameters);
		if (SUCCEEDED(hr))
			Menu::OnDeviceReset();
		return hr;
	}
}

bool Hook::Install(const HMODULE hModule)
{
	if (g_installed)
		return true;

	D3D9::DeviceVTable vtable;
	if (!D3D9::GetDeviceVTable(vtable))
		return false;

	g_module = hModule;
	oEndScene = reinterpret_cast<tEndScene>(vtable[D3D9::EndScene]);
	oReset = reinterpret_cast<tReset>(vtable[D3D9::Reset]);

	Detour::Transaction transaction;
	if (!transaction.Attach(oEndScene, hkEndScene) || !transaction.Attach(oReset, hkReset) || !transaction.Commit())
		return false;

	g_installed = true;
	return true;
}

void Hook::Uninstall()
{
	g_unloadRequested = true;
	TearDownUi();
	DetachHooks();
}

void Hook::RequestUnload()
{
	g_unloadRequested = true;
}
