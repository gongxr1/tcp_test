#ifndef _MessageHeader_hpp_ 
#define _MessageHeader_hpp_
#include<gmp.h>

enum CMD
{
	
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGINOUT,
	CMD_LOGINOUT_RESULT,
	CMD_NEW_USER_JOIN,
	CMD_ERROR,
	CMD_ELGamal_KEY,
	CMD_ELGamal_KEY_Ya,
	CMD_ELGamal_KEY_PAQ,
	CMD_CLIENT_ID,
	CMD_EXCHANGE_YAB
};
struct DataHeader
{
	DataHeader()
	{
		dataLength = sizeof(DataHeader);
		cmd = CMD_ERROR;
	}
	short dataLength;//数据长度 
	short cmd;//命令
};
//DataPackage
//包体
struct Login : public DataHeader
{
	//DataHeader header;
	Login()
	{
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char PassWord[32];
	char data[932];
};
struct LoginResult : public DataHeader
{
	LoginResult()
	{
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
	char data[992];
};
struct LoginOut : public DataHeader
{
	LoginOut()
	{
		dataLength = sizeof(LoginOut);
		cmd = CMD_LOGINOUT;
	}
	char userName[32];
};
struct LoginOutResult : public DataHeader
{
	LoginOutResult()
	{
		dataLength = sizeof(LoginOutResult);
		cmd = CMD_LOGINOUT_RESULT;
		result = 0;
	}
	int result;
};
//新客户端加入
struct NewUserJoin : public DataHeader
{
	NewUserJoin()
	{
		dataLength = sizeof(NewUserJoin);
		cmd = CMD_NEW_USER_JOIN;
		sock = 0;
	}
	int sock;
};
//服务器互换KEY
struct ELGalamKey : public DataHeader
{
	ELGalamKey()
	{
		dataLength = sizeof(ELGalamKey);
		cmd = CMD_ELGamal_KEY;
	}
	mpz_t Y;//公钥
	mpz_t a;
	mpz_t p;
	mpz_t q;
};
//服务器互换KEY
struct ELGalamKey_Ya : public DataHeader
{
	ELGalamKey_Ya()
	{
		dataLength = sizeof(ELGalamKey_Ya);
		cmd = CMD_ELGamal_KEY_Ya;
	}
	mpz_t Y;//公钥
};
//服务器传给客户端KEY
struct ELGalamKeyPAQ : public DataHeader
{
	ELGalamKeyPAQ()
	{
		dataLength = sizeof(ELGalamKeyPAQ);
		cmd = CMD_ELGamal_KEY_PAQ;
	}
	mpz_t a;
	mpz_t p;
	mpz_t q;
};
struct ClientID : public DataHeader
{
	ClientID()
	{
		dataLength = sizeof(ClientID);
		cmd = CMD_CLIENT_ID;
	
	}
	int ID;
};
struct ExchangeYab : public DataHeader
{
	ExchangeYab()
	{
		dataLength = sizeof(ExchangeYab);
		cmd = CMD_EXCHANGE_YAB;

	}
	int dir;//要赋值给目标客户端的左右，0是左边，Ya,1是右边 Yb, 3是本身的左，4是本身的右
	int id;//要发送给的目标客户端
	mpz_t Y;//公钥
};
#endif //!_MessageHeader_hpp_

