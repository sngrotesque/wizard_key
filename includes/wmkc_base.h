#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_BASE
#define WMKC_BASE
#include <wmkc_object.h>

// Base64的填充值
#define WMKC_BASE64_PAD '='

// wmkcBase64对象
typedef struct {
    wmkc_obj *src;
    wmkc_obj *dst;
    
} wmkcBase64_obj;

/**
 * @brief 获取数据经过Base64编码后的长度
 * @authors SN-Grotesque
 * 
 * 此函数通过获取wmkc对象的size成员来计算编码后的长度
 * 也就是说size成员必须被赋值为正确的值。
 * 
 * @note 此函数无需改动，如果需要进行错误检查，那么请在编码函数中进行。
 * @param src 此参数是一个wmkc对象，此函数通过此对象的size成员获取编码后的长度。
 * @return 此函数返回一个wmkcSize类型变量，代表编码后的长度值。
*/
WMKC_PUBLIC(wmkcSize) wmkcBase64_encode_size WMKC_OPEN_API
WMKC_OF((wmkc_obj *src));

/**
 * @brief 获取数据经过Base64解码后的长度
 * @authors SN-Grotesque
 * 
 * 此函数通过获取wmkc对象的size成员来计算编码后的长度
 * 也就是说size成员必须被赋值为正确的值。
 * 
 * @note 此函数无需进行错误检查（请在解码函数进行），此函数
 *       应该可以再优化，但目前未想到好的方案。
 * @param src 此参数是一个wmkc对象，此函数通过此对象的size成员获取解码后的长度。
 * @return 此函数返回一个wmkcSize类型变量，代表解码后的长度值。
*/
WMKC_PUBLIC(wmkcSize) wmkcBase64_decode_size WMKC_OPEN_API
WMKC_OF((wmkc_obj *src));

/**
 * @brief 新建一个wmkcBase64对象
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcBase64对象申请内存空间。
 * 
 * @note 无。
 * @param obj 此参数是一个wmkcBase64对象的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_new WMKC_OPEN_API
WMKC_OF((wmkcBase64_obj **obj));

/**
 * @brief 释放一个wmkcBase64对象
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcBase64对象释放内存空间。
 * 
 * @note 无。
 * @param obj 此参数是一个wmkcBase64对象的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_free WMKC_OPEN_API
WMKC_OF((wmkcBase64_obj **obj));

/**
 * @brief Base64编码函数
 * @authors SN-Grotesque
 * 
 * 此函数将src中的内容编码后赋值到dst对象中。
 * 
 * @note 无。
 * @param src 此参数是一个wmkc对象，应至少包含数据与数据长度。
 * @param dst 此参数是一个wmkc对象，应至少包含数据与数据长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_Encode WMKC_OPEN_API
WMKC_OF((wmkc_obj *dst, wmkc_obj *src));

/**
 * @brief Base64解码函数
 * @authors SN-Grotesque
 * 
 * 此函数将src中的内容解码后赋值到dst对象中。
 * 
 * @note 无。
 * @param src 此参数是一个wmkc对象，应至少包含数据与数据长度。
 * @param dst 此参数是一个wmkc对象，应至少包含数据与数据长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_Decode WMKC_OPEN_API
WMKC_OF((wmkc_obj *dst, wmkc_obj *src));

#endif /* WMKC_BASE */
#endif /* WMKC_SUPPORT */
