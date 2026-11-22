#include "hooks/Hook.h"

namespace
{
	DWORD WINAPI MainThread(LPVOID lpParam)
	{
		Hook::Install(static_cast<HMODULE>(lpParam));
		return 0;
	}
}

BOOL WINAPI DllMain(const HINSTANCE hinstDLL, const DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		if (HANDLE thread = CreateThread(nullptr, 0, MainThread, hinstDLL, 0, nullptr))
			CloseHandle(thread);
		break;

	case DLL_PROCESS_DETACH:
		// lpReserved != nullptr: the process is terminating, nothing to clean up.
		if (!lpReserved)
			Hook::Uninstall();
		break;
	}

	return TRUE;
}
