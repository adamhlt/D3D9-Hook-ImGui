#pragma once

#include "pch.h"
#include <array>

namespace D3D9
{
	// Number of methods of IDirect3DDevice9 (IUnknown included). See docs/Functions.md.
	constexpr size_t kDeviceVTableSize = 119;

	// Indexes in the IDirect3DDevice9 virtual method table.
	enum DeviceMethod : size_t
	{
		Reset = 16,
		EndScene = 42,
	};

	using DeviceVTable = std::array<void*, kDeviceVTableSize>;

	/**
	    @brief : Retrieve the virtual method table of IDirect3DDevice9 using a dummy device.
	    @param  table : Receives the function pointers.
	    @retval : True on success.
	**/
	bool GetDeviceVTable(DeviceVTable& table);
}
