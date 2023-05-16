/*
* Copyright belongs to SN-Grotesque，版权所属于SN-Grotesque
* 允许使用，但请遵守Apache2.0协议。
*
* SNC是由SN-Grotesque开发的一个对称加密算法，它使用SP网络对数据进行分组加密与解密。
* 解密就是加密过程的逆运算，只不过加密是从第一轮子密钥直到最后一轮子密钥，解密与之相反。
*
* 此算法的区块长度为32字节（256位）。
* 密钥采用三种长度：256，512，768。
*     基础长度（256位）的密钥将进行7轮加密或解密。
*     扩展长度（512位）的密钥将进行8轮加密或解密。
*     扩展长度（768位）的密钥将进行9轮加密或解密。
*
* 目前提供了以下加密模式：ECB, CBC
* 请注意，ECB模式在大多数情况下不被认为是足够安全的。
*
* 目前CBC模式性能测试结果（截止于北京时间2023年4月30日14:07）：
* Current CBC mode performance test results (as of 14:07 Beijing time on April 30, 2023):
*     平台（Platform）:
*         OS:  Parrot Sec OS 5.2 amd64
*         CPU: Intel(R) Core(TM) i5-4200U CPU @ 1.60GHz
*         Mem: DDR3 1600MHZ 8GB (4 + 4)
*         模式（Mode）:
*             长度为16777216字节，全零（the length is 16777216 Bytes. All zero.）
*             GCC添加参数'-Ofast', '--std=c18'（GCC add parameter '-Ofast', '--std=c18'.）
*             此时间是取自15次加密的平均值（The time is taken from the average of 15 encryption attempts.）
*     SNC_256    0.34 seconds.
*     SNC_512    0.44 seconds
*     SNC_768    0.55 seconds
*/
#ifndef __SNC_H__
#define __SNC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <snConf.h>

#define SNC_BLOCKLEN 32 // 区块长度（Block size）
#define SNC_NB 8 // 纵向长度（Longitudinal length）
#define SNC_NK 4 // 横向长度（Transverse length）

//* 加解密轮数（Encryption or Decryption number of rounds.）
SN_PRIVATE_CONST(snByte) SNC_NR[3] = {
    0x7, // 7 round
    0x8, // 8 round
    0x9  // 9 round
};

//* 密钥长度（Key Length.）
SN_PRIVATE_CONST(snByte) SNC_KN[3] = {
    0x20, // 32 bytes
    0x40, // 64 bytes
    0x60  // 96 bytes
};

typedef enum {
    SNC_256, // 密钥长度：32字节（Key length: 32 bytes）
    SNC_512, // 密钥长度：64字节（Key length: 64 bytes）
    SNC_768  // 密钥长度：96字节（Key length: 96 bytes）
} SNC_mode;

// 声明区块类型（declare block type）
typedef snByte sncState[SNC_NB][SNC_NK];
// 声明SNC算法的数据结构（declare SNC structure）
typedef struct {
    sn_u16 NR;
    sn_u16 KN;
    SNC_mode mode;
    snByte iv[SNC_BLOCKLEN];
    snByte roundKey[864];
} SNC_ctx;

// 为SNC对象申请内存空间
SN_PUBLIC(snError) SNC_new SN_OPEN_API
SN_FUNC_OF((SNC_ctx **ctx, SNC_mode mode));

// 释放SNC对象
SN_PUBLIC(snError) SNC_release SN_OPEN_API
SN_FUNC_OF((SNC_ctx **ctx));

// 初始化SNC对象
SN_PUBLIC(snVoid) SNC_init SN_OPEN_API
SN_FUNC_OF((SNC_ctx *ctx, snByte *keyBuf, snByte *ivBuf));

// ECB加密
SN_PUBLIC(snVoid) SNC_ECB_Encrypt SN_OPEN_API
SN_FUNC_OF((SNC_ctx *ctx, snByte *buf, snSize size));

// ECB解密
SN_PUBLIC(snVoid) SNC_ECB_Decrypt SN_OPEN_API
SN_FUNC_OF((SNC_ctx *ctx, snByte *buf, snSize size));

// CBC加密
SN_PUBLIC(snVoid) SNC_CBC_Encrypt SN_OPEN_API
SN_FUNC_OF((SNC_ctx *ctx, snByte *buf, snSize size));

// CBC解密
SN_PUBLIC(snVoid) SNC_CBC_Decrypt SN_OPEN_API
SN_FUNC_OF((SNC_ctx *ctx, snByte *buf, snSize size));

#ifdef __cplusplus
}
#endif

#endif // #ifndef __SNC_H__