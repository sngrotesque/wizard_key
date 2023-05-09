#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "modes.h"
#include "sha.h"
#include "aes_cfb_crypt.h"


void AES_cfb128_encrypt(const unsigned char *in, unsigned char *out,
                        size_t length, const AES_KEY *key,
                        unsigned char *ivec, int *num, const int enc)
{

	if (enc)
		CRYPTO_cfb128_encrypt(in, out, length, key, ivec, num, enc, (block128_f)AES_encrypt);
	else
		CRYPTO_cfb128_encrypt(in, out, length, key, ivec, num, enc, (block128_f)AES_encrypt);
}

int randomIV(unsigned char *out)
{
	srand(time(NULL));
	char *t_enc = (char *)malloc(SHA512_DIGEST_LENGTH);
	int iLen = sizeof(int);
	int loop = SHA512_DIGEST_LENGTH / iLen;
	int t;
	while(loop--) {
		t = rand();
		memcpy(t_enc + loop * iLen, (char *)&t, iLen);
	}
	t = sizeof(SHA512_CTX);
	SHA512_CTX *ctx = (SHA512_CTX *)malloc(t);
	SHA512_Init(ctx);
	unsigned char *digest = malloc(SHA512_DIGEST_LENGTH);
	SHA512_Update(ctx, t_enc, SHA512_DIGEST_LENGTH);
	SHA512_Final(digest, ctx);
	if(AES_IV_SIZE > SHA512_DIGEST_LENGTH) {
		memcpy(out, digest, SHA512_DIGEST_LENGTH);
	} else {
		memcpy(out, digest + (rand() % (SHA512_DIGEST_LENGTH - AES_IV_SIZE + 1)), AES_IV_SIZE);
	}

	OPENSSL_cleanse(ctx, t);
	OPENSSL_cleanse(digest, SHA512_DIGEST_LENGTH);
	free(ctx);
	free(digest);
	memset(t_enc, 0, SHA512_DIGEST_LENGTH);
	free(t_enc);
	return AES_IV_SIZE;
}

int randomIv(unsigned char *out)
{
	srand(time(NULL));
	int t_enc_len = 2 * AES_BLOCK_SIZE * sizeof(char);
	char *t_enc = (char *)malloc(t_enc_len);
	int iLen = sizeof(int);
	int loop = t_enc_len / iLen;
	int t;
	while(loop--) {
		t = rand();
		memcpy(t_enc + loop * iLen, (char *)&t, iLen);
	}
	AES_KEY aesKey;
	t = AES_set_encrypt_key((const unsigned char *)t_enc, 128, &aesKey);
	if(t == 0) {
		AES_encrypt((const unsigned char *)t_enc + AES_BLOCK_SIZE, out, &aesKey);
		memset(t_enc, 0, t_enc_len);
		free(t_enc);
		return AES_BLOCK_SIZE;
	} else {
		memset(t_enc, 0, t_enc_len);
		free(t_enc);
		return 0;
	}
}

int random_iv(unsigned char *out)
{
	srand(time(NULL));
	int _len = sizeof(int) / 2;
	int loop = AES_BLOCK_SIZE * sizeof(char) / _len;
	int t;
	while(loop--) {
		t = rand();
		/*memcpy(out + loop * _len, (unsigned char *)&t, _len);*/
		memcpy(out + loop * _len, (char *)&t, _len);
	}
	t = 0;
	return AES_BLOCK_SIZE;
}

int aes_256_cfb_crypt(const unsigned char *in, unsigned char *out, size_t length, char *strPwd, unsigned char *crypt_iv, int *num, int mode)
{
	unsigned char *key = (unsigned char *)malloc(32);
	unsigned char *iv = (unsigned char *)malloc(AES_IV_SIZE);
	sha384_key_iv(strPwd, strlen(strPwd), key, iv);
	AES_KEY aesKey;
	int setKey = AES_set_encrypt_key((const unsigned char *)key, 256, &aesKey);
	free(key);
	if (setKey != 0) {
		return -1;
	}
	if(mode == 1) {
		AES_cfb128_encrypt(crypt_iv, out, AES_IV_SIZE, (const void *)&aesKey, iv, num, AES_ENCRYPT);
		if(*num != 0) {
			return -2;
		}
		AES_cfb128_encrypt(in, out + AES_IV_SIZE, length, (const void *)&aesKey, crypt_iv, num, AES_ENCRYPT);
		free(iv);
		/*
		if(num == (length % AES_BLOCK_SIZE))
		{
			printf("good~\n");
		}
		*/
		return length + AES_IV_SIZE;
	} else if(mode == 2) {
		AES_cfb128_encrypt(in, crypt_iv, AES_IV_SIZE, (const void *)&aesKey, iv, num, AES_DECRYPT);
		if(*num != 0) {
			return -2;
		}
		AES_cfb128_encrypt(in + AES_IV_SIZE, out, length - AES_IV_SIZE, (const void *)&aesKey, crypt_iv, num, AES_DECRYPT);
		free(iv);
		return length - AES_IV_SIZE;
	} else if(mode == 3) {
		AES_cfb128_encrypt(in, out, length, (const void *)&aesKey, crypt_iv, num, AES_ENCRYPT);
		free(iv);
		return length;
	} else if(mode == 4) {
		AES_cfb128_encrypt(in, out, length, (const void *)&aesKey, crypt_iv, num, AES_DECRYPT);
		free(iv);
		return length;
	}
	return -3;
}


int sha384_key_iv(char * in, size_t length, unsigned char * key, unsigned char * iv)
{
	int ctxLen = sizeof(SHA512_CTX);
	SHA512_CTX *ctx = (SHA512_CTX *)malloc(ctxLen);
	SHA384_Init(ctx);
	unsigned char *digest = (unsigned char *)malloc(SHA384_DIGEST_LENGTH);
	SHA384_Update(ctx, in, length);
	SHA384_Final(digest, ctx);
	memcpy(key, digest, 32);
	/*key[32] = '\0';*/
	memcpy(iv, digest + 32, SHA384_DIGEST_LENGTH - 32);
	/*iv[AES_IV_SIZE] = '\0';*/

	OPENSSL_cleanse(ctx, ctxLen);
	OPENSSL_cleanse(digest, SHA384_DIGEST_LENGTH);
	free(ctx);
	free(digest);
	return 32;
}
