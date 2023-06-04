/**
 * 后续重新设计一下这个库，让它变得更加易用一些。
 * 针对于Windows系统的一些文本编码兼容问题，想想办法解决。
*/
#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_FILE
#define WMKC_FILE

// 文件的分组大小
#define WMKC_FILE_BLOCKLEN 4096

#if defined(WMKC_PLATFORM_WINOS)
#include <Windows.h>
#include <shlwapi.h> // -lshlwapi
#define wmkcFile_text(x) WMKC_WTEXT(x)
#define wmkcFile_fopen(fn, mode) _wfopen(fn, mode)
typedef LPCWSTR _wmkcFileString;
#elif defined(WMKC_PLATFORM_LINUX)
#include <sys/stat.h>
#include <unistd.h>
#define wmkcFile_text(x) WMKC_WTEXT(x)
#define wmkcFile_fopen(fn, mode) fopen(fn, mode)
typedef wmkcString _wmkcFileString;
#endif

typedef _wmkcFileString wmkcFileString;
typedef struct {
    wmkcByte *data;    // 文件数据
    wmkcSize size;     // 文件长度
    wmkcSize quotient; // 文件长度除块大小的商
    wmkcSize leftover; // 文件剩余长度
} wmkcFile_obj;

/**
 * @brief 为wmkcFile对象申请内存空间
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcFile对象申请内存空间。
 * 
 * @note 无
 * @param obj 这是一个指针，指向wmkcFile对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_new WMKC_OPEN_API
WMKC_OF((wmkcFile_obj **obj));

/**
 * @brief 为wmkcFile对象释放内存空间
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcFile对象释放内存空间。
 * 
 * @note 无
 * @param obj 这是一个指针，指向wmkcFile对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_free WMKC_OPEN_API
WMKC_OF((wmkcFile_obj **obj));

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
WMKC_OF((wmkcFileString fn));

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
WMKC_OF((wmkcSize *size, wmkcFileString fn));

/**
 * @brief 完整读取一个文件的内容
 * @authors SN-Grotesque
 * 
 * 此函数获取文件的大小后分组将文件的完整内容读取到内存中。
 * 
 * @note 此函数不应用于单次读取超大文件，以免内存空间告急。
 * @param obj 这是一个指针，指向wmkcFile对象的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fread WMKC_OPEN_API
WMKC_OF((wmkcFile_obj *obj, wmkcFileString fn));

/**
 * @brief 将缓冲区的数据完整写入一个文件
 * @authors SN-Grotesque
 * 
 * 此函数将缓冲区的数据分组，并且完整写入至一个文件中。
 * 
 * @note 无
 * @param obj 这是一个指针，指向wmkcFile对象的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fwrite WMKC_OPEN_API
WMKC_OF((wmkcFile_obj *obj, wmkcFileString fn));

#endif /* WMKC_FILE */
#endif /* WMKC_SUPPORT */
