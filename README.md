```
                        ____ _____ ____  ____     __  __            __      ____          ______      _                      
                       / __ \__  // __ \/ __ \   / / / /___  ____  / /__   /  _/___ ___  / ____/_  __(_)                     
                      / / / //_ </ / / / /_/ /  / /_/ / __ \/ __ \/ //_/   / // __ `__ \/ / __/ / / / /                      
                     / /_/ /__/ / /_/ /\__, /  / __  / /_/ / /_/ / ,<    _/ // / / / / / /_/ / /_/ / /                       
                    /_____/____/_____//____/  /_/ /_/\____/\____/_/|_|  /___/_/ /_/ /_/\____/\__,_/_/                        
                                                                                                                             
                                                                                                                             
                                        D3D9 Hook with ImGui integration (x86 / x64)                                                                                                               
                                           Make DirectX 9 internal menu with ImGui     
```

[![C++](https://img.shields.io/badge/language-C%2B%2B-%23f34b7d.svg?style=for-the-badge&logo=appveyor)](https://en.wikipedia.org/wiki/C%2B%2B) [![Windows](https://img.shields.io/badge/platform-Windows-0078d7.svg?style=for-the-badge&logo=appveyor)](https://en.wikipedia.org/wiki/Microsoft_Windows) [![x86](https://img.shields.io/badge/arch-x86-red.svg?style=for-the-badge&logo=appveyor)](https://en.wikipedia.org/wiki/X86) [![x64](https://img.shields.io/badge/arch-x64-green.svg?style=for-the-badge&logo=appveyor)](https://en.wikipedia.org/wiki/X64)

## :open_book: Project Overview :

D3D9 Hook with [ImGui](https://github.com/ocornut/imgui) integration written in C++, works on x86 and x64 with all librairies and includes.

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

### Other IDE using CMAKE :

This **CMakeLists.txt** should compile the project.

```cmake
cmake_minimum_required(VERSION 3.0)
project(D3D9_HOOK)

set(CMAKE_CXX_STANDARD 17)
include_directories(Detours/include $ENV{DXSDK_DIR}Include)
link_directories(Detours/x86 $ENV{DXSDK_DIR}Lib/x86/)
file(GLOB ImGui ImGui/*.cpp ImGui/*.h)

add_library(D3D9_HOOK SHARED framework.h pch.h dllmain.cpp Hook.h Hook.cpp Drawing.h Drawing.cpp ${ImGui})
target_link_libraries(D3D9_HOOK detours.lib d3d9.lib d3dx9.lib)
```

Tested on CLion with MSVC compiler, you can get Visual Studio Build Tools [**here**](https://visualstudio.microsoft.com/fr/downloads/?q=build+tools).

## :hook: Hooking

You can hook every functions of DirectX 9 by changing the virtual function index (in Hook.cpp) :

Here I hook EndScene Function.

```
oEndScene = (tEndScene)d3d9Device[42];
```

All functions index of DirectX 9 are [**here**](https://github.com/adamhlt/D3D9-Hook-ImGui/blob/main/Functions.md).

## :test_tube: Example

#### Call Of Duty : World at War (x86)

![Exemple x86](https://user-images.githubusercontent.com/48086737/170037506-0e7fa637-b9fd-47fc-93dd-6300510a1bfd.PNG)

#### DirectX 9 Example Program (x64)

![Exemple x64](https://user-images.githubusercontent.com/48086737/170037541-b2bb7793-97d7-46cd-9475-786dc8d3562b.PNG)

