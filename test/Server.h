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

	//���ܽ���
	ELGamal*  server_ELGamal;
	bool key_on = 0;
	// �����׽��ֿ�
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	EasyTcpClient* client;

	void ClientInit(const char* ip, unsigned short port);//�������������ݵ���һ������
	void ClientSendMsg();//�������������ݵ���һ������


	SOCKET ListenSocket;// incoming connection requests.

	

	int ServerInit(int port);
	int ServerELGamalInit();
	int Listen();
	int CloseSever();

	void SendAll_paq();

private:
	std::vector<SOCKET> g_clientList;	// �ͻ����׽����б�
	SOCKET id_clientList[102] = {0};	// �ͻ����׽�������

	int processor(SOCKET sock);


};

