<div align="center">

# D3D9 Hook ImGui

**D3D9 hook with ImGui integration, as a DLL (x86 / x64)**

![C++](https://img.shields.io/badge/language-C%2B%2B-16171C?style=flat-square)
![Windows](https://img.shields.io/badge/platform-Windows-16171C?style=flat-square)
![x86 | x64](https://img.shields.io/badge/arch-x86%20%7C%20x64-16171C?style=flat-square)
[![License](https://img.shields.io/badge/license-GPLv3-16171C?style=flat-square)](LICENSE)

</div>

---

D3D9 Hook ImGui hooks `IDirect3DDevice9::EndScene` and `IDirect3DDevice9::Reset` to draw an [ImGui](https://github.com/ocornut/imgui) menu inside any DirectX 9 process.

The virtual method table is retrieved with the [**dummy device technique**](https://guidedhacking.com/threads/get-direct3d9-and-direct3d11-devices-dummy-device-method.11867/) and hooked with [MS Detours](https://www.microsoft.com/en-us/research/project/detours/). Everything is set up: you only write your menu.

### Example
https://github.com/adamhlt/D3D9-Hook-ImGui/assets/48086737/08665cbd-0f4c-49bc-ae98-9c9fa3bc240b

## Features

**Targets**
- x86 and x64, Debug and Release
- DLL to inject in a DirectX 9 process (same architecture)
- Direct3D 9 headers and libs from the Windows SDK, no legacy DirectX SDK needed

**Hook**
- Window resize and fullscreen changes are handled
- `Insert` shows or hides the menu
- `End` removes the hooks and unloads the DLL
- Unload requested from outside (`FreeLibrary`) is handled

**Customize**
- Write your interface in `DrawWindow` (`src/ui/Menu.cpp`)
- Change the hotkeys in `src/Config.h`
- ImGui sources are kept untouched in `external/imgui/` (git submodule)

## Usage

Add your widgets in `DrawWindow`:

```cpp
void DrawWindow()
{
	ImGui::SetNextWindowSize({ 500, 300 }, ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Menu Window Title", &g_visible))
	{
		ImGui::Text("Draw your menu here.");
	}
	ImGui::End();
}
```

### Hooking another function

Every function of DirectX 9 can be hooked by changing its virtual function index in `src/d3d9/VTable.h` and `src/hooks/Hook.cpp`:

```cpp
oEndScene = reinterpret_cast<tEndScene>(vtable[D3D9::EndScene]);
```

All the function indexes are listed [here](docs/Functions.md).

## Project layout

```
src/
  dllmain.cpp        entry point
  Config.h           hotkeys
  hooks/             Detours wrapper, EndScene / Reset hooks, unload logic
  d3d9/              dummy device -> IDirect3DDevice9 vtable
  input/             window procedure subclassing (ImGui input, hotkeys)
  ui/Menu.cpp        ImGui setup and the menu itself
external/
  imgui/             ImGui (submodule)
  detours/           MS Detours headers and libs
docs/Functions.md    IDirect3DDevice9 vtable indexes
```

## Build

1. Clone with the submodules: `git clone --recurse-submodules https://github.com/adamhlt/D3D9-Hook-ImGui.git` (or `git submodule update --init` in an existing clone).
2. Open `D3D9HookImGui.sln` in Visual Studio.
3. Build in Release (x86 or x64).

- **Output:** `bin/<platform>/<configuration>/D3D9HookImGui.dll`
- **Configuration:** every configuration is already set up with libraries and includes
- **Missing ImGui files:** the submodule is not initialized, run `git submodule update --init`

## Credits

- [**@liyunlong23**](https://github.com/liyunlong23) - Fix crash when changing resolution.

## License

This project is licensed under the [GNU GPLv3](LICENSE).
