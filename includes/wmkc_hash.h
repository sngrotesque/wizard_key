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
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. All advertising materials mentioning features or use of this
*    software must display the following acknowledgment: "This product
*    includes software developed by the OpenSSL Project for use in the
*    OpenSSL Toolkit. (http://www.openssl.org/)"
* 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be
*    used to endorse or promote products derived from this software
*    without prior written permission. For written permission, please
*    contact openssl-core@openssl.org.
* 5. Products derived from this software may not be called "OpenSSL"
*    nor may "OpenSSL" appear in their names without prior written
*    permission of the OpenSSL Project.
* 6. Redistributions of any form whatsoever must retain the following
*    acknowledgment: "This product includes software developed by the
*    OpenSSL Project for use in the OpenSSL Toolkit (http://www.openssl.org/)"
*/
#ifndef WMKC_HASH
#define WMKC_HASH
#include <wmkc_file.h>
#include <wmkc_binascii.h>
#include <openssl/evp.h>

#define SN_HASH_BLOCKLEN 0xfffffff // 256 MB

typedef struct {
    const EVP_MD *md;
    wmkc_u32 digestSize;
    wmkc_u32 hexdigestSize;
    wmkcByte *digest;
    wmkcByte *hexdigest;
} wmkcHash_obj;

typedef enum {
    wmkcHash_HashType_MD5,
    wmkcHash_HashType_SHA1,
    wmkcHash_HashType_SHA224,
    wmkcHash_HashType_SHA256,
    wmkcHash_HashType_SHA384,
    wmkcHash_HashType_SHA512
} wmkcHash_HashType;

WMKC_PRIVATE_CONST(wmkc_u32) wmkcHash_SIZE[6] = {
    16,  // MD5
    20,  // SHA-1
    28,  // SHA-224
    32,  // SHA-256
    48,  // SHA-384
    64   // SHA-512
};

// 初始化wmkcHash对象
WMKC_PUBLIC(wmkcErr_obj) wmkcHash_new WMKC_OPEN_API
WMKC_OF((wmkcHash_obj **obj, wmkcHash_HashType hashType));

// 根据你选择的算法进行哈希计算
WMKC_PUBLIC(wmkcErr_obj) wmkcHash WMKC_OPEN_API
WMKC_OF((wmkcHash_obj *hash, wmkcByte *buf, wmkcSize size));

// 根据你选择的算法对文件进行哈希计算
WMKC_PUBLIC(wmkcErr_obj) wmkcHash_file WMKC_OPEN_API
WMKC_OF((wmkcHash_obj *hash, wmkcFileString fn));

// 释放掉wmkcHash对象
WMKC_PUBLIC(wmkcErr_obj) wmkcHash_free WMKC_OPEN_API
WMKC_OF((wmkcHash_obj **obj));

#endif
