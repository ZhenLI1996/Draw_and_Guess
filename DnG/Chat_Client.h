#pragma once
#include "stdafx.h"

struct VALUE {
	char value[12];
};

class Chat_Client {
public:
	Chat_Client(char*, int);
	bool ClientInit();
	bool ClientConnect();
	void SendMsg(const char*);
	bool Close();

	int SetHWND(HWND hWnd);

	// pack function
	bool pack(UINT msg, LPARAM lparam);

	// guess function
	int guess(const wchar_t* str);
	
	void Disconnect();
private:
	char ip[16];
	char name[100], path[200];	// name := name of client; path := path of file
	u_short port;
	WSADATA wsaData;
	sockaddr_in addr;
	SOCKET clt;
	HANDLE ht;	// 用来存储线程，当传文件的时候不能阻塞聊天界面

	static Chat_Client* self;

	HWND hWnd;
	HANDLE event;

	// pack data
	char value[12];
	CRITICAL_SECTION lock;

	static unsigned long _stdcall PullFromServer(void*);
	static unsigned long _stdcall SendToServer(void*);
	bool quit;

	// variables storing data
	std::vector<VALUE> V_vec;
	wchar_t GuessStr[5];

	int DrawLine();
	int GuessResult(bool);

	// status variable
	int status;

};

#define SEND_Empty		0
#define SEND_DrawLine	1
#define SEND_Guess		2