#include <crypto/rsa.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcRsa_new WMKC_OPEN_API
WMKC_OF((wmkcRsa_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcRsa_new: obj is NULL.");
    }
    if(!wmkcMem_new(wmkcRsa_obj *, (*obj), sizeof(wmkcRsa_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcRsa_new: "
            "Failed to allocate memory for (*obj).");
    }

    if(!wmkcMem_new(wmkcRsa_pk_obj *, (*obj)->pub, sizeof(wmkcRsa_pk_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcRsa_new: "
            "Failed to allocate memory for (*obj)->pub.");
    }
    if(!wmkcMem_new(wmkcRsa_sk_obj *, (*obj)->pri, sizeof(wmkcRsa_sk_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcRsa_new: "
            "Failed to allocate memory for (*obj)->pri.");
    }

    // 将所有内容初始化为0
    (*obj)->key_bit = 0;
    (*obj)->pub_enc = wmkcNull;
    (*obj)->pub_dec = wmkcNull;
    (*obj)->pri_enc = wmkcNull;
    (*obj)->pri_dec = wmkcNull;

    (*obj)->pub->key = wmkcNull;
    (*obj)->pub->n = wmkcNull;
    (*obj)->pub->e = wmkcNull;

    (*obj)->pri->key = wmkcNull;
    (*obj)->pri->n = wmkcNull;
    (*obj)->pri->e = wmkcNull;
    (*obj)->pri->d = wmkcNull;
    (*obj)->pri->q = wmkcNull;
    (*obj)->pri->p = wmkcNull;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcRsa_newkeys WMKC_OPEN_API
WMKC_OF((wmkcRsa_obj *obj, wmkc_u32 key_bit))
{
    wmkcErr_obj error;
    if(!obj || !key_bit) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcRsa_newkeys: "
            "obj or key_bit is NULL.");
    }

    EVP_PKEY_CTX *ctx = wmkcNull;
    EVP_PKEY *keyPair = wmkcNull;
    obj->key_bit = key_bit;

    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, wmkcNull);

    if(EVP_PKEY_keygen_init(ctx) != 1) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcRsa_newkeys: "
            "Function EVP_PKEY_keygen_init returned an error code.");
    }
    if(EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, key_bit) != 1) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcRsa_newkeys: "
            "Function EVP_PKEY_CTX_set_rsa_keygen_bits returned an error code.");
    }

    if(EVP_PKEY_keygen(ctx, &keyPair) != 1) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcRsa_newkeys: "
            "Function EVP_PKEY_keygen returned an error code.");
    }
    obj->pub->key = EVP_PKEY_new();
    obj->pri->key = EVP_PKEY_new();



    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
