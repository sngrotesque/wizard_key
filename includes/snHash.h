/*
* This software uses the OpenSSL library for Hash algorithm. OpenSSL is
* licensed under the Apache License 2.0. The following is the original
* copyright and license notice of OpenSSL:
* 
* Copyright (c) 1998-2021 The OpenSSL Project
* Copyright (c) 1995-1998 Eric A. Young, Tim J. Hudson
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in
* the documentation and/or other materials provided with the
* distribution.
* 3. All advertising materials mentioning features or use of this
* software must display the following acknowledgment: "This product
* includes software developed by the OpenSSL Project for use in the
* OpenSSL Toolkit. (http://www.openssl.org/)"
* 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be
* used to endorse or promote products derived from this software
* without prior written permission. For written permission, please
* contact openssl-core@openssl.org.
* 5. Products derived from this software may not be called "OpenSSL"
* nor may "OpenSSL" appear in their names without prior written
* permission of the OpenSSL Project.
* 6. Redistributions of any form whatsoever must retain the following
* acknowledgment: "This product includes software developed by the
* OpenSSL Project for use in the OpenSSL Toolkit (http://www.openssl.org/)"
*/
#ifndef __SN_HASH__
#define __SN_HASH__
#include <snFile.h>
#include <snBinascii.h>
#include <openssl/evp.h>

#ifndef MD5_DIGEST_LENGTH
#define MD5_DIGEST_LENGTH 16
#endif
#ifndef SHA_DIGEST_LENGTH
#define SHA_DIGEST_LENGTH 20
#endif
#ifndef SHA224_DIGEST_LENGTH
#define SHA224_DIGEST_LENGTH 28
#endif
#ifndef SHA256_DIGEST_LENGTH
#define SHA256_DIGEST_LENGTH 32
#endif
#ifndef SHA384_DIGEST_LENGTH
#define SHA384_DIGEST_LENGTH 48
#endif
#ifndef SHA512_DIGEST_LENGTH
#define SHA512_DIGEST_LENGTH 64
#endif

// 以digest的位长度来区分
#define SN_HASH_TYPE_MD5      0x0080 // 16
#define SN_HASH_TYPE_SHA1     0x00a0 // 20
#define SN_HASH_TYPE_SHA224   0x00e0 // 28
#define SN_HASH_TYPE_SHA256   0x0100 // 32
#define SN_HASH_TYPE_SHA384   0x0180 // 48
#define SN_HASH_TYPE_SHA512   0x0200 // 64

#define SN_HASH_BLOCKLEN      0xfffffff // 256 MB

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
SN_FUNC_OF((snHash_ctx *hash, snString fn));

// 释放掉snHash对象
SN_PUBLIC(snError) snHash_release SN_OPEN_API
SN_FUNC_OF((snHash_ctx **obj, sn_u32 instruction));

#endif // #ifndef __SN_HASH__
