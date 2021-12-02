#include <Windows.h>
#include <iostream>
#include "DirectX.h"

#pragma warning(disable:4996)

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hSecInstance, LPSTR nCmdLine, INT nCmdShow);
void SetWindowToTarget();