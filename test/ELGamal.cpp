#include "ELGamal.h"

ELGamal::ELGamal()
{

}
ELGamal::~ELGamal()
{

}

//��ʼ��
void ELGamal::elgamal_init()
{
	string planttxt;
	mpz_init(q);//���ڹ�����������
	mpz_init(a);//����Ԫa
	mpz_init(p);//ȫ������p
	mpz_init(Xa);
	mpz_init(Ya);
	mpz_init(M);
	mpz_init(K);
	mpz_init(C1);
	mpz_init(C2);
	mpz_init(result);

	gmp_randinit_mt(gmp_state);//�������������
//����ϵͳʱ������random��seed   
	gmp_randseed_ui(gmp_state, (unsigned int)(time(NULL)));
}

//���public key = {p,a,Ya}; private key Xa;

//���ѡ��һ������d��Ϊ��Կ��2��d��p-2 ��
//����y = �� ^ d mod p��ȡyΪ��Կ��
void ELGamal::key_generation() {

	mpz_urandomb(q, gmp_state, msize);//����128bit�������
	mpz_nextprime(q, q);//��������q
	//p=2*q+1,����һ������p��ʹ��p-1������ֻ��1,2,q��p-1�ĸ�
	mpz_mul_ui(p, q, 2);
	mpz_add_ui(p, p, 1);
	//������Ԫ��1��p-1֮��������a���ǵ�λԪ���ײ�Ϊ2��q��
	//�����Ϊp-1������֤����aΪp������Ԫ
	mpz_t a_2, a_q;
	mpz_init(a_2);
	mpz_init(a_q);
	do {
		mpz_urandomm(a, gmp_state, p);//����1��p-1�������
		mpz_powm_ui(a_2, a, 2, p);//a_2=a^2 % p
		mpz_powm(a_q, a, q, p);//a_q=a^q % p
		if ((mpz_cmp_ui(a, 1) != 0) && (mpz_cmp_ui(a_2, 1) != 0) && (mpz_cmp_ui(a_q, 1) != 0))
			break;
	} while (1);
	mpz_urandomm(Xa, gmp_state, p);//����˽ԿXa
	mpz_powm(Ya, a, Xa, p);//���ɹ�ԿYa,Ya=a^Xa % p

}

//�������ģ�������ģ�C1��C2��

//�����ѡȡһ������k��2��k��p - 2��
//C1����^k mod  p��
//C2��MY^k mod  p��
//
//����Ϊ��C1, C2��
void ELGamal::elgamal_encrypt(const char* planttxt) {
	mpz_t k;//�����k
	mpz_init(k);
	mpz_urandomm(k, gmp_state, p);//���������k
	mpz_set_str(M, planttxt, 10);//����M
	mpz_powm(K, Ya, k, p);//K= Ya^k % p
	mpz_powm(C1, a, k, p);//C1= a^k % p
	mpz_mul(C2, K, M);//C2= K*M
	mpz_mod(C2, C2, p);//C2= C2 % p
}

//�������ģ�C1,C2�����������

//M=C2/C1^d mod p
void ELGamal::elgamal_decrypt(mpz_t tmp1, mpz_t tmp2) {
	mpz_t tmpK;
	mpz_init(tmpK);
	mpz_powm(tmpK, tmp1, Xa, p);//tmpK= C1^Xa % p
	mpz_invert(tmpK, tmpK, p);//��tmpK��Ԫ
	mpz_mul(result, tmp2, tmpK);//result= C2*tmpK^-1
	mpz_mod(result, result, p);//result= result % p
}