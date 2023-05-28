#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_IMAGE_PNG
#define WMKC_IMAGE_PNG
#include <zlib/zlib.h>
#include <wmkc_struct.h>

// PNG图像结构文件头
#define PNG_HEAD "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"
// PNG图像结构文件尾
#define PNG_IEND "\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82"

// wmkcPng对象
typedef struct wmkcPng_block {
    wmkcByte size[4];
    wmkcSize dataSize;
    wmkcByte *data;
    wmkcByte crc[4];
} wmkcPng_block;

/**
 * @brief 为wmkcPng对象申请内存空间
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcPng对象申请内存空间。
 * 
 * @note 未完成
 * @param obj wmkcPng对象的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcPng_new WMKC_OPEN_API
WMKC_OF((wmkcPng_block **obj));

/**
 * @brief 为wmkcPng对象释放内存空间
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcPng对象释放内存空间。
 * 
 * @note 未完成
 * @param obj wmkcPng对象的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcPng_free WMKC_OPEN_API
WMKC_OF((wmkcPng_block **obj));

#endif /* WMKC_IMAGE_PNG */
#endif /* WMKC_SUPPORT */
