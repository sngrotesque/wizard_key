#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_HEXDUMP
#define WMKC_HEXDUMP
#include <wmkc_file.h>

#ifndef WMKC_HEXDUMP_BLOCKLEN
#define WMKC_HEXDUMP_BLOCKLEN 32
#endif

/**
 * @brief 以十六进制打印文件完整内容
 * @authors SN-Grotesque
 * 
 * 以十六进制打印文件的完整内容，每一行的长度为WMKC_HEXDUMP_BLOCKLEN。
 * 
 * @note 无
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcHexdump WMKC_OPEN_API
WMKC_OF((wmkcFileString fn));

#endif /* WMKC_HEXDUMP */
#endif /* WMKC_SUPPORT */
