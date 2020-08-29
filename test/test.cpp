// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "Server.h"
#include "EasyTcpClient.hpp"
#include "GeneralHashFunctions.h"
#include "ELGamal.h"


#include <gmp.h>

#include <thread>
#include <iostream>
#include <string>



bool g_bRun = true;
int g_bRun_send = 0;
//客户端数量
const int cCount = 100;
//发送线程数量
const int tCount = 4;
//客户端数组
EasyTcpClient* client[cCount];

//服务器对象
Server* 	server_test1;
Server* 	server_test2;
ELGamal*   server_ELGamal1;
ELGamal*   server_ELGamal2;


EasyTcpClient* client1[2];
EasyTcpClient* client2[2];


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
		if(n>=50)
			client[n]->Connect("127.0.0.1", 4568);
		else
			client[n]->Connect("127.0.0.1", 4567);

		printf("Connect = %d\n", n);
	}
	
	Login login;
	strcpy(login.userName, "zjj");
	strcpy(login.PassWord, "969513");

	g_bRun_send += 1;

	while (g_bRun)
	{
		for (int n = begin; n < end; n++)
		{
			//client[n]->SendData(&login);
			client[n]->OnRun();
		}
		

	}

	for (int n = begin; n < end; n++)
	{
		client[n]->Close();
	}
}

void Sever1ClentThread()
{
	Login login;
	strcpy(login.userName, "aaaaaaa");
	strcpy(login.PassWord, "111111");

	while (g_bRun)
	{
		for (int n = cCount; n < cCount+1; n++)
		{
			client1[0]->SendData(&login);
			client1[1]->SendData(&login);

			printf("id=111111");

			//client[n]->OnRun();
		}

		/*Sleep(500);

			client[cCount]->SendData(&login);
			Sleep(500);

			client[cCount + 1]->SendData(&login);*/
	
	}

	
	client1[0]->Close();
	client1[1]->Close();

}


void Sever2ClentThread()
{
	Login login;
	strcpy(login.userName, "aaaaaaa");
	strcpy(login.PassWord, "2222222");

	while (g_bRun)
	{
		for (int n = cCount; n < cCount + 1; n++)
		{
			client2[0]->SendData(&login);
			client2[1]->SendData(&login);

			printf("id=22222");

			//client[n]->OnRun();
		}

		/*Sleep(500);

			client[cCount]->SendData(&login);
			Sleep(500);

			client[cCount + 1]->SendData(&login);*/

	}
	client2[0]->Close();
	client2[1]->Close();

}

void Sever1Thread()
{
	while (g_bRun)
	{
		server_test1->Listen();
	}
	server_test1->CloseSever();
}

void Sever2Thread()
{
	while (g_bRun)
	{
		server_test2->Listen();
	}

	server_test2->CloseSever();
}


void ServerInit()
{
	server_test1 = new Server();
	server_test1->ServerInit(4567);
	thread t2(Sever1Thread);
	t2.detach();
	server_test2 = new Server();
	server_test2->ServerInit(4568);
	thread t3(Sever2Thread);
	t3.detach();

	client1[0] = new EasyTcpClient();
	client1[1] = new EasyTcpClient();
	client1[0]->Connect("127.0.0.1", 4568);
	client1[1]->Connect("127.0.0.1", 4568);

	client2[0] = new EasyTcpClient();
	client2[1] = new EasyTcpClient();
	client2[0]->Connect("127.0.0.1", 4567);
	client2[1]->Connect("127.0.0.1", 4567);

	server_test1->ServerELGamalInit();
	server_test2->ServerELGamalInit();

	server_test1->server_ELGamal->key_generation();

	ELGalamKey server1ELGamaKey;
	mpz_init_set(server1ELGamaKey.Y, server_test1->server_ELGamal->Y);
	mpz_init_set(server1ELGamaKey.p, server_test1->server_ELGamal->p);
	mpz_init_set(server1ELGamaKey.a, server_test1->server_ELGamal->a);
	mpz_init_set(server1ELGamaKey.q, server_test1->server_ELGamal->q);


	client1[0]->SendData(&server1ELGamaKey);
	client1[1]->SendData(&server1ELGamaKey);

	//Sleep(100);

	while (1)
	{
		if (server_test2->key_on)
		{
			ELGalamKey_Ya server2ELGalamKey_Y;
			mpz_init_set(server2ELGalamKey_Y.Y, server_test2->server_ELGamal->Y);

			client2[0]->SendData(&server2ELGalamKey_Y);
			client2[1]->SendData(&server2ELGalamKey_Y);
			break;
		}
	}

	//Sleep(100);

	while (1)
	{
		if (server_test1->key_on)
		{
			server_test1->server_ELGamal->key_generation_end();
			server_test2->server_ELGamal->key_generation_end();
			break;
		}
	}
}
void ClentInit()
{
	/*for (int n = 0; n < cCount; n++)
	{
		client[n] = new EasyTcpClient();
	}
	for (int n = 0; n < cCount/2; n++)
	{
		client[n]->Connect("127.0.0.1", 4567);

	}
	for (int n = cCount / 2; n < cCount; n++)
	{
		client[n]->Connect("127.0.0.1", 4568);
	}*/
	//启动发送线程
	for (int n = 0; n < tCount; n++)
	{
		thread t4(sendThread, n + 1);
		t4.detach();
	}

	Sleep(3000);

	Sleep(3000);

	//客户端识别号，服务器为0和51，最大号101
	ClientID ClientID;

	for (int n = 0; n < cCount/2; n++)
	{
		ClientID.ID = n + 1;
		client[n]->SendData(&ClientID);
	}

	for (int n = cCount / 2; n < cCount; n++)
	{
		ClientID.ID = n + 2;
		client[n]->SendData(&ClientID);
	}
	

	//广播paq到所有客户端，客户端收到后生成左右私钥和公钥
	server_test1->SendAll_paq();
	server_test2->SendAll_paq();

	//两个服务器生成左右私钥和公钥
	server_test1->server_ELGamal->key_generation_easy_a();//左私钥和公钥生成
	server_test1->server_ELGamal->key_generation_easy_b();
	server_test2->server_ELGamal->key_generation_easy_a();
	server_test2->server_ELGamal->key_generation_easy_b();

	Sleep(3000);

	ExchangeYab exchangeYab;

	//编号为0的服务器将左右公钥发送给101号客户端和1号客户端
	exchangeYab.dir = 1;
	exchangeYab.id = 101;//前一个
	mpz_init_set(exchangeYab.Y, server_test1->server_ELGamal->Ya);
	client1[0]->SendData(&exchangeYab);
	client1[1]->SendData(&exchangeYab);

	exchangeYab.dir = 0;
	exchangeYab.id = 1;//前一个
	mpz_init_set(exchangeYab.Y, server_test1->server_ELGamal->Yb);
	client2[0]->SendData(&exchangeYab);
	client2[1]->SendData(&exchangeYab);

	//编号为51的服务器将左右公钥发送给50号客户端和52号客户端
	exchangeYab.dir = 1;
	exchangeYab.id = 50;//前一个
	mpz_init_set(exchangeYab.Y, server_test2->server_ELGamal->Ya);
	client2[0]->SendData(&exchangeYab);
	client2[1]->SendData(&exchangeYab);

	exchangeYab.dir = 0;
	exchangeYab.id = 52;//前一个
	mpz_init_set(exchangeYab.Y, server_test2->server_ELGamal->Yb);
	client1[0]->SendData(&exchangeYab);
	client1[1]->SendData(&exchangeYab);

	//编号为1的客户端将左右公钥发送给0号服务器和2号客户端
	exchangeYab.dir = 4;
	exchangeYab.id = 0;
	mpz_init_set(exchangeYab.Y, client[0]->client_ELGamal->Ya);
	client[0]->SendData(&exchangeYab);

	exchangeYab.dir = 0;
	exchangeYab.id = 2;
	mpz_init_set(exchangeYab.Y, client[0]->client_ELGamal->Yb);
	client[0]->SendData(&exchangeYab);

	//编号为50的客户端将左右公钥发送给49号客户端和51号服务器
	exchangeYab.dir = 1;
	exchangeYab.id = 49;
	mpz_init_set(exchangeYab.Y, client[49]->client_ELGamal->Ya);
	client[49]->SendData(&exchangeYab);

	exchangeYab.dir = 3;
	exchangeYab.id = 51;
	mpz_init_set(exchangeYab.Y, client[49]->client_ELGamal->Yb);
	client1[0]->SendData(&exchangeYab);
	client1[1]->SendData(&exchangeYab);

	//编号为52的客户端将左右公钥发送给51号服务器和53号客户端
	exchangeYab.dir = 4;
	exchangeYab.id = 51;
	mpz_init_set(exchangeYab.Y, client[50]->client_ELGamal->Ya);
	client[50]->SendData(&exchangeYab);

	exchangeYab.dir = 0;
	exchangeYab.id = 53;
	mpz_init_set(exchangeYab.Y, client[50]->client_ELGamal->Yb);
	client[50]->SendData(&exchangeYab);

	//编号为101的客户端将左右公钥发送给100号客户端和0号服务器
	exchangeYab.dir = 1;
	exchangeYab.id = 100;
	mpz_init_set(exchangeYab.Y, client[99]->client_ELGamal->Ya);
	client[99]->SendData(&exchangeYab);

	exchangeYab.dir = 3;
	exchangeYab.id = 0;
	mpz_init_set(exchangeYab.Y, client[99]->client_ELGamal->Yb);
	client2[0]->SendData(&exchangeYab);
	client2[1]->SendData(&exchangeYab);

	//适用于编号为2~49的客户端
	for (int n = 1; n < (cCount / 2)-1; n++)
	{
		exchangeYab.dir = 1;//right
		exchangeYab.id = n;//前一个
		mpz_init_set(exchangeYab.Y, client[n]->client_ELGamal->Ya);
		client[n]->SendData(&exchangeYab);

		exchangeYab.dir = 0;//left
		exchangeYab.id = n+2;//后一个
		mpz_init_set(exchangeYab.Y, client[n]->client_ELGamal->Yb);
		client[n]->SendData(&exchangeYab);

	}
	//适用于编号为53~101的客户端
	for (int n = (cCount / 2)+1; n < cCount-1; n++)
	{
		exchangeYab.dir = 1;//right
		exchangeYab.id = n+1;//前一个
		mpz_init_set(exchangeYab.Y, client[n]->client_ELGamal->Ya);
		client[n]->SendData(&exchangeYab);

		exchangeYab.dir = 0;//left
		exchangeYab.id = n + 3;//后一个
		mpz_init_set(exchangeYab.Y, client[n]->client_ELGamal->Yb);
		client[n]->SendData(&exchangeYab);
	}

	Sleep(4000);

	cout << "服务器0完成后秘钥：" << endl;
	gmp_printf("{p:%Zd,\nXa:%Zd,\nY_a:%Zd,\nY_b:%Zd}\n\n", server_test1->server_ELGamal->p, server_test1->server_ELGamal->Xa, server_test1->server_ELGamal->Y_a, server_test1->server_ELGamal->Y_b);

	for (int i = 0; i < 50; i++)
	{
		cout << "客户端"<<i+1<<"完成后秘钥：" << endl;
		gmp_printf("{p:%Zd,\nXa:%Zd,\nY_a:%Zd,\nY_b:%Zd}\n\n", client[i]->client_ELGamal->p, client[i]->client_ELGamal->Xa, client[i]->client_ELGamal->Y_a, client[i]->client_ELGamal->Y_b);

	}

	cout << "服务器51完成后秘钥：" << endl;
	gmp_printf("{p:%Zd,\nXa:%Zd,\nY_a:%Zd,\nY_b:%Zd}\n\n", server_test2->server_ELGamal->p, server_test2->server_ELGamal->Xa, server_test2->server_ELGamal->Y_a, server_test2->server_ELGamal->Y_b);

	for (int i = 50; i < 100; i++)
	{
		cout << "客户端" << i + 2 << "完成后秘钥：" << endl;
		gmp_printf("{p:%Zd,\nXa:%Zd,\nY_a:%Zd,\nY_b:%Zd}\n\n", client[i]->client_ELGamal->p, client[i]->client_ELGamal->Xa, client[i]->client_ELGamal->Y_a, client[i]->client_ELGamal->Y_b);
	}
}



int main()
{
	/*char Data[2048], Key[2048];
	SHA256_DATA SD256;
	SHA1_DATA SD1;
	printf("Data(2048バイト未満) = ");
	gets_s(Data);
	printf("Key(2048バイト未満) = ");
	gets_s(Key);
	SHA1(&SD1, Data, 0);
	SHA256(&SD256, Data, 0);
	printf("\nSHA1 = %s\n", SD1.Val_String);
	printf("SHA256 = %s\n\n", SD256.Val_String);
	HMAC_SHA1(&SD1, Data, Key, 0);
	HMAC_SHA256(&SD256, Data, Key, 0);
	printf("HMAC-SHA1 = %s\n", SD1.Val_String);
	printf("HMAC-SHA256 = %s\n", SD256.Val_String);*/



	//mpz_t t;	//mpz_t 为GMP内置大数类型
	//mpz_init(t);    //大数t使用前要进行初始化，以便动态分配空间
	//mpz_ui_pow_ui(t, 2, 100);	//GMP所有函数基本都是以mpz打头
	//gmp_printf("2^100=%Zd\n", t);   //输出大数，大数的格式化标志为%Zd
	//mpz_clear(t);
	////scanf_s("%s");


	//const std::string key = "abcdefghijklmnopqrstuvwxyz12345678901";

	//std::cout << "General Purpose Hash Function Algorithms Test" << std::endl;
	//std::cout << "By Arash Partow - 2002        " << std::endl;
	//std::cout << "Key: " << key << std::endl;
	//std::cout << " 1. RS-Hash Function Value:   " << sha1(key) << std::endl;
	//std::cout << " 2. JS-Hash Function Value:   " << JSHash(key) << std::endl;
	//std::cout << " 3. PJW-Hash Function Value:  " << PJWHash(key) << std::endl;
	////std::cout << " 4. ELF-Hash Function Value:  " << ELFHash(key) << std::endl;
	//std::cout << " 5. BKDR-Hash Function Value: " << BKDRHash(key) << std::endl;
	//std::cout << " 6. SDBM-Hash Function Value: " << SDBMHash(key) << std::endl;
	//std::cout << " 7. DJB-Hash Function Value:  " << DJBHash(key) << std::endl;
	//std::cout << " 8. DEK-Hash Function Value:  " << DEKHash(key) << std::endl;
	//std::cout << " 9. FNV-Hash Function Value:  " << FNVHash(key) << std::endl;
	//std::cout << "10. BP-Hash Function Value:   " << BPHash(key) << std::endl;
	//std::cout << "11. AP-Hash Function Value:   " << APHash(key) << std::endl;
	ServerInit();
	ClentInit();


	//exit终止
	thread t1(cmdThread);
	t1.detach();


	//thread t5(Sever1ClentThread);
	//t5.detach();
	//thread t6(Sever2ClentThread);
	//t6.detach();
	//Sleep(3000);

	////启动发送线程
	//for (int n = 0; n < tCount; n++)
	//{
	//	thread t4(sendThread, n + 1);
	//	t4.detach();
	//}

	Sleep(100);

	
	

	//Sleep(100);
	//server_test1.ClientInit("127.0.0.1", 4568);//服务器发送数据到另一服务器
	//Sleep(100);
	//server_test2.ClientInit("127.0.0.1", 4567);//服务器发送数据到另一服务器


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
