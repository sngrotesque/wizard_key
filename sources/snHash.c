#include <snHash.h>

SN_PRIVATE(snError) _snHash_init
SN_FUNC_OF((snHash_ctx **obj, snSize size))
{
    (*obj)->hexdigestSize = size << 1;
    (*obj)->digestSize = size;

    (*obj)->hexdigest = snNull;
    if(!snMemoryNew(snByte *, (*obj)->digest, size))
        return snErr_Memory;

    return snErr_OK;
}

SN_PUBLIC(snError) snHash_new SN_OPEN_API
SN_FUNC_OF((snHash_ctx **obj, snHash_HashType hashType))
{
    if(!obj)
        return snErr_ErrNullData;

    if(!snMemoryNew(snHash_ctx *, (*obj), sizeof(snHash_ctx)))
        return snErr_Memory;

    const EVP_MD *snHash_EVP_Type[6] = {
        EVP_md5(),    // MD5
        EVP_sha1(),   // SHA-1
        EVP_sha224(), // SHA-224
        EVP_sha256(), // SHA-256
        EVP_sha384(), // SHA-384
        EVP_sha512()  // SHA-512
    };

    (*obj)->md = snHash_EVP_Type[hashType];
    if(_snHash_init(obj, snHash_SIZE[hashType])) {
        return snErr_Memory;
    }

    return snErr_OK;
}

SN_PUBLIC(snError) snHash SN_OPEN_API
SN_FUNC_OF((snHash_ctx *hash, snByte *buf, snSize size))
{
    snError err_code;
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();

    EVP_DigestInit_ex(md_ctx, hash->md, snNull);
    EVP_DigestUpdate(md_ctx, buf, size);
    EVP_DigestFinal_ex(md_ctx, hash->digest, snNull);
    EVP_MD_CTX_free(md_ctx);

    if((err_code = snBinascii_b2a_hex(&hash->hexdigest, hash->digest, hash->digestSize)))
        return err_code;

    return snErr_OK;
}

SN_PUBLIC(snError) snHash_file SN_OPEN_API
SN_FUNC_OF((snHash_ctx *hash, snString fn))
{
    static EVP_MD_CTX *md_ctx = snNull;
    static snByte *buf = snNull;
    static snFile *fp = snNull;
    static snError err_code;
    static snSize fileSize;
    static snSize quotient; // 文件长度除块大小的商
    static snSize leftover; // 文件剩余长度
    static snSize index;

    if(!snFile_exists(fn)) {
        return snErr_FileFolderPath;
    }
    if((err_code = snFile_fileSize(&fileSize, fn))) {
        return err_code;
    }
    if(!(buf = (snByte *)malloc(SN_HASH_BLOCKLEN))) {
        return snErr_Memory;
    }

    quotient = fileSize / SN_HASH_BLOCKLEN;
    leftover = fileSize % SN_HASH_BLOCKLEN;

    fp = fopen(fn, "rb");

    md_ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(md_ctx, hash->md, snNull);

    for(index = 0; index < quotient; ++index) {
        fread(buf, 1, SN_HASH_BLOCKLEN, fp);
        EVP_DigestUpdate(md_ctx, buf, SN_HASH_BLOCKLEN);
    }
    if(leftover) {
        fread(buf, 1, leftover, fp);
        EVP_DigestUpdate(md_ctx, buf, leftover);
    }
    EVP_DigestFinal_ex(md_ctx, hash->digest, snNull);
    EVP_MD_CTX_free(md_ctx);

    if((err_code = snBinascii_b2a_hex(&hash->hexdigest, hash->digest, hash->digestSize)))
        return err_code;

    free(buf);
    fclose(fp);
    return snErr_OK;
}

SN_PUBLIC(snError) snHash_free SN_OPEN_API
SN_FUNC_OF((snHash_ctx **obj))
{
    if(!obj) {
        return snErr_ErrNullData;
    }
    snMemoryFree((*obj)->digest);
    snMemoryFree((*obj)->hexdigest);
    snMemoryFree((*obj));
    return snErr_OK;
}
