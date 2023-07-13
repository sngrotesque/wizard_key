#include <wmkc_conf.h>

#if 0
#if WMKC_SUPPORT
#ifndef WMKC_RSA
#define WMKC_RSA
#include <wmkc_error.h>
#include <wmkc_memory.h>
#include <wmkc_file.h>

#if defined(WMKC_PLATFORM_WINOS)
#include <openssl/applink.c>
#endif

#include <openssl/pem.h>
#include <openssl/evp.h>

typedef struct {
    EVP_PKEY *key;
    wmkcChar *n;
    wmkcChar *e;
} wmkcRsa_pk_obj;

typedef struct {
    EVP_PKEY *key;
    wmkcChar *n;
    wmkcChar *e;
    wmkcChar *d;
    wmkcChar *p;
    wmkcChar *q;
} wmkcRsa_sk_obj;

typedef struct {
    EVP_PKEY_CTX *pub_enc; // 使用公钥加密的上下文
    EVP_PKEY_CTX *pub_dec; // 使用公钥解密的上下文
    EVP_PKEY_CTX *pri_enc; // 使用私钥加密的上下文
    EVP_PKEY_CTX *pri_dec; // 使用私钥解密的上下文
    wmkcRsa_pk_obj *pub; // RSA公钥
    wmkcRsa_sk_obj *pri; // RSA私钥
    wmkc_u32    key_bit; // 密钥对位长度
} wmkcRsa_obj;

/**
 * @brief 为RSA对象申请内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcRsa对象指针的地址
 * @return wmkcErr对象code为非0代表出错，需检查。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcRsa_new WMKC_OPEN_API
WMKC_OF((wmkcRsa_obj **obj));

#endif /* WMKC_KEY */
#endif /* WMKC_SUPPORT */




/*
WMKC_PUBLIC(wmkcErr_obj) wmkcKey_RSA_free WMKC_OPEN_API
WMKC_OF((wmkcKey_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcKey_RSA_free: obj is NULL.");
    }

    if((*obj)->pub_ctx) {
        EVP_PKEY_CTX_free((*obj)->pub_ctx);
        (*obj)->pub_ctx = wmkcNull;
    }
    if((*obj)->keyPair) {
        EVP_PKEY_free((*obj)->keyPair);
        (*obj)->keyPair = wmkcNull;
    }
    wmkcMem_free((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcKey_RSA_generateKeyPair WMKC_OPEN_API
WMKC_OF((wmkcKey_obj *obj, wmkc_u32 pubKey_bit))
{
    wmkcErr_obj error;
    if(!obj || !pubKey_bit) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcKey_generateKeyPair: "
            "obj or pubKey_bit is NULL.");
    }

    obj->key_bit = pubKey_bit;
    BIGNUM *e = wmkcNull;

    if(!(e = BN_new())) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcKey_generateKeyPair: "
            "The BN_new function returned NULL.");
    }
    if(EVP_PKEY_keygen_init(obj->pub_ctx) != 1) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcKey_generateKeyPair: "
            "Function EVP_PKEY_keygen_init returned an error code.");
    }
    if(EVP_PKEY_CTX_set_rsa_keygen_bits(obj->pub_ctx, pubKey_bit) != 1) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcKey_generateKeyPair: "
            "Function EVP_PKEY_CTX_set_rsa_keygen_bits returned an error code.");
    }
    if(BN_set_word(e, 65537) != 1) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcKey_generateKeyPair: "
            "Function BN_set_word returned an error code.");
    }
    if(EVP_PKEY_CTX_set1_rsa_keygen_pubexp(obj->pub_ctx, e) != 1) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcKey_generateKeyPair: "
            "Function EVP_PKEY_CTX_set1_rsa_keygen_pubexp returned an error code.");
    }
    if(EVP_PKEY_keygen(obj->pub_ctx, &obj->keyPair) != 1) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcKey_generateKeyPair: "
            "Function EVP_PKEY_keygen returned an error code.");
    }
    BN_free(e);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcKey_RSA_PublicKey_save_pem WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn, EVP_PKEY *keyPair))
{
    wmkcErr_obj error;
    if(!fn || !keyPair) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcKey_RSA_PublicKey_save_pem: "
            "fn or keyPair is NULL.");
    }
    wmkcFile_obj *file = wmkcNull;

    error = wmkcFile_open(&file, fn, "wb");
    if(error.code) return error;

    PEM_write_PUBKEY(file->fp, keyPair);

    error = wmkcFile_close(&file);
    if(error.code) return error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcKey_RSA_PublicKey_load_pem WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn, EVP_PKEY **keyPair))
{
    wmkcErr_obj error;
    if(!fn || !keyPair) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcKey_RSA_PublicKey_load_pem: "
            "fn or keyPair is NULL.");
    }
    wmkcFile_obj *file = wmkcNull;

    error = wmkcFile_open(&file, fn, "rb");
    if(error.code) return error;

    (*keyPair) = PEM_read_PUBKEY(file->fp, wmkcNull, wmkcNull, wmkcNull);

    error = wmkcFile_close(&file);
    if(error.code) return error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcKey_RSA_PrivateKey_save_pem WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn, EVP_PKEY *keyPair))
{
    wmkcErr_obj error;
    if(!fn || !keyPair) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcKey_RSA_PrivateKey_save_pem: "
            "fn or keyPair is NULL.");
    }
    wmkcFile_obj *file = wmkcNull;

    error = wmkcFile_open(&file, fn, "wb");
    if(error.code) return error;

    PEM_write_PrivateKey(file->fp, keyPair, wmkcNull, wmkcNull, 0,
        wmkcNull, wmkcNull);

    error = wmkcFile_close(&file);
    if(error.code) return error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcKey_RSA_PrivateKey_load_pem WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn, EVP_PKEY **keyPair))
{
    wmkcErr_obj error;
    if(!fn || !keyPair) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcKey_RSA_PrivateKey_load_pem: "
            "fn or keyPair is NULL.");
    }
    wmkcFile_obj *file = wmkcNull;

    error = wmkcFile_open(&file, fn, "rb");
    if(error.code) return error;

    (*keyPair) = PEM_read_PrivateKey(file->fp, wmkcNull, wmkcNull, wmkcNull);

    error = wmkcFile_close(&file);
    if(error.code) return error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
*/
#endif /* #if 0 */

