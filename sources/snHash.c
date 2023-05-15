#include <snHash.h>

SN_PRIVATE(snError) _snHash_init
SN_FUNC_OF((snHash_ctx **obj, snSize size))
{
    (*obj)->digest = (snByte *)malloc(size);
    (*obj)->hexdigest = (snByte *)malloc((size << 1) + 1);
    if(!(*obj)->digest || !(*obj)->hexdigest) {
        return snErr_Memory;
    }
    (*obj)->hexdigest[size << 1] = 0x00;
    (*obj)->digestSize = size;
    (*obj)->hexdigestSize = size << 1;
    return snErr_OK;
}

SN_PUBLIC(snError) snHash_new SN_OPEN_API
SN_FUNC_OF((snHash_ctx **obj, sn_u32 hashType))
{
    if(!(*obj)) {
        if(!((*obj) = (snHash_ctx *)malloc(sizeof(snHash_ctx)))) {
            return snErr_Memory;
        }
    }

    static sn_u32 err_code;
    if(hashType == SN_HASH_TYPE_MD5) {
        (*obj)->md = EVP_md5();
        err_code = _snHash_init(obj, MD5_DIGEST_LENGTH);
    } else if(hashType == SN_HASH_TYPE_SHA1) {
        (*obj)->md = EVP_sha1();
        err_code = _snHash_init(obj, SHA_DIGEST_LENGTH);
    } else if(hashType == SN_HASH_TYPE_SHA224) {
        (*obj)->md = EVP_sha224();
        err_code = _snHash_init(obj, SHA224_DIGEST_LENGTH);
    } else if(hashType == SN_HASH_TYPE_SHA256) {
        (*obj)->md = EVP_sha256();
        err_code = _snHash_init(obj, SHA256_DIGEST_LENGTH);
    } else if(hashType == SN_HASH_TYPE_SHA384) {
        (*obj)->md = EVP_sha384();
        err_code = _snHash_init(obj, SHA384_DIGEST_LENGTH);
    } else if(hashType == SN_HASH_TYPE_SHA512) {
        (*obj)->md = EVP_sha512();
        err_code = _snHash_init(obj, SHA512_DIGEST_LENGTH);
    } else {
        return snErr_ErrType;
    }

    if(err_code == snErr_Memory) {
        return snErr_Memory;
    }

    return snErr_OK;
}

SN_PUBLIC(snError) snHash SN_OPEN_API
SN_FUNC_OF((snHash_ctx *hash, snByte *buf, snSize size))
{
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(md_ctx, hash->md, snNull);
    EVP_DigestUpdate(md_ctx, buf, size);
    EVP_DigestFinal_ex(md_ctx, hash->digest, snNull);
    EVP_MD_CTX_free(md_ctx);
    memcpy(hash->hexdigest, snBinascii_b2a_hex(hash->digest, hash->digestSize), hash->hexdigestSize);
    return snErr_OK;
}

SN_PUBLIC(snError) snHash_file SN_OPEN_API
SN_FUNC_OF((snHash_ctx *hash, const snChar *fn))
{
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

    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
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
    memcpy(hash->hexdigest, snBinascii_b2a_hex(hash->digest, hash->digestSize), hash->hexdigestSize);

    free(buf);
    fclose(fp);
    return snErr_OK;
}

SN_PUBLIC(snError) snHash_release SN_OPEN_API
SN_FUNC_OF((snHash_ctx **obj, sn_u32 instruction))
{
    if(instruction == SN_RELEASE_NORMAL) {
        free((*obj)->digest);
        free((*obj)->hexdigest);
        (*obj)->digest = snNull;
        (*obj)->hexdigest = snNull;
    }
    free((*obj));
    (*obj) = snNull;
    return snErr_OK;
}
