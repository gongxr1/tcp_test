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

	mpz_t q, a, p;//ȫ�ֲ�����pΪprime number;aΪ primitive root of p;

	mpz_t X;//Xa˽Կ;
	mpz_t Y;//Ya��Կ;��Ҫ����
	mpz_t Y_other;//Ya��Կ;��һ����������
	bool flag_XY = 0;
	mpz_t Y_;//�������ɵĹ�Կ
	bool flag_Y_ = 0;//��Կ����


	mpz_t Xa;//Xa˽Կ;
	mpz_t Ya;//Ya��Կ;��Ҫ����
	mpz_t Ya_other;//Ya��Կ;��һ����������
	bool flag_XaYa = 0;//��Կ����
	mpz_t Y_a;//�������ɵĹ�Կ
	bool flag_Y_a = 0;//��Կ����


	mpz_t Xb;//Xb˽Կ;
	mpz_t Yb;//Yb��Կ;��Ҫ����
	mpz_t Yb_other;//Yb��Կ;��һ����������
	bool flag_XbYb = 0;//��Կ����
	mpz_t Y_b;//�������ɵĹ�Կ
	bool flag_Y_b = 0;//��Կ����


	mpz_t M, K, C1, C2;//����M;
	mpz_t result;//���ܺ���

	int msize = 128;//����

	gmp_randstate_t gmp_state;

	void elgamal_init();
	void key_generation();

	void key_generation_easy();
	void key_generation_easy_a();
	void key_generation_easy_b();

	void key_generation_end();
	void key_generation_end_a();//������Կ����
	void key_generation_end_b();//������Կ����
	void elgamal_encrypt(const char* planttxt);
	void elgamal_decrypt(mpz_t tmp1, mpz_t tmp2);

	void D_H_yxp(mpz_t Yb, mpz_t Xa, mpz_t& K);//����dh��Կ���������չ�����Կ��K=(Yb^Xa)mod p
};

//int main()
//{
//	while (1)
//	{
//		string planttxt;
//		mpz_init(q);//���ڹ�����������
//		mpz_init(a);//����Ԫa
//		mpz_init(p);//ȫ������p
//		mpz_init(Xa);
//		mpz_init(Ya);
//		mpz_init(M);
//		mpz_init(K);
//		mpz_init(C1);
//		mpz_init(C2);
//		mpz_init(result);

//		gmp_randinit_mt(gmp_state);//�������������
//		//����ϵͳʱ������random��seed   
//		gmp_randseed_ui(gmp_state, (unsigned int)(time(NULL)));
//
//		cout << "��������Կ���ȣ�";
//		cin >> msize;
//		cout << endl;

//		key_generation();//��Կ����
//		cout << "ȫ�ֲ�����" << endl;
//		gmp_printf("{%Zd,\n%Zd,\n%Zd}\n\n", p, a, Ya);
//		cout << "���������ģ�" << endl;
//		cin >> planttxt;
//		cout << endl;
//		const char*pt = planttxt.c_str();
//		elgamal_encrypt(pt);//elgamal����
//		gmp_printf("����:\n(%Zd,\n%Zd)\n\n", C1, C2);
//		elgamal_decrypt(C1, C2);//elgamal����
//		cout << "���ܺ�����ģ�" << endl;
//		gmp_printf("%Zd\n", result);
//		mpz_clear(q);
//		mpz_clear(p);
//		mpz_clear(a);
//	}
//	return 0;
//}