// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "Server.h"
#include "EasyTcpClient.hpp"
#include "GeneralHashFunctions.h"

#include <thread>
#include <iostream>
#include <string>

bool g_bRun = true;

//客户端数量
const int cCount = 200;
//发送线程数量
const int tCount = 4;
//客户端数组
EasyTcpClient* client[cCount];

//将输入命令分离出来
void cmdThread() {
	while (true)
	{
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit"))
		{
			g_bRun = false;
			printf("退出cmdThread线程\n");
			break;
		}
		else
		{
			printf("不支持的命令\n");
		}
	}
}
void sendThread(int id) //4个线程 1 - 4
{
	//ID 1-4
	int c = cCount / tCount;
	int begin = (id - 1) * c;
	int end = id * c;
	for (int n = begin; n < end; n++)
	{
		client[n] = new EasyTcpClient();
	}
	for (int n = begin; n < end; n++)
	{
		client[n]->Connect("127.0.0.1", 4567);
		printf("Connect = %d\n", n);
	}
	
	Login login;
	strcpy(login.userName, "zjj");
	strcpy(login.PassWord, "969513");

	while (g_bRun)
	{
		for (int n = begin; n < end; n++)
		{
			client[n]->SendData(&login);
			//client[n]->OnRun();
		}
	}

	for (int n = begin; n < end; n++)
	{
		client[n]->Close();
	}
}

void Sever1Thread()
{
	Server 	server_test;

	server_test.ServerInit(4568);

	server_test.ClientInit("127.0.0.1", 4567);//服务器发送数据到另一服务器
	
	server_test.ClientSendMsg();//服务器发送数据到另一服务器

	while (g_bRun)
	{
		server_test.Listen();
	}

	server_test.CloseSever();

}

void Sever2Thread()
{
	Server 	server_test;

	server_test.ServerInit(4567);

	server_test.ClientInit("127.0.0.1", 4568);//服务器发送数据到另一服务器

	server_test.ClientSendMsg();//服务器发送数据到另一服务器

	while (g_bRun)
	{
		server_test.Listen();
	}

	server_test.CloseSever();
}
int main()
{

	const std::string key = "abcdefghijklmnopqrstuvwxyz12345678901";

	std::cout << "General Purpose Hash Function Algorithms Test" << std::endl;
	std::cout << "By Arash Partow - 2002        " << std::endl;
	std::cout << "Key: " << key << std::endl;
	std::cout << " 1. RS-Hash Function Value:   " << sha1(key) << std::endl;
	std::cout << " 2. JS-Hash Function Value:   " << JSHash(key) << std::endl;
	std::cout << " 3. PJW-Hash Function Value:  " << PJWHash(key) << std::endl;
	//std::cout << " 4. ELF-Hash Function Value:  " << ELFHash(key) << std::endl;
	std::cout << " 5. BKDR-Hash Function Value: " << BKDRHash(key) << std::endl;
	std::cout << " 6. SDBM-Hash Function Value: " << SDBMHash(key) << std::endl;
	std::cout << " 7. DJB-Hash Function Value:  " << DJBHash(key) << std::endl;
	std::cout << " 8. DEK-Hash Function Value:  " << DEKHash(key) << std::endl;
	std::cout << " 9. FNV-Hash Function Value:  " << FNVHash(key) << std::endl;
	std::cout << "10. BP-Hash Function Value:   " << BPHash(key) << std::endl;
	std::cout << "11. AP-Hash Function Value:   " << APHash(key) << std::endl;

	//exit终止
	thread t1(cmdThread);
	t1.detach();

	//服务器线程
	thread t2(Sever1Thread);
	t2.detach();
	thread t3(Sever2Thread);
	t3.detach();

	//启动发送线程
	for (int n = 0; n < tCount; n++)
	{
		thread t1(sendThread, n + 1);
		t1.detach();
	}

	


	while (g_bRun)
	{
		Sleep(100);
	}
	printf("已退出.\n");
	return 0;
}





// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
