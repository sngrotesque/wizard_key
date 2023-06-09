#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_CHAT
#define WMKC_CHAT
#include <network/wmkc_net.h>
#include <crypto/snc.h>
#include <cjson/cJSON.h>

#include <wmkc_binascii.h>
#include <wmkc_object.h>
#include <wmkc_stream.h>
#include <wmkc_random.h>
#include <wmkc_struct.h>
#include <wmkc_common.h>
#include <wmkc_hash.h>
#include <wmkc_file.h>
#include <wmkc_misc.h>

#define WMKC_CHAT_USERNAME_SIZE 64 // 用户名的字节长度
#define WMKC_CHAT_PASSWORD_SIZE 32 // 密码的字节长度

#define WMKC_CHAT_HASH_SIZE 32 // 哈希摘要的十六进制串长度
#define WMKC_CHAT_SALT_SIZE 32 // 盐的长度

#define WMKC_CHAT_ITERATIONS 5771 // PKCS5_PBKDF2_HMAC函数迭代次数

#define WMKC_CHAT_CIPHER_MODE SNC_256 // SNC加密算法模式

typedef struct {
    wmkcSize  uid;    // 用户ID
    wmkcChar *name;   // 用户名
    wmkcByte *hash;   // 用户哈希值，用于查找用户
    wmkcByte *salt;   // 用户的盐，用于生成固定的密钥
    wmkcSNC_obj *snc; // 密码算法数据类型
} wmkcChat_obj;

/**
 * @brief 初始化对应用户的盐和密钥与初始向量
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcChat对象的地址
 * @param password 这是一个指针，指向用户的密码口令字符串的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PRIVATE(wmkcErr_obj) wmkcChat_initCipher WMKC_OPEN_API
WMKC_OF((wmkcChat_obj *obj, wmkcString password));

/**
 * @brief 给wmkcChat对象申请内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcChat对象指针的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcChat_new WMKC_OPEN_API
WMKC_OF((wmkcChat_obj **obj));

/**
 * @brief 释放wmkcChat对象的内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcChat对象指针的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcChat_free WMKC_OPEN_API
WMKC_OF((wmkcChat_obj **obj));

/**
 * @brief 创建用户并计算出其对应的数据内容
 * @authors SN-Grotesque
 * @note 对于UID，目前的生成方式不能采用，请后续修改
 * @param obj 这是一个指针，指向wmkcChat对象的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcChat_signup WMKC_OPEN_API
WMKC_OF((wmkcChat_obj *obj));

/**
 * @brief 生成用户哈希摘要
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcChat对象的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcChat_getUserHash WMKC_OPEN_API
WMKC_OF((wmkcChat_obj *obj));

#endif /* WMKC_CHAT */
#endif /* WMKC_SUPPORT */
