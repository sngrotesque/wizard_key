#include <snHash.h>

SN_PRIVATE(snErr_ctx) _snHash_init
SN_FUNC_OF((snHash_ctx **obj, snSize size))
{
    snErr_ctx error;
    (*obj)->hexdigestSize = size << 1;
    (*obj)->digestSize = size;

    (*obj)->hexdigest = snNull;
    if(!snMemoryNew(snByte *, (*obj)->digest, size)) {
        snErr_return(error, snErr_ErrMemory,
            "_snHash_init: (*obj)->digest failed to apply for memory.");
    }

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snHash_new SN_OPEN_API
SN_FUNC_OF((snHash_ctx **obj, snHash_HashType hashType))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNullData, "snHash_new: obj is NULL.");
    }
    if((sn_u32)hashType > 5) {
        snErr_return(error, snErr_ErrType,
            "snHash_new: Wrong hashType, must be 0 to 5.");
    }
    if(!snMemoryNew(snHash_ctx *, (*obj), sizeof(snHash_ctx))) {
        snErr_return(error, snErr_ErrMemory,
            "snHash_new: (*obj) failed to apply for memory.");
    }

    const EVP_MD *snHash_EVP_Type[6] = {
        EVP_md5(),    // MD5
        EVP_sha1(),   // SHA-1
        EVP_sha224(), // SHA-224
        EVP_sha256(), // SHA-256
        EVP_sha384(), // SHA-384
        EVP_sha512()  // SHA-512
    };

    (*obj)->md = snHash_EVP_Type[hashType];
    error = _snHash_init(obj, snHash_SIZE[hashType]);
    if(error.code) {
        return error;
    }

    snErr_return(error, snErr_OK, "OK.");
}

/**
 * https://www.openssl.org/docs/man1.1.1/man3/EVP_MD_CTX_new.html
 * 对于OpenSSL库提供的EVP相关功能，我查阅到OpenSSL库中的提示是
 * 返回值为1时表示无错误，为0时表示错误。
 * 为了不与本库（Shark Coast）产生冲突，不针对EVP相关功能进行
 * 返回值检查。
*/
SN_PUBLIC(snErr_ctx) snHash SN_OPEN_API
SN_FUNC_OF((snHash_ctx *hash, snByte *buf, snSize size))
{
    snErr_ctx error;
    if(!hash || !buf || !size) {
        snErr_return(error, snErr_ErrNullData, "snHash: hash or buf or size is NULL.");
    }
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();

    EVP_DigestInit_ex(md_ctx, hash->md, snNull);
    EVP_DigestUpdate(md_ctx, buf, size);
    EVP_DigestFinal_ex(md_ctx, hash->digest, snNull);
    EVP_MD_CTX_free(md_ctx);

    error = snBinascii_b2a_hex(&hash->hexdigest, hash->digest, hash->digestSize);
    if(error.code) {
        return error;
    }

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snHash_file SN_OPEN_API
SN_FUNC_OF((snHash_ctx *hash, snFileStr fn))
{
    snErr_ctx error;
    if(!hash || !fn) {
        snErr_return(error, snErr_ErrNullData,
            "snHash_file: hash or fn is NULL.");
    }
    EVP_MD_CTX *md_ctx = snNull;
    snByte *buf = snNull;
    snFile *fp = snNull;
    snSize fileSize;
    snSize quotient; // 文件长度除块大小的商
    snSize leftover; // 文件剩余长度
    snSize index;

    error = snFile_fileSize(&fileSize, fn);
    if(error.code) {
        return error;
    }

    if(!snMemoryNew(snByte *, buf, SN_HASH_BLOCKLEN)) {
        snErr_return(error, snErr_ErrMemory,
            "snHash_file: buf failed to apply for memory.");
    }

    quotient = fileSize / SN_HASH_BLOCKLEN;
    leftover = fileSize % SN_HASH_BLOCKLEN;

    if(!(fp = snFile_open(fn, snFile_Char("rb")))) {
        snErr_return(error, snErr_FileOpen, "snHash_file: File opening failed.");
    }

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

    error = snBinascii_b2a_hex(&hash->hexdigest, hash->digest, hash->digestSize);
    if(error.code) {
        return error;
    }

    snMemoryFree(buf);
    if(fclose(fp)) {
        snErr_return(error, snErr_FileClose, "snHash_file: File closing failed.");
    }

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snHash_free SN_OPEN_API
SN_FUNC_OF((snHash_ctx **obj))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNullData, "snHash_free: obj is NULL.");
    }
    if((*obj)->hexdigest) {
        snMemoryFree((*obj)->hexdigest);
    }
    snMemoryFree((*obj)->digest);
    snMemoryFree((*obj));
    snErr_return(error, snErr_OK, "OK.");
}
