#pragma once
#include "stdafx.h"
#include "Chat_Client.h"

#define MAX_LOADSTRING 100

#define MENU_BTN_START 991
#define MENU_BTN_EXIT 992
#define MENU_BTN_GUESS 993

class GUI
{
public:
	GUI(HINSTANCE hInstance, int cmdShow);
	GUI(const GUI& G) = delete;
	~GUI();

	int run();



private:
// functions:
	static long _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	// create button
	int CreateButton(HWND hWnd, int BtnCnt);
	int MyWM_COMMAND(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// data:
	// client socket
	Chat_Client client;

	// from WinMain
	HINSTANCE hInstance;
	//HINSTANCE hPrev;
	//LPSTR cmd;
	int cmdShow;
	
	// data of window and mouse
	int WndWidth;
	int WndHeight;
	POINT P;
	bool MouseDown;

	// HANDLE of buttons
	HWND hBTNStart;
	HWND hBTNExit;
	HWND hBTNGuess;

	// other 
	MSG msg;
	HINSTANCE glblhInst;
	WCHAR MainWndTitle[MAX_LOADSTRING];                  // 标题栏文本
	WCHAR MainWndClass[MAX_LOADSTRING];            // 主窗口类名


	// to cater for WndProc
	static GUI* THIS;
};

