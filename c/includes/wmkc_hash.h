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
#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_HASH
#define WMKC_HASH
#include <wmkc_error.h>
#include <wmkc_memory.h>
#include <wmkc_file.h>
#include <wmkc_binascii.h>
#include <openssl/evp.h>

// 文件缓冲区块长度：256 MB
#define SN_HASH_BLOCKLEN 0xfffffff

// wmkcHash对象
typedef struct {
    const EVP_MD *md;
    wmkc_u32 digestSize;
    wmkc_u32 hexdigestSize;
    wmkcByte *digest;
    wmkcByte *hexdigest;
} wmkcHash_obj;

// wmkcHash对象的哈希算法类型
typedef enum {
    wmkcHash_MD5,    wmkcHash_SHA1,   wmkcHash_SHA224,
    wmkcHash_SHA256, wmkcHash_SHA384, wmkcHash_SHA512
} wmkcHash_Type;

/**
 * @brief 为wmkcHash对象申请内存
 * @authors SN-Grotesque
 *
 * 此函数创建一个新的哈希对象，并使用指定的哈希算法类型对其进行初始化。
 *
 * @note 无
 * @param obj 指针，指向wmkcHash对象指针的地址。
 * @param hashType 为哈希算法类型，此值必须介于[0, 5]。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
**/
WMKC_PUBLIC(wmkcErr_obj) wmkcHash_new WMKC_OPEN_API
WMKC_OF((wmkcHash_obj **obj, wmkcHash_Type hashType));

/**
 * @brief 求出一段完整数据的哈希值
 * @authors SN-Grotesque
 *
 * 此函数得到传入的数据的哈希值后赋值到wmkcHash对象中。
 *
 * [EVP_MD_CTX_new](https://www.openssl.org/docs/man1.1.1/man3/EVP_MD_CTX_new.html)
 * @note 对于OpenSSL库提供的EVP相关功能，我查阅到OpenSSL库中的提示是
 *       返回值为1时表示无错误，为0时表示错误。
 *       为了不与本库产生冲突，暂时不对EVP相关功能进行返回值检查。
 * @param hash 指针，指向wmkcHash对象的地址。
 * @param buf 指针，指向一段数据的地址。
 * @param suze 这是一个长度，代表buf指向的内容的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
**/
WMKC_PUBLIC(wmkcErr_obj) wmkcHash WMKC_OPEN_API
WMKC_OF((wmkcHash_obj *hash, wmkcByte *buf, wmkcSize size));

/**
 * @brief 获取指定文件的哈希值
 * @authors SN-Grotesque
 *
 * 此函数获取指定文件的哈希值后赋值到wmkcHash对象中。
 * 当然你需要使用wmkcHash_new函数初始化一个新的wmkcHash对象。
 *
 * [EVP_MD_CTX_new](https://www.openssl.org/docs/man1.1.1/man3/EVP_MD_CTX_new.html)
 * @note 对于OpenSSL库提供的EVP相关功能，我查阅到OpenSSL库中的提示是
 *       返回值为1时表示无错误，为0时表示错误。
 *       为了不与本库产生冲突，暂时不对EVP相关功能进行返回值检查。
 * @param hash 指针，指向wmkcHash对象的地址。
 * @param fn 指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
**/
WMKC_PUBLIC(wmkcErr_obj) wmkcHash_file WMKC_OPEN_API
WMKC_OF((wmkcHash_obj *hash, wmkcCSTR fn));

/**
 * @brief 为wmkcHash对象释放内存
 * @authors SN-Grotesque
 *
 * 此函数为wmkcHash对象释放内存。
 *
 * @note 无
 * @param obj 指针，指向wmkcHash对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
**/
WMKC_PUBLIC(wmkcErr_obj) wmkcHash_free WMKC_OPEN_API
WMKC_OF((wmkcHash_obj **obj));

#endif /* WMKC_HASH */
#endif /* WMKC_SUPPORT */
