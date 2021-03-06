#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>

#include "EasyTcpClient.hpp"
#include "MessageHeader.hpp"
#include "ELGamal.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Server
{

public:
	Server();
	~Server();

	//加密解密
	ELGamal*  server_ELGamal;
	bool key_on = 0;
	// 加载套接字库
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	EasyTcpClient* client;

	void ClientInit(const char* ip, unsigned short port);//服务器发送数据到另一服务器
	void ClientSendMsg();//服务器发送数据到另一服务器


	SOCKET ListenSocket;// incoming connection requests.

	

	int ServerInit(int port);
	int ServerELGamalInit();
	int Listen();
	int CloseSever();

	void SendAll_paq();

private:
	std::vector<SOCKET> g_clientList;	// 客户端套接字列表
	SOCKET id_clientList[102] = {0};	// 客户端套接字数组

	int processor(SOCKET sock);


};

