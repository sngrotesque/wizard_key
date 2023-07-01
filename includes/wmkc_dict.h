#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_DICT
#define WMKC_DICT
#include <wmkc_error.h>
#include <wmkc_common.h>
#include <wmkc_memory.h>

typedef struct wmkcDict_obj {
    wmkcChar delimiter;
    wmkcChar *key;
    wmkcChar *value;
} wmkcDict_obj;

/**
 * @brief 从字符串读取键值对并储存在wmkcDict对象中
 * @authors SN-Grotesque
 * @note 此函数会去掉键值对两边的空格
 * @param obj 指针，指向wmkcDict对象的地址
 * @param string 指针，指向字符串的地址
 * @param delimiter 这是一个字符，代表键值对的分隔符
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcDict_read WMKC_OPEN_API
WMKC_OF((wmkcDict_obj *obj, wmkcChar *string, wmkcChar delimiter));

/**
 * @brief 为wmkcDict对象申请内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcDict对象指针的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcDict_new WMKC_OPEN_API
WMKC_OF((wmkcDict_obj **obj));

/**
 * @brief 为wmkcDict对象释放内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcDict对象指针的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcDict_free WMKC_OPEN_API
WMKC_OF((wmkcDict_obj **obj));

#endif /* WMKC_DICT */
#endif /* WMKC_SUPPORT */
