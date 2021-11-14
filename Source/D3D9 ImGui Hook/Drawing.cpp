#include "pch.h"
#include "Drawing.h"

BOOL Drawing::bInit = FALSE;
bool Drawing::bDisplay = true;

HRESULT Drawing::hkEndScene(const LPDIRECT3DDEVICE9 D3D9Device)
{
	if (!Hook::pDevice)
		Hook::pDevice = D3D9Device;

	if (!bInit)
		InitImGui(D3D9Device);

	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDisplay = !bDisplay;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (bDisplay)
		ImGui::ShowDemoWindow(&bDisplay);

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return Hook::oEndScene(D3D9Device);
}

void Drawing::InitImGui(const LPDIRECT3DDEVICE9 pDevice)
{
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
