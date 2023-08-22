#ifndef DRAWING_H
#define DRAWING_H

#include "pch.h"

class Drawing
{
public:
	static bool bDisplay;
	static BOOL bInit;
	static bool bSetPos;

	static HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 D3D9Device);

private:
	static ImVec2 vWindowPos;
	static ImVec2 vWindowSize;

	static void InitImGui(LPDIRECT3DDEVICE9 pDevice);
};

#endif