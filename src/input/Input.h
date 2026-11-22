#pragma once

#include "pch.h"

namespace Input
{
	/** @brief : Subclass the window procedure of `window` to feed ImGui and handle the hotkeys. **/
	bool Install(HWND window);

	/** @brief : Restore the original window procedure. **/
	void Uninstall();
}
