#ifndef DRAWING_H
#define DRAWING_H

#include "pch.h"
#include "Hook.h"

class Drawing
{
public:
	static bool bDisplay;
	static BOOL bInit;

	static HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 D3D9Device);
	static void unhkEndScene();

private:
	static void InitImGui(LPDIRECT3DDEVICE9 pDevice);
	static void CloseImGui();
	static void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color);
};

#endif