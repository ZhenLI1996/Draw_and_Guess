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
	
	void Disconnect();
private:
	char ip[16];
	char name[100], path[200];	// name := name of client; path := path of file
	u_short port;
	WSADATA wsaData;
	sockaddr_in addr;
	SOCKET clt;
	HANDLE ht;	// �����洢�̣߳������ļ���ʱ���������������

	static Chat_Client* self;

	HWND hWnd;
	HANDLE event;

	// pack data
	char value[12];
	CRITICAL_SECTION lock;

	static unsigned long _stdcall PullFromServer(void*);
	static unsigned long _stdcall SendToServer(void*);
	bool quit;

	std::vector<VALUE> V_vec;
	//std::fstream File;

	int DrawLine();

};
