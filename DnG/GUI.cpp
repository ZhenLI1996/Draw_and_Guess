#include "GUI.h"
#include "stdafx.h"
#include "Resource.h"
#include "Chat_Client.h"

GUI* GUI::THIS = nullptr;


GUI::GUI(HINSTANCE hInstance, int cmdShow) :
WndWidth(GetSystemMetrics(SM_CXSCREEN)),
WndHeight(GetSystemMetrics(SM_CYSCREEN)),
hInstance(hInstance),
cmdShow(cmdShow),
//client("115.159.49.180", 2225)
client("192.168.20.103", 2225)
{
	THIS = this;
	client.ClientInit();
}


GUI::~GUI()
{
}


int GUI::run() {
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = L"testG";
	wcex.hIconSm = wcex.hIcon = nullptr;

	RegisterClassExW(&wcex);

	glblhInst = hInstance; // 将实例句柄存储在全局变量中


	HWND hWnd = CreateWindow(L"testG", L"GUI_class_test_1", WS_CLIPCHILDREN | WS_POPUP,
		0, 0, WndWidth, WndHeight, nullptr, nullptr, hInstance, nullptr);

	client.SetHWND(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);


	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1));
	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return true;
}



long _stdcall GUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static HDC memdc = 0;
	static HBITMAP hb = 0;
	static HPEN pen = 0;
	switch (msg) {
	case WM_CREATE: {
		THIS->hBTNStart = CreateWindowW(L"BUTTON", L"START", WS_VISIBLE | WS_CHILD,
			500, 50, THIS->WndWidth - 1000, THIS->WndHeight / 2 - 100, hWnd, (HMENU)MENU_BTN_START, THIS->glblhInst, nullptr);
		if (!THIS->hBTNStart) {
			return 1;
		}

		THIS->hBTNExit = CreateWindowW(L"BUTTON", L"EXIT", WS_VISIBLE | WS_CHILD,
			500, THIS->WndHeight / 2 + 50, THIS->WndWidth - 1000, THIS->WndHeight / 2 - 100, hWnd, (HMENU)MENU_BTN_EXIT, THIS->glblhInst, nullptr);
		if (!THIS->hBTNExit){
			return 1;
		}

		
		LOGFONTA logfont = { 0 };
		strcpy_s(logfont.lfFaceName, 15, "Comic Sans MS");
		logfont.lfHeight = 160;
		logfont.lfWidth = 50;
		HFONT hFont = CreateFontIndirectA(&logfont);
		SendMessage(THIS->hBTNStart, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(THIS->hBTNExit, WM_SETFONT, (WPARAM)hFont, 0);
		
		break;
	}
	case WM_COMMAND: {
		switch (wParam) {
		case MENU_BTN_START:
		{	
			ShowWindow(THIS->hBTNStart, SW_HIDE);
			ShowWindow(THIS->hBTNExit, SW_HIDE);

			HDC hdc = GetDC(hWnd);
			RECT rect;
			GetClientRect(hWnd, &rect);

			memdc = CreateCompatibleDC(hdc);
			HBITMAP hmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			ReleaseDC(hWnd, hdc);
			SelectObject(memdc, hmap);

			HBRUSH hBrush = CreateSolidBrush(0xFFFFFF);
			FillRect(memdc, &rect, hBrush);
			DeleteObject(hBrush);

			InvalidateRect(hWnd, &rect, true);

			THIS->hBTNGuess = CreateWindowW(L"BUTTON", L"Guess", WS_VISIBLE | WS_CHILD,
				50, 50, 100, 50, hWnd, (HMENU)MENU_BTN_GUESS, THIS->glblhInst, nullptr);
			if (!THIS->hBTNGuess) {
				return 1;
			}

			pen = CreatePen(PS_SOLID, 5, 0x000000);
			SelectObject(memdc, pen);

			THIS->client.ClientConnect();
		}
			break;
		case MENU_BTN_EXIT:
			PostQuitMessage(0);
			return 0;
			break;
		case MENU_BTN_GUESS:
			DialogBox(THIS->hInstance, MAKEINTRESOURCE(MY_IDD_BOX), hWnd, DialogProc);
			return 0;
			break;
		default:
			break;

		}

		break;
	}
	case WM_LBUTTONDOWN: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		MoveToEx(memdc, x, y, 0);
		THIS->MouseDown = true;
		THIS->client.pack(msg, lParam);
		break;
	}
	case WM_LBUTTONUP:
	{
		THIS->MouseDown = false;
		THIS->client.pack(msg, lParam); 

		break;
	}
	case WM_MOUSEMOVE: {
		if (THIS->MouseDown) {
			THIS->client.pack(msg, lParam);
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			RECT rect = { 0, 0, THIS->WndWidth, THIS->WndHeight};
			LineTo(memdc, x, y);
			MoveToEx(memdc, x, y, 0);
			THIS->P.x = x;
			THIS->P.y = y;

			InvalidateRect(hWnd, &rect, false);
			//goto paint;
		}
		break;
	}
	case WM_KEYDOWN: {
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
	}
	case WM_PAINT: {
	//paint:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, THIS->WndWidth, THIS->WndHeight, memdc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	// my defines:
	case MY_WM_START_LINE: 
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		MoveToEx(memdc, x, y, 0);
		break;
	}
	case MY_WM_SYNC_LINE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		RECT rect = { 0, 0, THIS->WndWidth, THIS->WndHeight };
		LineTo(memdc, x, y);
		MoveToEx(memdc, x, y, 0);
		THIS->P.x = x;
		THIS->P.y = y;

		InvalidateRect(hWnd, &rect, false);
		break;
	}

	default:
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int GUI::CreateButton(HWND hWnd, int BtnCnt) {

	return 0;
}
int GUI::MyWM_COMMAND(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	return 0;
}

// “Guess”框的消息处理程序。
INT_PTR CALLBACK GUI::DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			wchar_t buffer[5] = { 0 };
			GetDlgItemTextW(hDlg, MY_IDD_BOX_EDIT1, buffer, 5);	// '\0' counts
			//MessageBox(0, buffer, 0, MB_OK);
			if (!wcslen(buffer)) {
				MessageBoxA(0, "Please Enter Your Guess String!", 0, MB_OK);
				return (INT_PTR)FALSE;
			}
			THIS->client.guess(buffer);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
