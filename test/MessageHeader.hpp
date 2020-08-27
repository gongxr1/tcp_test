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
	short dataLength;//���ݳ��� 
	short cmd;//����
};
//DataPackage
//����
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
//�¿ͻ��˼���
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
//����������KEY
struct ELGalamKey : public DataHeader
{
	ELGalamKey()
	{
		dataLength = sizeof(ELGalamKey);
		cmd = CMD_ELGamal_KEY;
	}
	mpz_t Y;//��Կ
	mpz_t a;
	mpz_t p;
	mpz_t q;
};
//����������KEY
struct ELGalamKey_Ya : public DataHeader
{
	ELGalamKey_Ya()
	{
		dataLength = sizeof(ELGalamKey_Ya);
		cmd = CMD_ELGamal_KEY_Ya;
	}
	mpz_t Y;//��Կ
};
//�����������ͻ���KEY
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
	int dir;//Ҫ��ֵ��Ŀ��ͻ��˵����ң�0����ߣ�Ya,1���ұ� Yb, 3�Ǳ������4�Ǳ������
	int id;//Ҫ���͸���Ŀ��ͻ���
	mpz_t Y;//��Կ
};
#endif //!_MessageHeader_hpp_

