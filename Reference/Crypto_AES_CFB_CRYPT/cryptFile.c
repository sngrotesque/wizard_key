#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#if !defined(__APPLE__) && !defined(__ANDROID__)
#include <io.h>
#endif

#include "aes_cfb_crypt.h"


// 判断文件是否存在
int file_exists(char *filename);

int enc_iv_encrypt(char *in, int inLen, unsigned char *out, char *strPwd, unsigned char *crypt_iv, int *num)
{
	return aes_256_cfb_crypt((const unsigned char *)in, out, inLen, strPwd, crypt_iv, num, ENC_IV_ENCRYPT);
}

int enc_iv_decrypt(char *in, int inLen, unsigned char *out, char *strPwd, unsigned char *crypt_iv, int *num)
{
	return aes_256_cfb_crypt((const unsigned char *)in, out, inLen, strPwd, crypt_iv, num, ENC_IV_DECRYPT);
}

int reuse_iv_encrypt(char *in, int inLen, unsigned char *out, char *strPwd, unsigned char *crypt_iv, int *num)
{
	return aes_256_cfb_crypt((const unsigned char *)in, out, inLen, strPwd, crypt_iv, num, REUSE_IV_ENCRYPT);
}

int reuse_iv_decrypt(char *in, int inLen, unsigned char *out, char *strPwd, unsigned char *crypt_iv, int *num)
{
	return aes_256_cfb_crypt((const unsigned char *)in, out, inLen, strPwd, crypt_iv, num, REUSE_IV_DECRYPT);
}

// 主函数
int main(int argc, char *argv[])
{
	char *source, *dest, *strKey, *enc;
	printf("args: Inputfile Outputfile password encrypt:1|decrypt:0 ?:FileBufferLength\ndecrypt with file buffer length 1024 e.g : Inputfile.txt Outputfile.txt myPassword 0 1024\n\n");
	FILE *fSource, *fDest;
	if(argc > 4) {
		source = argv[1]; //源文件     Inputfile
		dest = argv[2];   //目的文件   Outputfile
		strKey = argv[3]; //密码       password
		enc = argv[4];    //是否为加密 encrypt or decrypt : 1 for encrypt else for decrypt
	} else {
		printf("param error\nparam: sourceFile destFile password enc|dec\ne.g ./cryptFile Inputfile.txt Outputfile.txt myPassword 1\n");
		exit(0);
	}
	// check parameter; 检查参数
	if(source==NULL || dest==NULL || strKey==NULL || enc==NULL) {
		printf("param error\nparam: sourceFile destFile password enc|dec\ne.g ./cryptFile Inputfile.txt Outputfile.txt myPassword 0\n");
		exit(0);
	}
	// if source file exists; 判断源文件是否存在
	if(file_exists(source)==FALSE) {
		printf("%s not exists\n",source);
		exit(0);
	}
	unsigned char *crypt_iv = (unsigned char *)malloc(AES_IV_SIZE);
	if(enc[0] == '1' && randomIV(crypt_iv) != AES_IV_SIZE) {
		printf("Get random iv error\n");
		free(crypt_iv);
		exit(0);
	}

	char *tfBuf;
	int charLen = sizeof(char);
	int fBufLen = 8192;
	if(argc > 5) {
		sscanf(argv[5], "%d", &fBufLen);
		if(fBufLen < AES_IV_SIZE){
			fBufLen = AES_IV_SIZE;//at least AES_IV_SIZE; 解密时至少读取16字节iv
		}
	}
	printf("File buffer length : %d\n", fBufLen);

	fSource = fopen(source, "rb");
	fDest = fopen(dest, "wb");

	time_t tStart;
	time(&tStart);

	int tfBufLen = fBufLen * charLen;
	tfBuf = (char *)malloc(tfBufLen);
	/* cfb算法(no padding)没有填充cryptOutBufLen = fBufLen + AES_IV_SIZE就够了
	有填充的算法(has padding)必须cryptOutBufLen = fBufLen + AES_IV_SIZE + AES_BLOCK_SIZE */
	int cryptOutBufLen = (fBufLen + AES_IV_SIZE + AES_BLOCK_SIZE) * charLen;
	unsigned char *cryptOutBuf = (unsigned char *)malloc(cryptOutBufLen);
	int readCount = fread(tfBuf, charLen, fBufLen, fSource);//读取
	int cryptedLen = 0;
	int num = 0;
	if(readCount == fBufLen || (readCount > 0 && feof(fSource))) {
		if(enc[0] == '1') {
			cryptedLen = enc_iv_encrypt(tfBuf, readCount, cryptOutBuf, strKey, crypt_iv, &num);
		} else {
			cryptedLen = enc_iv_decrypt(tfBuf, readCount, cryptOutBuf, strKey, crypt_iv, &num);
		}
		if(cryptedLen < 0) {
			printf("Crypt error !\n");
		} else {
			fwrite(cryptOutBuf, charLen, cryptedLen, fDest);//写入文件
		}
	} else {
		if(!feof(fSource)) {
			printf("Read file error : %s\n", source);
		}
	}
	while(1) {
		readCount = fread(tfBuf, charLen, fBufLen, fSource);
		if(readCount == fBufLen || (readCount > 0 && feof(fSource))) {
			if(enc[0] == '1') {
				cryptedLen = reuse_iv_encrypt(tfBuf, readCount, cryptOutBuf, strKey, crypt_iv, &num);
			} else {
				cryptedLen = reuse_iv_decrypt(tfBuf, readCount, cryptOutBuf, strKey, crypt_iv, &num);
			}
			if(cryptedLen < 0) {
				printf("Crypt error !\n");
			} else {
				fwrite(cryptOutBuf, charLen, cryptedLen, fDest);
			}
		} else {
			if(!feof(fSource)) {
				printf("Read file error : %s\n", source);
			}
			break;
		}
	}
	
	double the_difftime = difftime(time(NULL), tStart);
	if(ferror(fSource)) {
		printf("Read file error\n\nWith %lf second\n", the_difftime);
	} else {
		printf("Success --> With %lf second\n", the_difftime);
	}
	fclose(fSource);
	fclose(fDest);
	memset(tfBuf, 0, tfBufLen);
	free(tfBuf);
	memset(cryptOutBuf, 0, cryptOutBufLen);
	free(cryptOutBuf);
	memset(strKey, 0, strlen(strKey));
	free(strKey);
	memset(crypt_iv, 0, AES_IV_SIZE);
	free(crypt_iv);
	exit(0);
}

// 判断文件是否存在
int file_exists(char *filename)
{
	return (access(filename, 0)==0);
}
