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

#define SN_HASH_BLOCKLEN      0xfffffff // 256 MB

typedef struct {
    const EVP_MD *md;
    sn_u32 digestSize;
    sn_u32 hexdigestSize;
    snByte *digest;
    snByte *hexdigest;
} snHash_ctx;

typedef enum {
    snHash_HashType_MD5,
    snHash_HashType_SHA1,
    snHash_HashType_SHA224,
    snHash_HashType_SHA256,
    snHash_HashType_SHA384,
    snHash_HashType_SHA512
} snHash_HashType;

SN_PRIVATE_CONST(sn_u32) snHash_SIZE[6] = {
    16,  // MD5
    20,  // SHA-1
    28,  // SHA-224
    32,  // SHA-256
    48,  // SHA-384
    64   // SHA-512
};

// 初始化snHash对象
SN_PUBLIC(snError) snHash_new SN_OPEN_API
SN_FUNC_OF((snHash_ctx **obj, snHash_HashType hashType));

// 根据你选择的算法进行哈希计算
SN_PUBLIC(snError) snHash SN_OPEN_API
SN_FUNC_OF((snHash_ctx *hash, snByte *buf, snSize size));

// 根据你选择的算法对文件进行哈希计算
SN_PUBLIC(snError) snHash_file SN_OPEN_API
SN_FUNC_OF((snHash_ctx *hash, snFileStr fn));

// 释放掉snHash对象
SN_PUBLIC(snError) snHash_free SN_OPEN_API
SN_FUNC_OF((snHash_ctx **obj));

#endif // #ifndef __SN_HASH__
