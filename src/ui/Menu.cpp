#include "Menu.h"

namespace
{
	bool g_initialized = false;
	bool g_visible = true;

	// ---------------------------------------------------------------------
	// Edit this function to build your own menu.
	// ---------------------------------------------------------------------
	void DrawWindow()
	{
		ImGui::SetNextWindowSize({ 500, 300 }, ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Menu Window Title", &g_visible))
		{
			ImGui::Text("Draw your menu here.");
		}
		ImGui::End();
	}
}

bool Menu::Init(IDirect3DDevice9* pDevice, HWND window)
{
	if (g_initialized)
		return true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.Fonts->AddFontDefault();
	ImGui::StyleColorsDark();

	if (!ImGui_ImplWin32_Init(window))
	{
		ImGui::DestroyContext();
		return false;
	}

	if (!ImGui_ImplDX9_Init(pDevice))
	{
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		return false;
	}

	g_initialized = true;
	return true;
}

void Menu::Shutdown()
{
	if (!g_initialized)
		return;

	g_initialized = false;
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool Menu::IsInitialized() { return g_initialized; }
bool Menu::IsVisible() { return g_visible; }
void Menu::ToggleVisible() { g_visible = !g_visible; }

void Menu::OnDeviceLost()
{
	if (g_initialized)
		ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
	if (g_initialized)
		ImGui_ImplDX9_CreateDeviceObjects();
}

void Menu::Render()
{
	if (!g_initialized)
		return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Draw the software cursor only while the mouse is over the menu.
	ImGui::GetIO().MouseDrawCursor = g_visible && ImGui::GetIO().WantCaptureMouse;

	if (g_visible)
		DrawWindow();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
