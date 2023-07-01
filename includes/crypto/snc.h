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
*         OS:  Windows 10.0 x64 22H2
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

/*
* 这个加密算法的注释没必要转为Doxygen风格的注释，当然，要是谁有闲心的话
* 可以考虑考虑帮我把注释改了。
*/

#ifndef WMKC_SNC
#define WMKC_SNC
#include <wmkc_conf.h>
#include <wmkc_error.h>
#include <wmkc_memory.h>
#ifdef __cplusplus
extern "C" {
#endif

#define SNC_BLOCKLEN 32 // 区块长度（Block size）
#define SNC_NB 8 // 纵向长度（Longitudinal length）
#define SNC_NK 4 // 横向长度（Transverse length）

//* 加解密轮数（Encryption or Decryption number of rounds.）
WMKC_PRIVATE_CONST(wmkcByte) SNC_NR[3] = {7, 8, 9};

//* 密钥长度（Key Length (Bytes).）
WMKC_PRIVATE_CONST(wmkcByte) SNC_KN[3] = {32, 64, 96};

//* 密码类型（Cipher type.）
typedef enum {SNC_256, SNC_512, SNC_768} SNC_mode;

// 声明区块类型（declare block type）
typedef wmkcByte sncState[SNC_NB][SNC_NK];
// 声明SNC算法的数据结构（declare SNC structure）
typedef struct {
    wmkc_u16 NR;
    wmkc_u16 KN;
    SNC_mode mode;
    wmkcByte iv[SNC_BLOCKLEN];
    wmkcByte roundKey[864];
} wmkcSNC_obj;

/**
 * @brief 为SNC对象申请内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param ctx 指针，指向wmkcSNC对象指针的地址
 * @param mode 这是需要使用的模式：SNC_256, SNC_512, SNC_768
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcSNC_new WMKC_OPEN_API
WMKC_OF((wmkcSNC_obj **ctx, SNC_mode mode));

/**
 * @brief 为SNC对象释放内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param ctx 指针，指向wmkcSNC对象指针的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcSNC_free WMKC_OPEN_API
WMKC_OF((wmkcSNC_obj **ctx));

/**
 * @brief 初始化wmkcSNC对象
 * @authors SN-Grotesque
 * @note 无
 * @param ctx 指针，指向wmkcSNC对象的地址
 * @param keyBuf 指针，指向密钥的地址
 * @param ivBuf 指针，指向初始向量的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcSNC_init WMKC_OPEN_API
WMKC_OF((wmkcSNC_obj *ctx, wmkcByte *keyBuf, wmkcByte *ivBuf));

/**
 * @brief SNC-ECB，加密
 * @authors SN-Grotesque
 * @note 无
 * @param ctx 指针，指向wmkcSNC对象的地址
 * @param buf 指针，指向缓冲区的地址
 * @param size 代表缓冲区的长度
 * @return 无
 */
WMKC_PUBLIC(wmkcVoid) wmkcSNC_ecb_encrypt WMKC_OPEN_API
WMKC_OF((wmkcSNC_obj *ctx, wmkcByte *buf, wmkcSize size));

/**
 * @brief SNC-ECB，解密
 * @authors SN-Grotesque
 * @note 无
 * @param ctx 指针，指向wmkcSNC对象的地址
 * @param buf 指针，指向缓冲区的地址
 * @param size 代表缓冲区的长度
 * @return 无
 */
WMKC_PUBLIC(wmkcVoid) wmkcSNC_ecb_decrypt WMKC_OPEN_API
WMKC_OF((wmkcSNC_obj *ctx, wmkcByte *buf, wmkcSize size));

/**
 * @brief SNC-CBC，加密
 * @authors SN-Grotesque
 * @note 无
 * @param ctx 指针，指向wmkcSNC对象的地址
 * @param buf 指针，指向缓冲区的地址
 * @param size 代表缓冲区的长度
 * @return 无
 */
WMKC_PUBLIC(wmkcVoid) wmkcSNC_cbc_encrypt WMKC_OPEN_API
WMKC_OF((wmkcSNC_obj *ctx, wmkcByte *buf, wmkcSize size));

/**
 * @brief SNC-CBC，解密
 * @authors SN-Grotesque
 * @note 无
 * @param ctx 指针，指向wmkcSNC对象的地址
 * @param buf 指针，指向缓冲区的地址
 * @param size 代表缓冲区的长度
 * @return 无
 */
WMKC_PUBLIC(wmkcVoid) wmkcSNC_cbc_decrypt WMKC_OPEN_API
WMKC_OF((wmkcSNC_obj *ctx, wmkcByte *buf, wmkcSize size));

#ifdef __cplusplus
}
#endif

#endif // #ifndef WMKC_SNC