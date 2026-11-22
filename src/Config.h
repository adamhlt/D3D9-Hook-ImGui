#pragma once

#include "pch.h"

// User-facing settings of the template.
namespace Config
{
	constexpr WPARAM kToggleMenuKey = VK_INSERT; // Show / hide the menu.
	constexpr WPARAM kUnloadKey = VK_END;        // Unhook everything and unload the DLL.
}
