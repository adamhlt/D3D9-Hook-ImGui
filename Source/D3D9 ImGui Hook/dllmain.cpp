#include "pch.h"
#include "Hook.h"

/**
    @brief : Function that init the hook of DirectX and window message handler.
    @param hDLLModule : HMODULE of the current DLL.
**/
void Initialize(const HMODULE hDLLModule)
{
	Hook::hDDLModule = hDLLModule;
	Hook::HookDirectX();
}

BOOL WINAPI DllMain(const HINSTANCE hinstDLL, const DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Initialize, hinstDLL, 0, nullptr);
	}

	if (fdwReason == DLL_PROCESS_DETACH)
	{
		Hook::UnHookDirectX();
		FreeLibrary(hinstDLL);
	}

	return TRUE;
}

