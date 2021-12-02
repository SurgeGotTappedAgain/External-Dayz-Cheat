#include "DirectX.h"
#include <thread>

IDirect3D9Ex* p_Object = 0;
IDirect3DDevice9Ex* p_Device = 0;
D3DPRESENT_PARAMETERS p_Params;

ID3DXLine* p_Line;
ID3DXFont* pFontSmall = 0;

extern int Width;
extern int Height;
extern char lWindowName[256];
extern HWND hWnd;
extern char tWindowName[256];
extern HWND tWnd;
extern RECT tSize;
extern MSG Message;
extern bool Debug_Border;

int DirectXInit(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object))) exit(1);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
		exit(1);

	if (!p_Line)
		D3DXCreateLine(p_Device, &p_Line);

	D3DXCreateFont(p_Device, 16, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Tahoma", &pFontSmall);

	return 0;
}

int Render()
{
	while (true)
	{
		p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
		p_Device->BeginScene();

		if (tWnd == GetForegroundWindow())
		{
			Hack();
			Items();

			DrawShadowString("test", 20, 20, 255, 255, 255, pFontSmall);
		}

		p_Device->EndScene();
		p_Device->PresentEx(0, 0, 0, 0, 0);
		return 0;
	}
}