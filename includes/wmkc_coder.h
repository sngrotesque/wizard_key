#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_CODER
#define WMKC_CODER
#include <iconv/iconv.h>
#include <wmkc_common.h>

/**
 * @brief 字符串转换编码函数
 * @authors SN-Grotesque
 * @note 无
 * @param dst 这是一个指针，指向结果指针的地址
 * @param dst 这是一个指针，指向源编码字符串的地址
 * @param code_string 这是一个字符串，为源编码转目标编码，格式为：目标编码<源编码，
 *         示例：GBK<UTF-8
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcCoder_encoding WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *src, wmkcChar *code_string));

#endif /* WMKC_CODER */
#endif /* WMKC_SUPPORT */
