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
- [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
- [ImGui](https://github.com/ocornut/imgui)

## :rocket: Getting Started

This project use [MS Detours](https://github.com/microsoft/Detours) library for hooking and obviously DirectX 9 SDK.

MS Detours library is already include in the project folder, but if you don't have DirectX SDK you can download it [here](https://www.microsoft.com/en-us/download/details.aspx?id=6812).

> **Note** <br>
> Make sure that **DXSDK_DIR** is declared in your environment variables.

To see your environment variables :

> **Settings --> System --> About --> System Advanced Settings --> Environment Variables**

### Visual Studio :

1. Open the solution file (.sln).
2. Build the project in Release (x86 or x64)

Every configuration in x86 / x64 (Debug and Realese) are already configured with librairies and includes.

## :hook: Hooking

You can hook every functions of DirectX 9 by changing the virtual function index (in Hook.cpp) :

Here I hook EndScene Function.

```
oEndScene = (tEndScene)d3d9Device[42];
```

All functions index of DirectX 9 are [**here**](https://github.com/adamhlt/D3D9-Hook-ImGui/blob/main/Functions.md).

## :test_tube: Example

https://github.com/adamhlt/D3D9-Hook-ImGui/assets/48086737/08665cbd-0f4c-49bc-ae98-9c9fa3bc240b

# Credits

- [**@liyunlong23**](https://github.com/liyunlong23) - Fix crash when changing resolution.
