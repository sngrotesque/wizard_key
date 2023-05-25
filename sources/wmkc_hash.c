#include <wmkc_hash.h>

WMKC_PRIVATE(wmkcErr_ctx) _wmkcHash_init
WMKC_OF((wmkcHash_ctx **obj, wmkcSize size))
{
    wmkcErr_ctx error;
    (*obj)->hexdigestSize = size << 1;
    (*obj)->digestSize = size;

    (*obj)->hexdigest = wmkcNull;
    if(!wmkcMemoryNew(wmkcByte *, (*obj)->digest, size)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "_wmkcHash_init: (*obj)->digest failed to apply for memory.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcHash_new WMKC_OPEN_API
WMKC_OF((wmkcHash_ctx **obj, wmkcHash_HashType hashType))
{
    wmkcErr_ctx error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcHash_new: obj is NULL.");
    }
    if((wmkc_u32)hashType > 5) {
        wmkcErr_return(error, wmkcErr_ErrType,
            "wmkcHash_new: Wrong hashType, must be 0 to 5.");
    }
    if(!wmkcMemoryNew(wmkcHash_ctx *, (*obj), sizeof(wmkcHash_ctx))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcHash_new: (*obj) failed to apply for memory.");
    }

    const EVP_MD *wmkcHash_EVP_Type[6] = {
        EVP_md5(),    // MD5
        EVP_sha1(),   // SHA-1
        EVP_sha224(), // SHA-224
        EVP_sha256(), // SHA-256
        EVP_sha384(), // SHA-384
        EVP_sha512()  // SHA-512
    };

    (*obj)->md = wmkcHash_EVP_Type[hashType];
    error = _wmkcHash_init(obj, wmkcHash_SIZE[hashType]);
    if(error.code) {
        return error;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * https://www.openssl.org/docs/man1.1.1/man3/EVP_MD_CTX_new.html
 * 对于OpenSSL库提供的EVP相关功能，我查阅到OpenSSL库中的提示是
 * 返回值为1时表示无错误，为0时表示错误。
 * 为了不与本库（Shark Coast）产生冲突，不针对EVP相关功能进行
 * 返回值检查。
*/
WMKC_PUBLIC(wmkcErr_ctx) wmkcHash WMKC_OPEN_API
WMKC_OF((wmkcHash_ctx *hash, wmkcByte *buf, wmkcSize size))
{
    wmkcErr_ctx error;
    if(!hash || !buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcHash: hash or buf or size is NULL.");
    }
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();

    EVP_DigestInit_ex(md_ctx, hash->md, wmkcNull);
    EVP_DigestUpdate(md_ctx, buf, size);
    EVP_DigestFinal_ex(md_ctx, hash->digest, wmkcNull);
    EVP_MD_CTX_free(md_ctx);

    error = wmkcBinascii_b2a_hex(&hash->hexdigest, hash->digest, hash->digestSize);
    if(error.code) {
        return error;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcHash_file WMKC_OPEN_API
WMKC_OF((wmkcHash_ctx *hash, wmkcFileString fn))
{
    wmkcErr_ctx error;
    if(!hash || !fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcHash_file: hash or fn is NULL.");
    }
    EVP_MD_CTX *md_ctx = wmkcNull;
    wmkcByte *buf = wmkcNull;
    wmkcFile *fp = wmkcNull;
    wmkcSize fileSize;
    wmkcSize quotient; // 文件长度除块大小的商
    wmkcSize leftover; // 文件剩余长度
    wmkcSize index;

    error = wmkcFile_fileSize(&fileSize, fn);
    if(error.code) {
        return error;
    }

    if(!wmkcMemoryNew(wmkcByte *, buf, SN_HASH_BLOCKLEN)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcHash_file: buf failed to apply for memory.");
    }

    quotient = fileSize / SN_HASH_BLOCKLEN;
    leftover = fileSize % SN_HASH_BLOCKLEN;

    if(!(fp = wmkcFile_fopen(fn, wmkcFile_text("rb")))) {
        wmkcErr_return(error, wmkcErr_FileOpen, "wmkcHash_file: File opening failed.");
    }

    md_ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(md_ctx, hash->md, wmkcNull);

    for(index = 0; index < quotient; ++index) {
        fread(buf, 1, SN_HASH_BLOCKLEN, fp);
        EVP_DigestUpdate(md_ctx, buf, SN_HASH_BLOCKLEN);
    }
    if(leftover) {
        fread(buf, 1, leftover, fp);
        EVP_DigestUpdate(md_ctx, buf, leftover);
    }
    EVP_DigestFinal_ex(md_ctx, hash->digest, wmkcNull);
    EVP_MD_CTX_free(md_ctx);

    error = wmkcBinascii_b2a_hex(&hash->hexdigest, hash->digest, hash->digestSize);
    if(error.code) {
        return error;
    }

    wmkcMemoryFree(buf);
    if(fclose(fp)) {
        wmkcErr_return(error, wmkcErr_FileClose, "wmkcHash_file: File closing failed.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcHash_free WMKC_OPEN_API
WMKC_OF((wmkcHash_ctx **obj))
{
    wmkcErr_ctx error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcHash_free: obj is NULL.");
    }
    if((*obj)->hexdigest) {
        wmkcMemoryFree((*obj)->hexdigest);
    }
    wmkcMemoryFree((*obj)->digest);
    wmkcMemoryFree((*obj));
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
