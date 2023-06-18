/**
 * 后续重新设计一下这个库，让它变得更加易用一些。
 * 针对于Windows系统的一些文本编码兼容问题，想想办法解决。
*/
#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_FILE
#define WMKC_FILE
#include <wmkc_coder.h>

// 文件的分组大小
#define WMKC_FILE_BLOCKLEN 4096

#if defined(WMKC_PLATFORM_WINOS)
#include <Windows.h>
#include <shlwapi.h> // -lshlwapi
#elif defined(WMKC_PLATFORM_LINUX)
#include <sys/stat.h>
#include <unistd.h>
#endif /* WMKC_PLATFORM_WINOS */

/**
 * @brief 创建一个文件指针
 * @authors SN-Grotesque
 * @note 此函数在Windows系统中使用_wfopen函数创建文件指针
 * @param fn 这是一个字符串，必须是UTF-8编码，为文件路径
 * @param mode 这是一个字符串，必须是UTF-8编码，为文件打开模式
 * @return 返回一个文件指针
 */
WMKC_PUBLIC(FILE *) wmkcFile_fopen WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn, wmkcCSTR mode));

/**
 * @brief 检查是否存在指定的路径
 * @authors SN-Grotesque
 * 
 * 此函数通过调用系统函数进行检查路径是否存在。
 * 
 * @note 无
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcBool值，true表示路径存在，false表示不存在。
*/
WMKC_PUBLIC(wmkcBool) wmkcFile_exists WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn));

/**
 * @brief 获取文件的大小
 * @authors SN-Grotesque
 * 
 * 此函数通过调用系统函数获取文件的大小。
 * 
 * @note 无
 * @param size 这是一个指针，指向wmkcSize类型变量的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fileSize WMKC_OPEN_API
WMKC_OF((wmkcSize *size, wmkcCSTR fn));

WMKC_PUBLIC(wmkcErr_obj) wmkcFile_readline WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcCSTR fn))
{

}

/**
 * @brief 完整读取一个文件的内容
 * @authors SN-Grotesque
 * @note 此函数不应用于单次读取超大文件，以免内存空间告急。
 * @param buf 这是一个指针，指向缓冲区指针的地址
 * @param size 这是一个指针，指向长度的地址
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fread WMKC_OPEN_API
WMKC_OF((wmkcByte **buf, wmkcSize *size, wmkcCSTR fn));

/**
 * @brief 将缓冲区的数据完整写入一个文件
 * @authors SN-Grotesque
 * @note 无
 * @param buf 这是一个指针，指向缓冲区的地址
 * @param size 这是一个长度，代表缓冲区的长度
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fwrite WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size, wmkcCSTR fn));


#endif /* WMKC_FILE */
#endif /* WMKC_SUPPORT */
