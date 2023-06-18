#include "pch.h"
#include "Hook.h"

HANDLE hThread;

int Initialize(const HMODULE hModule)
{
	Hook::HookDirectX();

	while(!GetAsyncKeyState(VK_END))
	{
		Sleep(1);
	}

	Hook::UnHookDirectX();

	FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(const HINSTANCE hinstDLL, const DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Initialize, hinstDLL, 0, nullptr);
		if (hThread != INVALID_HANDLE_VALUE && hThread != nullptr)
			CloseHandle(hThread);
	}

	if (fdwReason == DLL_PROCESS_DETACH)
	{
		Hook::UnHookDirectX();
		if (hThread != INVALID_HANDLE_VALUE && hThread != nullptr)
		{
			TerminateThread(hThread, 0);
			CloseHandle(hThread);
		}
		FreeLibraryAndExitThread(hinstDLL, 0);
	}

	return TRUE;
}

