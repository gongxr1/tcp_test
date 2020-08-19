#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>

#include "MessageHeader.hpp"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Server
{

public:
	Server();
	~Server();
	// 加载套接字库
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	SOCKET ListenSocket;// incoming connection requests.

	int ServerInit(int port);
	int Listen();
	int CloseSever();
private:
	std::vector<SOCKET> g_clientList;	// 客户端套接字列表
	int processor(SOCKET sock);


};

