/********************************************************
Copyright (C),  2016-2018,
FileName:		AesECB
Author:        	woniu201
Email:         	wangpengfei.201@163.com
Created:       	2018/09/11
Description:	实现AES128-ECB方式加密和解密，填充方式zeropadding
********************************************************/
#include <iostream>
#include <string>
#include <stdlib.h>

#include "openssl/evp.h"  
#include "openssl/aes.h"
using namespace std;

#pragma comment(lib, "libeay32.lib")

/************************************
@ Brief:		加密
@ Author:		woniu201 
@ Created:		2018/09/11
@ Return:            
************************************/
int EncryptECB(unsigned char* inData, int inDataLen,  unsigned char* outData, unsigned char* key)
{
	AES_KEY  aesKey;

	int block = 0;
	if (inDataLen%16 == 0)
	{
		block = inDataLen/16;
	}
	else
	{
		block = inDataLen/16 + 1;
	}

	AES_set_encrypt_key(key, 128, &aesKey);


	for (int i=0; i<block; i++)
	{
		AES_ecb_encrypt(inData+i*16, outData+i*16, &aesKey, AES_ENCRYPT);
	}

	return 0;
}


/************************************
@ Brief:		解密
@ Author:		woniu201 
@ Created:		2018/09/11
@ Return:            
************************************/
int DecryptECB(unsigned char* inData, unsigned char* outData, unsigned char* key)
{
	AES_KEY  aesKey;
	int block = strlen((char*)inData) /16;
	AES_set_decrypt_key(key, 128, &aesKey);

	for (int i=0; i<block; i++)
	{
		AES_ecb_encrypt(inData+i*16, outData+i*16, &aesKey, AES_DECRYPT);
	}
	return 0;
}


int main()
{
	unsigned char key[128] = {0};
	unsigned char data[512] = {0};
	memcpy(data, "abcdefghijklmnopqrstuvwxyz", strlen("abcdefghijklmnopqrstuvwxyz"));
	memcpy(key, "1234567812345678", strlen("1234567812345678"));

	cout << "原文数据：" << data << endl;
	unsigned char outData[1024] = {0};
	
	//================加密=======================//
	EncryptECB(data,  strlen((char*)data), outData, key);
	cout << "加密后的数据(hex)：";
	for (int i=0; i<strlen((char*)outData); i++)
	{
		printf("%02x", outData[i]);
	}
	cout << endl;

	//=================解密=======================//
	unsigned char plainTxt[128] = {0};
	DecryptECB(outData, plainTxt, key);
	cout << "解密后数据：" << plainTxt << endl;
	getchar();
}