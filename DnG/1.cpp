#include "stdafx.h"
#include "Resource.h"
#include <iostream>
#include <string>

int width = GetSystemMetrics(SM_CXSCREEN);
int height = GetSystemMetrics(SM_CYSCREEN);

POINT p;
bool down;

long _stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	static HDC memdc = 0;
	static HBITMAP hb = 0;
	static HPEN pen = 0;
	switch (msg) {
	case WM_CREATE: {
		HDC hdc = GetDC(hwnd);
		RECT rect;
		GetClientRect(hwnd, &rect);

		memdc = CreateCompatibleDC(hdc);
		HBITMAP hmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		ReleaseDC(hwnd, hdc);
		SelectObject(memdc, hmap);
		InvalidateRect(hwnd, &rect, true);

		pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
		SelectObject(memdc, pen);
		break;
	}
	case WM_LBUTTONDOWN: {
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		MoveToEx(memdc, x, y, 0);
		down = true;
		break;
	}
	case WM_LBUTTONUP:
	{
		down = false;
		break;
	}
	case WM_MOUSEMOVE: {
		if (down) {
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			RECT rect = { 0, 0, width, height };
			LineTo(memdc, x, y);
			MoveToEx(memdc, x, y, 0);
			p.x = x;
			p.y = y;
			
			InvalidateRect(hwnd, &rect, false);
			goto paint;
		}
		break;
	}
	case WM_KEYDOWN: 	{
		switch (wparam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
	}
	case WM_PAINT: {
paint:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		BitBlt(hdc, 0, 0, width, height, memdc, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
		break;
	}
	
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
	return 0;
}

int _stdcall WinMain(HINSTANCE hinstance, HINSTANCE hprev, LPSTR cmd, int cmdshow)
{
	UNREFERENCED_PARAMETER(hprev);
	UNREFERENCED_PARAMETER(cmd);

	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance;
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = L"test";
	wcex.hIconSm = wcex.hIcon = LoadIcon(hinstance, MAKEINTRESOURCEW(IDC_MYICON));

	RegisterClassEx(&wcex);

	HWND hwnd = CreateWindow(L"test", L"test", WS_CLIPCHILDREN | WS_POPUP,
		0, 0, width, height, NULL, NULL, hinstance, NULL);
	//MessageBoxW(hwnd, std::to_wstring(GetLastError()).c_str(), 0, MB_OK);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	MSG msg;
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hinstance, MAKEINTRESOURCE(IDC_MY1));
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}