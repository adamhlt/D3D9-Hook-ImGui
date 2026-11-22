#pragma once

#include "pch.h"

namespace Hook
{
	/** @brief : Hook IDirect3DDevice9::EndScene and ::Reset. Can be called from any thread. **/
	bool Install(HMODULE hModule);

	/** @brief : Remove every hook and release ImGui. Idempotent. **/
	void Uninstall();

	/**
	    @brief : Ask for the DLL to unload itself. The actual teardown happens on the render
	             thread (next EndScene) and the library is freed from a helper thread.
	**/
	void RequestUnload();
}
