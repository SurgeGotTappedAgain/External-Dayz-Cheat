#include "Common.h"
#include "Globals.h"
#include <thread>

HANDLE memoryRead = 0, memoryWrite = 0, memoryESPWrite = 0;

int Width = 1920;
int Height = 1080;
char lWindowName[256] = "Spotify";
HWND hWnd;
char tWindowName[256] = "DayZ";
HWND tWnd;
RECT tSize;
MSG Message;
const MARGINS Margin = { 0, 0, Width, Height };


LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		SetWindowPos(tWnd, hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		Render();
		break;

	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, &Margin);
		break;

	case WM_DESTROY:
		PostQuitMessage(1);
		return 0;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}


void Init()
{
	WNDCLASSEX wClass;
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wClass.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wClass.lpfnWndProc = WinProc;
	wClass.lpszClassName = lWindowName;
	wClass.lpszMenuName = lWindowName;
	wClass.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx(&wClass);
	tWnd = FindWindow(0, "DayZ");
	if (tWnd)
	{
		GetWindowRect(tWnd, &tSize);
		Width = tSize.right - tSize.left;
		Height = tSize.bottom - tSize.top;
		hWnd = CreateWindowEx(NULL, lWindowName, lWindowName, WS_POPUP | WS_VISIBLE, 1, 1, Width, Height, 0, 0, 0, 0);

		SetLayeredWindowAttributes(hWnd, 0, 1.0f, LWA_ALPHA);
		SetLayeredWindowAttributes(hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
		SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
		ShowWindow(hWnd, SW_SHOW);
	}

	DirectXInit(hWnd);
}

int main()
{
	Addy();
	Init();


	// overlay stuff
	while (1)
	{
		tWnd = FindWindow(0, tWindowName);

		if (PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Message);
			TranslateMessage(&Message);
		}
	}
}