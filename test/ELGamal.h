#pragma once
#include<string>
#include<iostream>
#include<gmp.h>
#include<stdlib.h>
#include<time.h>

using namespace std;

class ELGamal
{
public:
	ELGamal();
	~ELGamal();

	mpz_t q, a, p;//全局参数：p为prime number;a为 primitive root of p;

	mpz_t X;//Xa私钥;
	mpz_t Y;//Ya公钥;需要发送
	mpz_t Y_other;//Ya公钥;另一个服务器的
	bool flag_XY = 0;
	mpz_t Y_;//最终生成的公钥
	bool flag_Y_ = 0;//公钥生成


	mpz_t Xa;//Xa私钥;
	mpz_t Ya;//Ya公钥;需要发送
	mpz_t Ya_other;//Ya公钥;另一个服务器的
	bool flag_XaYa = 0;//公钥生成
	mpz_t Y_a;//最终生成的公钥
	bool flag_Y_a = 0;//公钥生成


	mpz_t Xb;//Xb私钥;
	mpz_t Yb;//Yb公钥;需要发送
	mpz_t Yb_other;//Yb公钥;另一个服务器的
	bool flag_XbYb = 0;//公钥生成
	mpz_t Y_b;//最终生成的公钥
	bool flag_Y_b = 0;//公钥生成


	mpz_t M, K, C1, C2;//明文M;
	mpz_t result;//解密后结果

	int msize = 128;//长度

	gmp_randstate_t gmp_state;

	void elgamal_init();
	void key_generation();

	void key_generation_easy();
	void key_generation_easy_a();
	void key_generation_easy_b();

	void key_generation_end();
	void key_generation_end_a();//最终秘钥生成
	void key_generation_end_b();//最终秘钥生成
	void elgamal_encrypt(const char* planttxt);
	void elgamal_decrypt(mpz_t tmp1, mpz_t tmp2);

	void D_H_yxp(mpz_t Yb, mpz_t Xa, mpz_t& K);//计算dh秘钥交换的最终共享秘钥，K=(Yb^Xa)mod p
};

//int main()
//{
//	while (1)
//	{
//		string planttxt;
//		mpz_init(q);//用于构造特殊素数
//		mpz_init(a);//生成元a
//		mpz_init(p);//全局素数p
//		mpz_init(Xa);
//		mpz_init(Ya);
//		mpz_init(M);
//		mpz_init(K);
//		mpz_init(C1);
//		mpz_init(C2);
//		mpz_init(result);

//		gmp_randinit_mt(gmp_state);//用于随机数生成
//		//根据系统时间设置random的seed   
//		gmp_randseed_ui(gmp_state, (unsigned int)(time(NULL)));
//
//		cout << "请输入密钥长度：";
//		cin >> msize;
//		cout << endl;

//		key_generation();//密钥生成
//		cout << "全局参数：" << endl;
//		gmp_printf("{%Zd,\n%Zd,\n%Zd}\n\n", p, a, Ya);
//		cout << "请输入明文：" << endl;
//		cin >> planttxt;
//		cout << endl;
//		const char*pt = planttxt.c_str();
//		elgamal_encrypt(pt);//elgamal加密
//		gmp_printf("密文:\n(%Zd,\n%Zd)\n\n", C1, C2);
//		elgamal_decrypt(C1, C2);//elgamal解密
//		cout << "解密后的明文：" << endl;
//		gmp_printf("%Zd\n", result);
//		mpz_clear(q);
//		mpz_clear(p);
//		mpz_clear(a);
//	}
//	return 0;
//}