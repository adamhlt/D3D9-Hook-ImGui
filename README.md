# D3D9 Hook ImGui
D3D9 Hook with [ImGui](https://github.com/ocornut/imgui) integration, works on x86 and x64.

## Getting Started

This project use [MS Detours](https://github.com/microsoft/Detours) library for hooking and obviously DirectX 9 SDK.

MS Detours library is already include in the project folder, but if you don't have DirectX SDK you can download it [here](https://www.microsoft.com/en-us/download/details.aspx?id=6812).

Make that **DXSDK_DIR** is declared in your environment variables.

To see your environment variables :

> **Settings --> System --> About --> System Advanced Settings --> Environment Variables**

## Hooking

You can hook every functions of DirectX 9 by changing the virtual function index (in Hook.cpp) :

Here I hook EndScene Function.

```
oEndScene = (tEndScene)d3d9Device[42];
```

All functions index of DirectX 9 are here.

## Example

![Call Of Duty : World at War (x86)](https://github.com/adamhlt/D3D9-Hook-ImGui/blob/main/Ressources/Exemple%20x86.PNG)

Call Of Duty : World at War (x86)

![DirectX 9 Example Program (x64)](https://github.com/adamhlt/D3D9-Hook-ImGui/blob/main/Ressources/Exemple%20x64.PNG)

DirectX 9 Example Program (x64)
