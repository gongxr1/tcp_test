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


#ifndef INCLUDE_GENERALHASHFUNCTION_CPP_H
#define INCLUDE_GENERALHASHFUNCTION_CPP_H

#include<iostream>
#include<vector>
#include<string>
#include <stdio.h>
#include <malloc.h>

using namespace std;



typedef unsigned int (*HashFunction)(const std::string&);


unsigned int RSHash  (const std::string& str);
unsigned int JSHash  (const std::string& str);
unsigned int PJWHash (const std::string& str);
//unsigned int ELFHash (const std::string& str);
unsigned int BKDRHash(const std::string& str);
unsigned int SDBMHash(const std::string& str);
unsigned int DJBHash (const std::string& str);
unsigned int DEKHash (const std::string& str);
unsigned int BPHash  (const std::string& str);
unsigned int FNVHash (const std::string& str);
unsigned int APHash  (const std::string& str);
unsigned int sha1(const std::string& text);


//型定義
typedef unsigned __int32 SHA_INT_TYPE;

//構造体定義
typedef struct tagSHA256_DATA {
	SHA_INT_TYPE Value[8];
	char Val_String[72];
}SHA256_DATA;

typedef struct tagSHA1_DATA {
	SHA_INT_TYPE Value[5];
	char Val_String[45];
}SHA1_DATA;

//SHA1のメッセージダイジェスト初期値
const SHA_INT_TYPE SHA1_H_Val[] = { 0x67452301 , 0xefcdab89 , 0x98badcfe , 0x10325476 , 0xc3d2e1f0 };

//SHA256のメッセージダイジェスト初期値
const SHA_INT_TYPE SHA256_H_Val[] = { 0x6a09e667 , 0xbb67ae85 , 0x3c6ef372 , 0xa54ff53a , 0x510e527f , 0x9b05688c , 0x1f83d9ab , 0x5be0cd19 };

//SHA256の加算定数
const SHA_INT_TYPE SHA256_K_Val[] = { 0x428a2f98 , 0x71374491 , 0xb5c0fbcf , 0xe9b5dba5 , 0x3956c25b , 0x59f111f1 , 0x923f82a4 , 0xab1c5ed5 ,
									  0xd807aa98 , 0x12835b01 , 0x243185be , 0x550c7dc3 , 0x72be5d74 , 0x80deb1fe , 0x9bdc06a7 , 0xc19bf174 ,
									  0xe49b69c1 , 0xefbe4786 , 0x0fc19dc6 , 0x240ca1cc , 0x2de92c6f , 0x4a7484aa , 0x5cb0a9dc , 0x76f988da ,
									  0x983e5152 , 0xa831c66d , 0xb00327c8 , 0xbf597fc7 , 0xc6e00bf3 , 0xd5a79147 , 0x06ca6351 , 0x14292967 ,
									  0x27b70a85 , 0x2e1b2138 , 0x4d2c6dfc , 0x53380d13 , 0x650a7354 , 0x766a0abb , 0x81c2c92e , 0x92722c85 ,
									  0xa2bfe8a1 , 0xa81a664b , 0xc24b8b70 , 0xc76c51a3 , 0xd192e819 , 0xd6990624 , 0xf40e3585 , 0x106aa070 ,
									  0x19a4c116 , 0x1e376c08 , 0x2748774c , 0x34b0bcb5 , 0x391c0cb3 , 0x4ed8aa4a , 0x5b9cca4f , 0x682e6ff3 ,
									  0x748f82ee , 0x78a5636f , 0x84c87814 , 0x8cc70208 , 0x90befffa , 0xa4506ceb , 0xbef9a3f7 , 0xc67178f2 };

//プロトタイプ宣言(基本関数)
SHA_INT_TYPE SHA1_K(SHA_INT_TYPE);
SHA_INT_TYPE SHA1_f(SHA_INT_TYPE, SHA_INT_TYPE, SHA_INT_TYPE, SHA_INT_TYPE);
SHA_INT_TYPE SHA1_rotl(SHA_INT_TYPE, SHA_INT_TYPE);
SHA_INT_TYPE SHA256_rotr(SHA_INT_TYPE, SHA_INT_TYPE);
SHA_INT_TYPE SHA256_sigma0(SHA_INT_TYPE);
SHA_INT_TYPE SHA256_sigma1(SHA_INT_TYPE);
SHA_INT_TYPE SHA256_sum0(SHA_INT_TYPE);
SHA_INT_TYPE SHA256_sum1(SHA_INT_TYPE);
SHA_INT_TYPE SHA256_ch(SHA_INT_TYPE, SHA_INT_TYPE, SHA_INT_TYPE);
SHA_INT_TYPE SHA256_maj(SHA_INT_TYPE, SHA_INT_TYPE, SHA_INT_TYPE);

void SHA_Reverse_INT64(const unsigned char *, unsigned __int64);
SHA_INT_TYPE SHA_Reverse(SHA_INT_TYPE);

//プロトタイプ宣言(SHA)
void SHA1_HashBlock(SHA_INT_TYPE *, const unsigned char *);
bool SHA1(SHA1_DATA *, const char*, SHA_INT_TYPE);
void SHA256_HashBlock(SHA_INT_TYPE *, const unsigned char *);
bool SHA256(SHA256_DATA *, const char*, SHA_INT_TYPE);

//プロトタイプ(HMAC)
bool HMAC_SHA1(SHA1_DATA *, const char*, const char*, SHA_INT_TYPE);
void HMAC_SHA1_Copy(const unsigned char*, SHA1_DATA*);
bool HMAC_SHA256(SHA256_DATA *, const char*, const char*, SHA_INT_TYPE);
void HMAC_SHA256_Copy(const unsigned char*, SHA256_DATA*);


#endif
