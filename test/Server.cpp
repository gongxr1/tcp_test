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
	char szRecv[4096] = {};
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

int Server::Listen() 
{

	while (true)
	{
		// Berkeley sockets
		fd_set readfds;			// ������(socket)����
		fd_set writefds;
		fd_set exceptfds;

		// ��������
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
		timeval timeout = { 0, 0 };

		// nfds��һ������ֵ����ָfd_set����������������(socket)�ķ�Χ������������
		// ���������ļ����������ֵ+1 ��Windows�������������д0
		//int ret = select(ListenSocket + 1, &readfds, &writefds, &exceptfds, NULL);
		int ret = select(ListenSocket + 1, &readfds, &writefds, &exceptfds, &timeout);
		if (ret < 0)
		{
			printf("select�������,called failed:%d!\n", WSAGetLastError());
			break;
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
					send(g_clientList[n], (const char*)&userJoin, sizeof(NewUserJoin), 0);
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
	}

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