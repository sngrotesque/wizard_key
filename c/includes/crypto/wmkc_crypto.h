#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_CRYPTO
#define WMKC_CRYPTO
#include <wmkc_random.h>

/**
 * @brief 将传入的sbox指针赋值为置换盒
 * @authors SN-Grotesque
 * 
 * 此函数通过调用wmkcRandom_seed函数设置随机数种子，
 * 使用for循环生成一个0至255的序列，再使用wmkcRandom_randint函数
 * 将这个序列打乱16次。
 * 
 * @note 后续使用wmkcRandom库来打乱序列。
 * @param sbox 指针，指向用于保存生成的置换盒结果，长度应为256字节。
 * @return 无
*/
WMKC_PUBLIC(wmkcVoid) wmkcCrypto_sbox_init WMKC_OPEN_API
WMKC_OF((wmkcByte *sbox));

/**
 * @brief 将传入的rsbox指针赋值为逆置换盒
 * @authors SN-Grotesque
 * 
 * 此函数通过sbox（置换盒）生成其对应的rsbox（逆置换盒）。
 * 
 * @note 无
 * @param sbox 指针，指向置换盒，长度应为256字节。
 * @param rsbox 指针，指向逆置换盒，长度应为256字节。
 * @return 无
*/
WMKC_PUBLIC(wmkcVoid) wmkcCrypto_rsbox_init WMKC_OPEN_API
WMKC_OF((wmkcByte *sbox, wmkcByte *rsbox));

#endif /* WMKC_CRYPTO */
#endif /* WMKC_SUPPORT */
