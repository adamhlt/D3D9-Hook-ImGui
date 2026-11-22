```
            ____ _____ ____  ____     __  __            __      ____          ______      _ 
           / __ \__  // __ \/ __ \   / / / /___  ____  / /__   /  _/___ ___  / ____/_  __(_)
          / / / //_ </ / / / /_/ /  / /_/ / __ \/ __ \/ //_/   / // __ `__ \/ / __/ / / / /
         / /_/ /__/ / /_/ /\__, /  / __  / /_/ / /_/ / ,<    _/ // / / / / / /_/ / /_/ / /
        /_____/____/_____//____/  /_/ /_/\____/\____/_/|_|  /___/_/ /_/ /_/\____/\__,_/_/
                                                                             
                                                                           
                            D3D9 Hook with ImGui integration (x86 / x64)  
                             Make DirectX 9 internal menu with ImGui
                                  (Resize & Fullscreen support)
```
<p align="center">
    <img src="https://img.shields.io/badge/language-C%2B%2B-%23f34b7d.svg?style=for-the-badge&logo=appveyor" alt="C++">
    <img src="https://img.shields.io/badge/platform-Windows-0078d7.svg?style=for-the-badge&logo=appveyor" alt="Windows">
    <img src="https://img.shields.io/badge/arch-x86-red.svg?style=for-the-badge&logo=appveyor" alt="x86">
    <img src="https://img.shields.io/badge/arch-x64-green.svg?style=for-the-badge&logo=appveyor" alt="x64">
</p>

## :open_book: Project Overview :

D3D9 Hook with [ImGui](https://github.com/ocornut/imgui) integration written in C++, works on x86 and x64 with all librairies and includes.

#### Features :

- Handle window resize
- DLL unload itself
- Handler external unload
- Handle fullscreen change

This use the [**dummy device technique**](https://guidedhacking.com/threads/get-direct3d9-and-direct3d11-devices-dummy-device-method.11867/) to retrieve the virtual method table of DirectX 9.

#### Used librairies :

- [MS Detours](https://www.microsoft.com/en-us/research/project/detours/)
- [ImGui](https://github.com/ocornut/imgui)
- Direct3D 9 headers / libs from the Windows SDK (no legacy DirectX SDK needed)

## :rocket: Getting Started

MS Detours is already included in `external/detours`, ImGui is a git submodule :

```
git clone --recurse-submodules https://github.com/adamhlt/D3D9-Hook-ImGui.git
```

(or `git submodule update --init` in an existing clone).

### Visual Studio :

1. Open `D3D9HookImGui.sln`.
2. Build the project in Release (x86 or x64). The DLL is written in `bin/<platform>/<configuration>/`.

Every configuration (x86 / x64, Debug and Release) is already configured with libraries and includes.

### Usage :

Inject the DLL in a DirectX 9 process (same architecture).

- `INSERT` : show / hide the menu
- `END` : unhook and unload the DLL

Keys are configured in `src/Config.h`.

## :file_folder: Project layout

```
src/
  dllmain.cpp        entry point
  Config.h           hotkeys
  hooks/             Detours wrapper + EndScene / Reset hooks, unload logic
  d3d9/              dummy device -> IDirect3DDevice9 vtable
  input/             window procedure subclassing (ImGui input, hotkeys)
  ui/Menu.cpp        ImGui setup and the menu itself  <-- edit DrawWindow() here
external/
  imgui/             submodule
  detours/           MS Detours headers and libs
docs/Functions.md    IDirect3DDevice9 vtable indexes
```

## :hook: Hooking

You can hook every function of DirectX 9 by changing the virtual function index (`src/d3d9/VTable.h`, `src/hooks/Hook.cpp`) :

Here I hook the EndScene function.

```
oEndScene = reinterpret_cast<tEndScene>(vtable[D3D9::EndScene]);
```

All functions index of DirectX 9 are [**here**](docs/Functions.md).

## :test_tube: Example

https://github.com/adamhlt/D3D9-Hook-ImGui/assets/48086737/08665cbd-0f4c-49bc-ae98-9c9fa3bc240b

# Credits

- [**@liyunlong23**](https://github.com/liyunlong23) - Fix crash when changing resolution.
