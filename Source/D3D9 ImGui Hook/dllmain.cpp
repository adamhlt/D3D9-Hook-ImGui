#include "pch.h"
#include "Hook.h"

int Initialize(const HMODULE hModule)
{
	Hook::HookEndScene();

	while(!GetAsyncKeyState(VK_END))
	{
		Sleep(1);
	}

	Hook::UnHookEndScene();

	FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(const HINSTANCE hinstDLL, const DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		const HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Initialize, hinstDLL, 0, nullptr);
		if (hThread != INVALID_HANDLE_VALUE && hThread != nullptr)
			CloseHandle(hThread);
	}

	return TRUE;
}

