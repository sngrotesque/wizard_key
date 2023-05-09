#ifndef __SN_HASH__
#define __SN_HASH__
#include <snFile.h>
#include <snBinascii.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/evp.h>

// 以digest的位长度来区分
#define SN_HASH_TYPE_MD5      0x0080
#define SN_HASH_TYPE_SHA1     0x00a0
#define SN_HASH_TYPE_SHA224   0x00e0
#define SN_HASH_TYPE_SHA256   0x0100
#define SN_HASH_TYPE_SHA384   0x0180
#define SN_HASH_TYPE_SHA512   0x0200

#define SN_HASH_BLOCKLEN      0xfffffff

typedef struct {
    const EVP_MD *md;
    sn_u32 digestSize;
    sn_u32 hexdigestSize;
    snByte *digest;
    snByte *hexdigest;
} snHash_ctx;

/**
 * hashType (按照算法的digest位长度划分):
 *      md5: 0x080, sha1: 0x0a0, sha224: 0x0e0, sha256: 0x100, sha384: 0x180, sha512: 0x200
*/
SN_PUBLIC(snError) snHash_new SN_OPEN_API
SN_FUNC_OF((snHash_ctx **obj, sn_u32 hashType));

// 根据你需要的算法进行哈希计算
SN_PUBLIC(snError) snHash SN_OPEN_API
SN_FUNC_OF((snHash_ctx *hash, snByte *buf, snSize size));

SN_PUBLIC(snError) snHash_file SN_OPEN_API
SN_FUNC_OF((snHash_ctx *hash, const snChar *fn));

// 释放掉snHash对象
SN_PUBLIC(snError) snHash_release SN_OPEN_API
SN_FUNC_OF((snHash_ctx **obj, sn_u32 instruction));

#endif // #ifndef __SN_HASH__
