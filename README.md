![Project Banner](https://github.com/adamhlt/D3D9-Hook-ImGui/blob/main/Ressources/D3D9_Hook.png)

# D3D9 Hook ImGui

[![C++](https://img.shields.io/badge/language-C%2B%2B-%23f34b7d.svg?style=for-the-badge&logo=appveyor)](https://en.wikipedia.org/wiki/C%2B%2B) [![Windows](https://img.shields.io/badge/platform-Windows-0078d7.svg?style=for-the-badge&logo=appveyor)](https://en.wikipedia.org/wiki/Microsoft_Windows) [![x86](https://img.shields.io/badge/arch-x86-red.svg?style=for-the-badge&logo=appveyor)](https://en.wikipedia.org/wiki/X86) [![x64](https://img.shields.io/badge/arch-x64-green.svg?style=for-the-badge&logo=appveyor)](https://en.wikipedia.org/wiki/X64)

D3D9 Hook with [ImGui](https://github.com/ocornut/imgui) integration, works on x86 and x64.

## :rocket: Getting Started

This project use [MS Detours](https://github.com/microsoft/Detours) library for hooking and obviously DirectX 9 SDK.

MS Detours library is already include in the project folder, but if you don't have DirectX SDK you can download it [here](https://www.microsoft.com/en-us/download/details.aspx?id=6812).

Make that **DXSDK_DIR** is declared in your environment variables.

To see your environment variables :

> **Settings --> System --> About --> System Advanced Settings --> Environment Variables**

## :hook: Hooking

You can hook every functions of DirectX 9 by changing the virtual function index (in Hook.cpp) :

Here I hook EndScene Function.

```
oEndScene = (tEndScene)d3d9Device[42];
```

All functions index of DirectX 9 are [here](https://github.com/adamhlt/D3D9-Hook-ImGui/blob/main/Ressources/Functions%20Index.md).

## :test_tube: Example

#### Call Of Duty : World at War (x86)

![Call Of Duty : World at War (x86)](https://github.com/adamhlt/D3D9-Hook-ImGui/blob/main/Ressources/Exemplex86.PNG)

#### DirectX 9 Example Program (x64)

![DirectX 9 Example Program (x64)](https://github.com/adamhlt/D3D9-Hook-ImGui/blob/main/Ressources/Exemplex64.PNG)
