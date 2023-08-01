#include "pch.h"
#include "Drawing.h"
#include "Hook.h"

BOOL Drawing::bInit = FALSE;
bool Drawing::bDisplay = true;
bool Drawing::bSetPos = false;
ImVec2 Drawing::vWindowPos = { 0, 0 };
ImVec2 Drawing::vWindowSize = { 0, 0 };

HRESULT Drawing::hkEndScene(const LPDIRECT3DDEVICE9 D3D9Device)
{
	if (!Hook::pDevice)
		Hook::pDevice = D3D9Device;

	if (!bInit)
		InitImGui(D3D9Device);

	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDisplay = !bDisplay;

	if (GetAsyncKeyState(VK_END) & 1)
	{
		Hook::UnHookDirectX();
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, Hook::hDDLModule, 0, nullptr);
		return Hook::oEndScene(D3D9Device);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (bDisplay)
	{
		ImGui::Begin("Menu Window Title", &bDisplay);
		{
			ImGui::SetWindowSize({ 500, 300 }, ImGuiCond_Once);

			if (vWindowPos.x != 0.0f && vWindowPos.y != 0.0f && vWindowSize.x != 0.0f && vWindowSize.y != 0.0f && bSetPos)
			{
				ImGui::SetWindowPos(vWindowPos);
				ImGui::SetWindowSize(vWindowSize);
				bSetPos = false;
			}

			if (bSetPos == false)
			{
				vWindowPos = {ImGui::GetWindowPos().x, ImGui::GetWindowPos().y};
				vWindowSize = {ImGui::GetWindowSize().x, ImGui::GetWindowSize().y};
			}

			ImGui::Text("Draw your menu here.");
		}
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return Hook::oEndScene(D3D9Device);
}

void Drawing::InitImGui(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DDEVICE_CREATION_PARAMETERS CP;
	pDevice->GetCreationParameters(&CP);
	Hook::window = CP.hFocusWindow;
	Hook::HookWindow();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;
	io.Fonts->AddFontDefault();

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(Hook::window);
	ImGui_ImplDX9_Init(pDevice);

	bInit = TRUE;
}

void Drawing::DrawFilledRect(const int x, const int y, const int w, const int h, const D3DCOLOR color)
{
	const D3DRECT rect = { x,y,x + w,y + h };
	Hook::pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}
