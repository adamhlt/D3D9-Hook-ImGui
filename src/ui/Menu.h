#pragma once

#include "pch.h"

namespace Menu
{
	/** @brief : Create the ImGui context and its Win32 / DX9 backends. **/
	bool Init(IDirect3DDevice9* pDevice, HWND window);

	/** @brief : Destroy the ImGui context and backends. Safe to call if not initialized. **/
	void Shutdown();

	bool IsInitialized();
	bool IsVisible();
	void ToggleVisible();

	/** @brief : Release / recreate the device objects around IDirect3DDevice9::Reset. **/
	void OnDeviceLost();
	void OnDeviceReset();

	/** @brief : Build and render one ImGui frame. Call once per EndScene. **/
	void Render();
}
