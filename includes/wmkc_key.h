#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_KEY
#define WMKC_KEY

#include <wmkc_base.h>
#include <wmkc_file.h>
#include <wmkc_random.h>

#define WMKC_KEY_CRYPTO_BLOCKLEN 4
#define WMKC_KEY_CRYPTO_NR 13
#define WMKC_KEY_CRYPTO_NB 2
#define WMKC_KEY_CRYPTO_NK 2
#define WMKC_KEY_GROUP 64
#define WMKC_KEY_MODE_ENCRYPT true
#define WMKC_KEY_MODE_DECRYPT false
#define WMKC_KEY_END_PK "-----END SN PUBLIC KEY-----"
#define WMKC_KEY_END_SK "-----END SN PRIVATE KEY-----"
#define WMKC_KEY_BEGIN_PK "-----BEGIN SN PUBLIC KEY-----"
#define WMKC_KEY_BEGIN_SK "-----BEGIN SN PRIVATE KEY-----"

typedef wmkcByte wmkcKey_state[WMKC_KEY_CRYPTO_NB][WMKC_KEY_CRYPTO_NK];
typedef struct {
    wmkc_u32 mode; // true: Encrypt, false: Decrypt
    wmkc_u32 size; // 密钥长度
    wmkcByte *key; // 密钥指针
} wmkcKey_obj;

/**
 * @brief 为wmkcKey对象申请内存空间
 * @authors SN-Grotesque
 * 
 * 为wmkcKey对象申请内存空间并初始化
 * 
 * @note 如果key或者keySize为空，那么mode参数无效。
 * @param obj 这是一个指针，指向wmkcKey对象指针的地址。
 * @param key 这是一个指针，指向密钥数据的地址，可以为空。
 * @param keySize 这是一个长度，为key指针指向的密钥数据的长度。
 * @param mode 这是一个模式，1为加密，0为解密。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcKey_new WMKC_OPEN_API
WMKC_OF((wmkcKey_obj **obj, wmkcByte *key, wmkc_u32 keySize, wmkc_u32 mode));

/**
 * @brief 为wmkcKey对象释放内存空间
 * @authors SN-Grotesque
 * 
 * 为wmkcKey对象释放内存空间
 * 
 * @note 无
 * @param obj 这是一个指针，指向wmkcKey对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcKey_free WMKC_OPEN_API
WMKC_OF((wmkcKey_obj **obj));

/**
 * @brief 将密钥数据进行混淆与弱加密
 * @authors SN-Grotesque
 * 
 * 混淆与弱加密wmkcKey对象key成员指针指向的密钥
 * 
 * @note 此函数只能处理长度为4的倍数的数据。
 * @param obj 这是一个指针，指向wmkcKey对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcKey_CryptKey WMKC_OPEN_API
WMKC_OF((wmkcKey_obj *obj));

/**
 * @brief 导入密钥
 * @authors SN-Grotesque
 * 
 * 根据指定的路径导入密钥，如果obj->mode等于false，那么会同时进行解密。
 * 
 * @note 未完成
 * @param obj 这是一个指针，指向wmkcKey对象指针的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcKey_loadKey WMKC_OPEN_API
WMKC_OF((wmkcKey_obj *obj, wmkcString fn));

/**
 * @brief 写入密钥
 * @authors SN-Grotesque
 * 
 * 根据指定的路径写入密钥，如果obj->mode等于true，那么会同时进行加密。
 * 
 * @note 未完成
 * @param obj 这是一个指针，指向wmkcKey对象指针的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcKey_saveKey WMKC_OPEN_API
WMKC_OF((wmkcKey_obj *obj, wmkcString fn));

#endif /* WMKC_KEY */
#endif /* WMKC_SUPPORT */
