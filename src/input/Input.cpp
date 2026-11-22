#include "Input.h"
#include "Config.h"
#include "hooks/Hook.h"
#include "ui/Menu.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
	HWND g_window = nullptr;
	WNDPROC g_originalWndProc = nullptr;

	bool IsMouseMessage(const UINT msg)
	{
		return msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST;
	}

	bool IsKeyboardMessage(const UINT msg)
	{
		return msg >= WM_KEYFIRST && msg <= WM_KEYLAST;
	}

	LRESULT CALLBACK WndProc(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
	{
		// Hotkeys (ignore auto-repeat).
		if (msg == WM_KEYDOWN && (lParam & (1 << 30)) == 0)
		{
			if (wParam == Config::kToggleMenuKey)
				Menu::ToggleVisible();
			else if (wParam == Config::kUnloadKey)
				Hook::RequestUnload();
		}

		if (Menu::IsInitialized() && Menu::IsVisible())
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

			// Only swallow the input ImGui wants, everything else reaches the host application.
			const ImGuiIO& io = ImGui::GetIO();
			if ((io.WantCaptureMouse && IsMouseMessage(msg)) || (io.WantCaptureKeyboard && IsKeyboardMessage(msg)))
				return 0;
		}

		return CallWindowProc(g_originalWndProc, hWnd, msg, wParam, lParam);
	}
}

bool Input::Install(const HWND window)
{
	if (g_originalWndProc || !window)
		return false;

	g_window = window;
	g_originalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));
	return g_originalWndProc != nullptr;
}

void Input::Uninstall()
{
	if (!g_originalWndProc)
		return;

	SetWindowLongPtr(g_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_originalWndProc));
	g_originalWndProc = nullptr;
	g_window = nullptr;
}
