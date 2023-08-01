#include "pch.h"
#include "Hook.h"

void Initialize() {
    Hook::HookDirectX();
    while (!GetAsyncKeyState(VK_END)) {
        if (GetAsyncKeyState(VK_MENU)) {
            ImGui::SetNextFrameWantCaptureMouse(true);
        }
    }
}

BOOL WINAPI

DllMain(const HINSTANCE hinstDLL, const DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) Initialize, nullptr, 0, nullptr);
    }

    if (fdwReason == DLL_PROCESS_DETACH) {
        Hook::UnHookDirectX();
    }

    return TRUE;
}

