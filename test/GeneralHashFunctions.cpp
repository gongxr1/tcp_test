/*
 **************************************************************************
 *                                                                        *
 *          General Purpose Hash Function Algorithms Library              *
 *                                                                        *
 * Author: Arash Partow - 2002                                            *
 * URL: http://www.partow.net                                             *
 * URL: http://www.partow.net/programming/hashfunctions/index.html        *
 *                                                                        *
 * Copyright notice:                                                      *
 * Free use of the General Purpose Hash Function Algorithms Library is    *
 * permitted under the guidelines and in accordance with the MIT License. *
 * http://www.opensource.org/licenses/MIT                                 *
 *                                                                        *
 **************************************************************************
*/


#include "GeneralHashFunctions.h"


unsigned int RSHash(const std::string& str)
{
   unsigned int b    = 378551;
   unsigned int a    = 63689;
   unsigned int hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = hash * a + str[i];
      a    = a * b;
   }

   return hash;
}
/* End Of RS Hash Function */


unsigned int JSHash(const std::string& str)
{
   unsigned int hash = 1315423911;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash ^= ((hash << 5) + str[i] + (hash >> 2));
   }

   return hash;
}
/* End Of JS Hash Function */


unsigned int PJWHash(const std::string& str)
{
   unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
   unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
   unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
   unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
   unsigned int hash              = 0;
   unsigned int test              = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = (hash << OneEighth) + str[i];

      if((test = hash & HighBits)  != 0)
      {
         hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
      }
   }

   return hash;
}
/* End Of  P. J. Weinberger Hash Function */


//unsigned int ELFHash(const std::string& str)
//{
//   unsigned int hash = 0;
//   unsigned int x    = 0;
//
//   for(std::size_t i = 0; i < str.length(); i++)
//   {
//      hash = (hash << 4) + str[i];
//      if((x = hash & 0xF0000000L) != 0)
//      {
//         hash ^= (x >> 24);
//      }
//      hash &= ~x;
//   }
//
//   return hash;
//}
///* End Of ELF Hash Function */


unsigned int BKDRHash(const std::string& str)
{
   unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
   unsigned int hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = (hash * seed) + str[i];
   }

   return hash;
}
/* End Of BKDR Hash Function */


unsigned int SDBMHash(const std::string& str)
{
   unsigned int hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = str[i] + (hash << 6) + (hash << 16) - hash;
   }

   return hash;
}
/* End Of SDBM Hash Function */


unsigned int DJBHash(const std::string& str)
{
   unsigned int hash = 5381;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = ((hash << 5) + hash) + str[i];
   }

   return hash;
}
/* End Of DJB Hash Function */


unsigned int DEKHash(const std::string& str)
{
   unsigned int hash = static_cast<unsigned int>(str.length());

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = ((hash << 5) ^ (hash >> 27)) ^ str[i];
   }

   return hash;
}
/* End Of DEK Hash Function */


unsigned int BPHash(const std::string& str)
{
   unsigned int hash = 0;
   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = hash << 7 ^ str[i];
   }

   return hash;
}
/* End Of BP Hash Function */


unsigned int FNVHash(const std::string& str)
{
   const unsigned int fnv_prime = 0x811C9DC5;
   unsigned int hash = 0;
   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash *= fnv_prime;
      hash ^= str[i];
   }

   return hash;
}
/* End Of FNV Hash Function */


unsigned int APHash(const std::string& str)
{
   unsigned int hash = 0xAAAAAAAA;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ str[i] * (hash >> 3)) :
                               (~((hash << 11) + (str[i] ^ (hash >> 5))));
   }

   return hash;
}
/* End Of AP Hash Function */

vector<int> X;//8*64=512，每个下标存放8位
int W[80];//32位为一组
int A, B, C, D, E;
int A1, B1, C1, D1, E1;//缓冲区寄存器,产生最后结果
int Turn;//加密分组数量
void printX() {//输出填充后的文本
	for (int i = 0; i < X.size(); i++) {
		printf("%02x", X[i]);
		if ((i + 1) % 4 == 0)
			printf(" ");
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
}
int S(unsigned int x, int n) {//循环左移
	return x >> (32 - n) | (x << n);
}
void append(string m) {//文本的填充处理
	Turn = (m.size() + 8) / 64 + 1;
	X.resize(Turn * 64);
	int i = 0;
	for (; i < m.size(); i++) {
		X[i] = m[i];
	}
	X[i++] = 0x80;
	while (i < X.size() - 8) {
		X[i] = 0;
		i++;
	}
	long long int a = m.size() * 8;
	for (i = X.size() - 1; i >= X.size() - 8; i--) {
		X[i] = a % 256;
		a /= 256;
	}
}
void setW(vector<int> m, int n) {//W数组的生成
	n *= 64;
	for (int i = 0; i < 16; i++) {
		W[i] = (m[n + 4 * i] << 24) + (m[n + 4 * i + 1] << 16)
			+ (m[n + 4 * i + 2] << 8) + m[n + 4 * i + 3];
	}
	for (int i = 16; i < 80; i++) {
		W[i] = S(W[i - 16] ^ W[i - 14] ^ W[i - 8] ^ W[i - 3], 1);
	}
}
int ft(int t) {//ft(B,C,D)函数
	if (t < 20)
		return (B & C) | ((~B) & D);
	else if (t < 40)
		return B ^ C ^ D;
	else if (t < 60)
		return (B & C) | (B & D) | (C & D);
	else
		return B ^ C ^ D;
}
int Kt(int t) {//常量K
	if (t < 20)
		return 0x5a827999;
	else if (t < 40)
		return 0x6ed9eba1;
	else if (t < 60)
		return 0x8f1bbcdc;
	else
		return 0xca62c1d6;
}
unsigned int sha1(const std::string& text) {
	A1 = A = 0x67452301;
	B1 = B = 0xefcdab89;
	C1 = C = 0x98badcfe;
	D1 = D = 0x10325476;
	E1 = E = 0xc3d2e1f0;
	append(text);
	printX();
	for (int i = 0; i < Turn; i++) {
		setW(X, i);
		for (int t = 0; t < 80; t++) {
			int temp = E + ft(t) + S(A, 5) + W[t] + Kt(t);
			E = D;
			D = C;
			C = S(B, 30);
			B = A;
			A = temp;
		}
		A1 = A = A + A1;
		B1 = B = B + B1;
		C1 = C = C + C1;
		D1 = D = D + D1;
		E1 = E = E + E1;
	}
	printf("%08x%08x%08x%08x%08x\n\n", A1, B1, C1, D1, E1);
	return 1;
}


//基本関数
SHA_INT_TYPE SHA1_K(SHA_INT_TYPE t) {
	if (t <= 19)return 0x5a827999;
	else if (t <= 39)return 0x6ed9eba1;
	else if (t <= 59)return 0x8f1bbcdc;
	else if (t <= 79)return 0xca62c1d6;
	return 0;
}

SHA_INT_TYPE SHA1_f(SHA_INT_TYPE t, SHA_INT_TYPE B, SHA_INT_TYPE C, SHA_INT_TYPE D) {
	if (t <= 19)return (B&C) | (~B&D);
	else if (t <= 39)return B ^ C^D;
	else if (t <= 59)return (B&C) | (B&D) | (C&D);
	else if (t <= 79)return B ^ C^D;
	return 0;
}

//左ローテート関数
SHA_INT_TYPE SHA1_rotl(SHA_INT_TYPE r, SHA_INT_TYPE x) {
	SHA_INT_TYPE rot = r % 32;
	return (x >> (32 - rot)) | (x << rot);
}

void HMAC_SHA1_Copy(unsigned char *copy, SHA1_DATA *shd) {
	SHA_INT_TYPE Value[5];
	for (int i = 0; i < 5; i++)Value[i] = SHA_Reverse(shd->Value[i]);
	memcpy(copy, Value, 20);
}

//右ローテート関数
SHA_INT_TYPE SHA256_rotr(SHA_INT_TYPE r, SHA_INT_TYPE x) {
	SHA_INT_TYPE rot = r % 32;
	return (x >> rot) | (x << (32 - rot));
}

SHA_INT_TYPE SHA256_sigma0(SHA_INT_TYPE x) {
	return SHA256_rotr(7, x) ^ SHA256_rotr(18, x) ^ (x >> 3);
}

SHA_INT_TYPE SHA256_sigma1(SHA_INT_TYPE x) {
	return SHA256_rotr(17, x) ^ SHA256_rotr(19, x) ^ (x >> 10);
}

SHA_INT_TYPE SHA256_sum0(SHA_INT_TYPE x) {
	return SHA256_rotr(2, x) ^ SHA256_rotr(13, x) ^ SHA256_rotr(22, x);
}

SHA_INT_TYPE SHA256_sum1(SHA_INT_TYPE x) {
	return SHA256_rotr(6, x) ^ SHA256_rotr(11, x) ^ SHA256_rotr(25, x);
}

SHA_INT_TYPE SHA256_ch(SHA_INT_TYPE x, SHA_INT_TYPE y, SHA_INT_TYPE z) {
	return (x & y) ^ (~x & z);
}

SHA_INT_TYPE SHA256_maj(SHA_INT_TYPE x, SHA_INT_TYPE y, SHA_INT_TYPE z) {
	return (x&y) ^ (x&z) ^ (y&z);
}

void SHA_Reverse_INT64(unsigned char *data, unsigned __int64 write) {
	unsigned char cdata[8];
	memcpy(cdata, &write, sizeof(__int64));
	for (int i = 0; i <= 7; i++)*(data + i) = cdata[7 - i];
}

SHA_INT_TYPE SHA_Reverse(SHA_INT_TYPE d) {
	unsigned char b_data[4], a_data[4];
	SHA_INT_TYPE ret;
	memcpy(b_data, &d, sizeof(__int32));
	for (int i = 0; i < 4; i++)a_data[i] = b_data[3 - i];
	memcpy(&ret, a_data, sizeof(a_data));
	return ret;
}

void HMAC_SHA256_Copy(unsigned char *copy, SHA256_DATA *shd) {
	SHA_INT_TYPE Value[8];
	for (int i = 0; i < 8; i++)Value[i] = SHA_Reverse(shd->Value[i]);
	memcpy(copy, Value, 32);
}

void SHA1_HashBlock(SHA_INT_TYPE *SHA1_H_Data, const unsigned char *data) {
	SHA_INT_TYPE SIT[80];
	SHA_INT_TYPE SIT_d[16];//512ビット、64バイト
	SHA_INT_TYPE a, b, c, d, e;
	for (int i = 0, j = 0; i < 16; i++, j += 4)SIT_d[i] = ((*(data + j + 3) & 0xFF) << 24) | ((*(data + j + 2) & 0xFF) << 16) | ((*(data + j + 1) & 0xFF) << 8) | ((*(data + j) & 0xFF));
	for (int i = 0; i < 16; i++)SIT[i] = SHA_Reverse(SIT_d[i]);
	for (int t = 16; t <= 79; t++)SIT[t] = SHA1_rotl(1, SIT[t - 3] ^ SIT[t - 8] ^ SIT[t - 14] ^ SIT[t - 16]);
	a = *SHA1_H_Data;
	b = *(SHA1_H_Data + 1);
	c = *(SHA1_H_Data + 2);
	d = *(SHA1_H_Data + 3);
	e = *(SHA1_H_Data + 4);
	for (int t = 0; t <= 79; t++) {
		SHA_INT_TYPE tmp;
		tmp = SHA1_rotl(5, a) + SHA1_f(t, b, c, d) + e + SIT[t] + SHA1_K(t);
		e = d;
		d = c;
		c = SHA1_rotl(30, b);
		b = a;
		a = tmp;
	}
	*SHA1_H_Data += a;
	*(SHA1_H_Data + 1) += b;
	*(SHA1_H_Data + 2) += c;
	*(SHA1_H_Data + 3) += d;
	*(SHA1_H_Data + 4) += e;
}

bool SHA1(SHA1_DATA *sha1d, const char *data, SHA_INT_TYPE size) {
	SHA_INT_TYPE s, h[5], ns;
	int cnt = 0;
	unsigned __int64 s64;
	unsigned char d[64];
	if (!sha1d)return false;
	s = (size) ? size : strlen(data);
	memcpy(h, SHA1_H_Val, sizeof(SHA1_H_Val));

	//dataのバイト数が64バイトを超えていたら60バイト未満になるまで処理をする
	for (SHA_INT_TYPE i = s, j = 0; i >= 64; i -= 64, j += 64)SHA1_HashBlock(h, (const unsigned char*)(data + j));

	//パディングに含めるデータのサイズ
	ns = s % 64;

	//d・・・パディング文字列
	memset(d, 0, 64);

	//パディングにコピー
	memcpy(d, data + (s - ns), ns);

	//d[s]に0x80を代入
	d[ns] = 0x80;

	//パディングに含めるデータのサイズが56バイト以上だった時の処理
	if (ns >= 56) {
		//パディングに含めるデータのサイズが56バイト以上だったらSHA1_HashBlockを実行する
		SHA1_HashBlock(h, d);
		//dの最初～56バイト文NULL文字をセット
		memset(d, 0, 56);
	}

	//データのサイズをビット数にする
	s64 = s * 8;

	//データのサイズ(ビット単位)を書き込む
	SHA_Reverse_INT64(&d[56], s64);

	//最後の処理
	SHA1_HashBlock(h, d);
	memcpy(sha1d->Value, h, sizeof(h));
	sprintf(sha1d->Val_String, "%08X %08X %08X %08X %08X", h[0], h[1], h[2], h[3], h[4]);
	return true;
}

bool HMAC_SHA1(SHA1_DATA *sha1d, const char *b_target, const char *b_key, SHA_INT_TYPE tsize) {
	unsigned char key[65], ipad[64], opad[64];
	unsigned char *tk, tk2[20], tk3[84];
	SHA_INT_TYPE tks;
	SHA1_DATA SD, ret;
	memset(&SD, 0, sizeof(SHA1_DATA));
	memset(key, 0, 65);
	memset(ipad, 0x36, 64);
	memset(opad, 0x5c, 64);
	if (!sha1d)return false;
	if (strlen(b_key) > 64) {
		SHA1(&SD, b_key, 0);
		HMAC_SHA1_Copy(key, &SD);
	}
	else {
		memcpy(key, (unsigned char*)b_key, strlen(b_key));
	}
	memset(&SD, 0, sizeof(SHA1_DATA));
	for (int i = 0; i < 64; i++) {
		ipad[i] = key[i] ^ ipad[i];
		opad[i] = key[i] ^ opad[i];
	}
	tks = (tsize) ? tsize : strlen(b_target) + 64;
	tk = (unsigned char *)malloc(tks);
	if (!tk)return false;
	memset(tk, 0, tks);
	memcpy(tk, ipad, 64);
	memcpy(tk + 64, (unsigned char*)b_target, (tsize) ? tsize : strlen(b_target));
	SHA1(&SD, (char *)tk, tks);
	HMAC_SHA1_Copy(tk2, &SD);
	memcpy(tk3, opad, 64);
	memcpy(tk3 + 64, tk2, 20);
	SHA1(&ret, (char *)tk3, 84);
	memcpy(sha1d, &ret, sizeof(SHA1_DATA));
	free(tk);
	return true;
}

void SHA256_HashBlock(SHA_INT_TYPE *SHA256_H_Data, const unsigned char *data) {
	SHA_INT_TYPE SIT[64];
	SHA_INT_TYPE SIT_d[16];//512ビット、64バイト
	SHA_INT_TYPE a, b, c, d, e, f, g, h;
	for (int i = 0, j = 0; i < 16; i++, j += 4)SIT_d[i] = ((*(data + j + 3) & 0xFF) << 24) | ((*(data + j + 2) & 0xFF) << 16) | ((*(data + j + 1) & 0xFF) << 8) | ((*(data + j) & 0xFF));
	for (int i = 0; i < 16; i++)SIT[i] = SHA_Reverse(SIT_d[i]);
	for (int t = 16; t <= 63; t++)	SIT[t] = SHA256_sigma1(SIT[t - 2]) + SIT[t - 7] + SHA256_sigma0(SIT[t - 15]) + SIT[t - 16];
	a = *SHA256_H_Data;
	b = *(SHA256_H_Data + 1);
	c = *(SHA256_H_Data + 2);
	d = *(SHA256_H_Data + 3);
	e = *(SHA256_H_Data + 4);
	f = *(SHA256_H_Data + 5);
	g = *(SHA256_H_Data + 6);
	h = *(SHA256_H_Data + 7);
	for (int t = 0; t <= 63; t++) {
		SHA_INT_TYPE tmp[2];
		tmp[0] = h + SHA256_sum1(e) + SHA256_ch(e, f, g) + SHA256_K_Val[t] + SIT[t];
		tmp[1] = SHA256_sum0(a) + SHA256_maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + tmp[0];
		d = c;
		c = b;
		b = a;
		a = tmp[0] + tmp[1];
	}
	*SHA256_H_Data += a;
	*(SHA256_H_Data + 1) += b;
	*(SHA256_H_Data + 2) += c;
	*(SHA256_H_Data + 3) += d;
	*(SHA256_H_Data + 4) += e;
	*(SHA256_H_Data + 5) += f;
	*(SHA256_H_Data + 6) += g;
	*(SHA256_H_Data + 7) += h;
}

bool SHA256(SHA256_DATA *sha256d, const char *data, SHA_INT_TYPE size) {
	SHA_INT_TYPE s, h[8], ns;
	int cnt = 0;
	unsigned __int64 s64;
	unsigned char d[64];
	if (!sha256d)return false;
	s = (size) ? size : strlen(data);
	memcpy(h, SHA256_H_Val, sizeof(SHA256_H_Val));

	//dataのバイト数が64バイトを超えていたら60バイト未満になるまで処理をする
	for (SHA_INT_TYPE i = s, j = 0; i >= 64; i -= 64, j += 64)SHA256_HashBlock(h, (const unsigned char*)(data + j));

	//パディングに含めるデータのサイズ
	ns = s % 64;

	//d・・・パディング文字列
	memset(d, 0, 64);

	//パディングにコピー
	memcpy(d, data + (s - ns), ns);

	//d[ns]に0x80を代入
	d[ns] = 0x80;

	//パディングに含めるデータのサイズが56バイト以上だった時の処理
	if (ns >= 56) {
		//パディングに含めるデータのサイズが56バイト以上だったらSHA1_HashBlockを実行する
		SHA256_HashBlock(h, d);

		//dの最初～56バイト文NULL文字をセット
		memset(d, 0, 56);
	}

	//データのサイズをビット数にする
	s64 = s * 8;

	//データのサイズ(ビット単位)を書き込む
	SHA_Reverse_INT64(&d[56], s64);

	//最後の処理
	SHA256_HashBlock(h, d);

	memcpy(sha256d->Value, h, sizeof(h));
	sprintf(sha256d->Val_String, "%08X %08X %08X %08X %08X %08X %08X %08X", h[0], h[1], h[2], h[3], h[4], h[5], h[6], h[7]);
	return true;
}

bool HMAC_SHA256(SHA256_DATA *sha256d, const char *b_target, const char *b_key, SHA_INT_TYPE tsize) {
	unsigned char key[65], ipad[64], opad[64];
	unsigned char *tk, tk2[32], tk3[96];
	SHA_INT_TYPE tks;
	SHA256_DATA SD, ret;
	memset(&SD, 0, sizeof(SHA256_DATA));
	memset(key, 0, 65);
	memset(ipad, 0x36, 64);
	memset(opad, 0x5c, 64);
	if (!sha256d)return false;
	if (strlen(b_key) > 64) {
		SHA256(&SD, b_key, 0);
		HMAC_SHA256_Copy(key, &SD);
	}
	else {
		memcpy(key, (unsigned char*)b_key, strlen(b_key));
	}
	memset(&SD, 0, sizeof(SHA256_DATA));
	for (int i = 0; i < 64; i++) {
		ipad[i] = key[i] ^ ipad[i];
		opad[i] = key[i] ^ opad[i];
	}
	tks = (tsize) ? tsize : strlen(b_target) + 64;
	tk = (unsigned char *)malloc(tks);
	if (!tk)return false;
	memset(tk, 0, tks);
	memcpy(tk, ipad, 64);
	memcpy(tk + 64, (unsigned char*)b_target, (tsize) ? tsize : strlen(b_target));
	SHA256(&SD, (char *)tk, tks);
	HMAC_SHA256_Copy(tk2, &SD);
	memcpy(tk3, opad, 64);
	memcpy(tk3 + 64, tk2, 32);
	SHA256(&ret, (char *)tk3, 96);
	memcpy(sha256d, &ret, sizeof(SHA256_DATA));
	free(tk);
	return true;
}