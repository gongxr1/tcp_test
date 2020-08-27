#include "Server.h"

Server::Server()
{

}

Server::~Server()
{

}

int Server::processor(SOCKET sock)
{
	// ������(4096�ֽ�)
	char szRecv[4096*2] = {};
	// 5�����տͻ��˵�����
	// �Ƚ�����Ϣͷ
	int recvLen = recv(sock, szRecv, sizeof(DataHeader), 0);
	DataHeader *pHeader = (DataHeader*)szRecv;
	if (recvLen <= 0)
	{
		printf("�ͻ���<Socket=%d>���˳����������...", sock);
		return -1;
	}

	// 6����������
	switch (pHeader->cmd)
	{
	case CMD_LOGIN:
	{
		Login *login = (Login*)szRecv;

		recv(sock, szRecv + sizeof(DataHeader), pHeader->dataLength - sizeof(DataHeader), 0);
		printf("�յ��ͻ���<Socket=%d>����CMD_LOGIN, ���ݳ��ȣ�%d, userName��%s Password�� %s\n",
			sock, login->dataLength, login->userName, login->PassWord);
		// �����ж��û����������Ƿ���ȷ�Ĺ���
		LoginResult ret;
		send(sock, (char*)&ret, sizeof(LoginResult), 0);
	}
	break;
	case CMD_LOGINOUT:
	{
		LoginOut *logout = (LoginOut*)szRecv;

		recv(sock, szRecv + sizeof(DataHeader), pHeader->dataLength - sizeof(DataHeader), 0);
		printf("�յ��ͻ���<Socket=%d>����CMD_LOGOUT, ���ݳ��ȣ�%d, userName��%s\n",
			sock, logout->dataLength, logout->userName);
		LoginOutResult ret;
		send(sock, (char*)&ret, sizeof(LoginOutResult), 0);
	}
	break;
	case CMD_ELGamal_KEY:
	{
		ELGalamKey *ELGamal = (ELGalamKey*)szRecv;

		recv(sock, szRecv + sizeof(DataHeader), pHeader->dataLength - sizeof(DataHeader), 0);
		printf("�յ��ͻ���<Socket=%d>����CMD_ELGamal_KEY, ���ݳ��ȣ�%d, ��Կ��%s\n",
			sock, ELGamal->dataLength, ELGamal->Y);

		mpz_init_set(server_ELGamal->Y_other, ELGamal->Y);
		mpz_init_set(server_ELGamal->q, ELGamal->q);
		mpz_init_set(server_ELGamal->a, ELGamal->a);
		mpz_init_set(server_ELGamal->p, ELGamal->p);


		server_ELGamal->key_generation_easy();

		key_on = 1;
	/*	LoginOutResult ret;
		send(sock, (char*)&ret, sizeof(LoginOutResult), 0);*/
	}
	break;
	case CMD_ELGamal_KEY_Ya:
	{
		ELGalamKey *ELGamal = (ELGalamKey*)szRecv;

		recv(sock, szRecv + sizeof(DataHeader), pHeader->dataLength - sizeof(DataHeader), 0);
		printf("�յ��ͻ���<Socket=%d>����CMD_ELGamal_KEY_Ya, ���ݳ��ȣ�%d, ��Կ��%s\n",
			sock, ELGamal->dataLength, ELGamal->Y);

		mpz_init_set(server_ELGamal->Y_other, ELGamal->Y);

		key_on = 1;
		/*	LoginOutResult ret;
			send(sock, (char*)&ret, sizeof(LoginOutResult), 0);*/
	}
	break;
	case CMD_CLIENT_ID:
	{
		ClientID *clientid = (ClientID*)szRecv;

		recv(sock, szRecv + sizeof(DataHeader), pHeader->dataLength - sizeof(DataHeader), 0);
		printf("�յ��ͻ���<Socket=%d>����ClientID, ���ݳ��ȣ�%d, ID: %d\n",
			sock, clientid->dataLength, clientid->ID);
		
		id_clientList[clientid->ID] = sock;
	}
	break;
	case CMD_EXCHANGE_YAB:
	{
		ExchangeYab *exchangeYab = (ExchangeYab*)szRecv;

		recv(sock, szRecv + sizeof(DataHeader), pHeader->dataLength - sizeof(DataHeader), 0);
		printf("�յ��ͻ���<Socket=%d>����CMD_EXCHANGE_YAB, ���ݳ��ȣ�%d, id: %d\n",
			sock, exchangeYab->dataLength, exchangeYab->id);
		if (exchangeYab->dir == 3)
		{
			mpz_init_set(server_ELGamal->Ya_other, exchangeYab->Y);
			server_ELGamal->key_generation_end_a();
			cout << "Ya is OK\n\n";
		}
		else if (exchangeYab->dir == 4)
		{
			mpz_init_set(server_ELGamal->Yb_other, exchangeYab->Y);
			server_ELGamal->key_generation_end_b();
			cout << "Yb is OK\n\n";
		}
		else
		{
			ExchangeYab exchangeyab;
			exchangeyab.dir = exchangeYab->dir;
			exchangeyab.id = exchangeYab->id;
			mpz_init_set(exchangeyab.Y, exchangeYab->Y);

			send(id_clientList[exchangeYab->id], (const char*)&exchangeyab, sizeof(ExchangeYab), 0);
		}

		//id_clientList[clientid->ID] = sock;
	}
	break;
	default:
	{
		DataHeader header = {};
		send(sock, (char*)&header, sizeof(header), 0);
		break;
	}
	}

	return 0;
}

int Server::ServerInit(int port)
{
	wVersionRequested = MAKEWORD(2, 2);
	// ����Windows Socket 2.x����
	err = WSAStartup(wVersionRequested, &wsaData);

	// ʹ��Socket API�������׵�TCP�����
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return 1;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return 1;
	}

	/* The WinSock DLL is acceptable. Proceed. */
	 //----------------------
   // Create a SOCKET for listening for
   // incoming connection requests.

	/*SOCKET ListenSocket;*/
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in service;
	service.sin_family = AF_INET;
	//service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(port);

	if (bind(ListenSocket,
		(SOCKADDR*)&service,
		sizeof(service)) == SOCKET_ERROR) {
		printf("bind() failed.������˿�ʧ��\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("������˿ڳɹ�...\n");
	}

	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, 5) == SOCKET_ERROR) {
		printf("���󣬼�������˿�ʧ��...\n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("��������˿ڳɹ�...\n");
	}

	printf("�ȴ��ͻ�������...\n");

	

	//for (int n = (int)g_clientList.size() - 1; n >= 0; n--)
	//{
	//	closesocket(g_clientList[n]);
	//}

	//// 8.�ر��׽���
	//closesocket(ListenSocket);
	//// 9.���Windows Socket����
	//WSACleanup();

	//printf("��������˳����������\n");

	//getchar();

	//return 0;
}

int Server::ServerELGamalInit()
{
	server_ELGamal = new ELGamal();
	//server_ELGamal->elgamal_init();
	return 0;
}


int Server::Listen() 
{

	/*while (true)
	{*/
		// Berkeley sockets
		fd_set readfds;			// ������(socket)����
		fd_set writefds;
		fd_set exceptfds;

		// ������
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);

		// ��������(socket)���뼯��
		FD_SET(ListenSocket, &readfds);
		FD_SET(ListenSocket, &writefds);
		FD_SET(ListenSocket, &exceptfds);

		for (int n = (int)g_clientList.size() - 1; n >= 0; n--)
		{
		FD_SET(g_clientList[n], &readfds);
		}

		// ���ó�ʱʱ�� select ������
		timeval timeout = { 1, 0 };

		// nfds��һ������ֵ����ָfd_set����������������(socket)�ķ�Χ������������
		// ���������ļ����������ֵ+1 ��Windows�������������д0
		//int ret = select(ListenSocket + 1, &readfds, &writefds, &exceptfds, NULL);
		int ret = select(ListenSocket + 1, &readfds, &writefds, &exceptfds, &timeout);
		if (ret < 0)
		{
			printf("select�������,called failed:%d!\n", WSAGetLastError());
			//break;
			return -1;
		}

		// �Ƿ������ݿɶ�
		// �ж�������(socket)�Ƿ��ڼ�����
		if (FD_ISSET(ListenSocket, &readfds))
		{
			//FD_CLR(ListenSocket, &readfds);

			// Create a SOCKET for accepting incoming requests.
			// 4. accept �ȴ����ܿͻ�������
			SOCKADDR_IN clientAddr = {};
			int nAddrLen = sizeof(SOCKADDR_IN);
			SOCKET ClientSocket = INVALID_SOCKET;
			ClientSocket = accept(ListenSocket, (SOCKADDR*)&clientAddr, &nAddrLen);
			if (INVALID_SOCKET == ClientSocket) {
				printf("accept() failed: %d,���յ���Ч�ͻ���Socket\n", WSAGetLastError());
				return 1;
			}
			else
			{
				// ���µĿͻ��˼��룬��֮ǰ�����пͻ���Ⱥ����Ϣ
				for (int n = (int)g_clientList.size() - 1; n >= 0; n--)
				{
					NewUserJoin userJoin;
					//send(g_clientList[n], (const char*)&userJoin, sizeof(NewUserJoin), 0);
				}

				g_clientList.push_back(ClientSocket);
				// �ͻ������ӳɹ�������ʾ�ͻ������ӵ�IP��ַ�Ͷ˿ں�
				printf("�¿ͻ���<Sokcet=%d>����,Ip��ַ��%s,�˿ںţ�%d\n", ClientSocket, inet_ntoa(clientAddr.sin_addr),
					ntohs(clientAddr.sin_port));
			}
		}

		for (int i = 0; i < (int)readfds.fd_count - 1; ++i)
		{
			if (-1 == processor(readfds.fd_array[i]))
			{
				auto iter = std::find(g_clientList.begin(), g_clientList.end(),
					readfds.fd_array[i]);
				if (iter != g_clientList.end())
				{
					g_clientList.erase(iter);
				}
			}
		}

		//printf("����ʱ�䴦������ҵ��...\n");
	//}

}

int Server::CloseSever()
{
	for (int n = (int)g_clientList.size() - 1; n >= 0; n--)
	{
		closesocket(g_clientList[n]);
	}

	// 8.�ر��׽���
	closesocket(ListenSocket);
	// 9.���Windows Socket����
	WSACleanup();

	printf("��������˳����������\n");

	getchar();

	return 0;
}

void Server::ClientInit(const char* ip, unsigned short port)//�������������ݵ���һ������
{
	client = new EasyTcpClient();

	client->Connect(ip, port);

	
}

void Server::ClientSendMsg()//�������������ݵ���һ������
{
	/*Login login;
	strcpy(login.userName, "123");
	strcpy(login.PassWord, "1234321");*/
	Login login;
	strcpy(login.userName, "zjj");
	strcpy(login.PassWord, "969513");

	client->SendData(&login);

}

void Server::SendAll_paq()
{
	ELGalamKeyPAQ paq;
	mpz_init_set(paq.p, server_ELGamal->p);
	mpz_init_set(paq.a, server_ELGamal->a);
	mpz_init_set(paq.q, server_ELGamal->q);
	// ��֮ǰ�����пͻ���Ⱥ����Ϣ
	for (int n = (int)g_clientList.size() - 1; n >= 0; n--)
	{
		//Sleep(1);
		send(g_clientList[n], (const char*)&paq, sizeof(ELGalamKeyPAQ), 0);
	}
	//LoginResult ret;
	//for (int n = 0; n < 102; n++)
	//{
	//	if(id_clientList[n]!=0)
	//	
	//	send(id_clientList[n], (char*)&ret, sizeof(LoginResult), 0);
	//	//send(id_clientList[n], (const char*)&paq, sizeof(ELGalamKeyPAQ), 0);
	//}
}