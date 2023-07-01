/**
 * 后续重新设计一下这个库，让它变得更加易用一些。
 * 针对于Windows系统的一些文本编码兼容问题，想想办法解决。
 * 
 * 2023.06.28
 * 又出现了新的问题，这个库的完善好麻烦。啊啊啊啊啊啊啊啊
*/
#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_FILE
#define WMKC_FILE
#include <wmkc_error.h>
#include <wmkc_coder.h>
#include <wmkc_memory.h>

#define WMKC_FILE_BLOCKLEN   4096 // 文件的分组大小
#define WMKC_FILE_MODE_FAIL  -1LL // 文件模式：（打开）失败
#define WMKC_FILE_MODE_READ  0x01 // 文件模式：读取
#define WMKC_FILE_MODE_WRITE 0x02 // 文件模式：写入

#if defined(WMKC_PLATFORM_WINOS)
#include <Windows.h>
#include <shlwapi.h> // -lshlwapi
#elif defined(WMKC_PLATFORM_LINUX)
#include <sys/stat.h>
#include <unistd.h>
#endif /* WMKC_PLATFORM_WINOS */

typedef struct {
    FILE *fp;         // 文件指针
    wmkcCSTR fn;      // 文件名
    wmkcSSize status; // 文件状态：读取，写入，失败
    wmkcSize fsize;   // 文件长度
} wmkcFile_obj;

/**
 * @brief 检测路径是否存在的函数
 * @authors SN-Grotesque
 * @note 无
 * @param fn 路径
 * @return 返回一个wmkcBool值，为True表示路径存在。
 */
WMKC_PUBLIC(wmkcBool) wmkcFile_exists WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn));

/**
 * @brief 获取文件大小的函数
 * @authors SN-Grotesque
 * @note 无
 * @param size 指针，指向长度的地址
 * @param fn 路径
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fileSize WMKC_OPEN_API
WMKC_OF((wmkcSize *size, wmkcCSTR fn));

/**
 * @brief 文件打开函数
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcFile对象指针的地址
 * @param fn 路径
 * @param mode 打开模式
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_open WMKC_OPEN_API
WMKC_OF((wmkcFile_obj **obj, wmkcCSTR fn, wmkcCSTR mode));

/**
 * @brief 文件关闭函数
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcFile对象指针的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_close WMKC_OPEN_API
WMKC_OF((wmkcFile_obj **obj));

/**
 * @brief 文件完整读取函数
 * @authors SN-Grotesque
 * @note 无
 * @param buf 指针，指向缓冲区指针的地址
 * @param size 指针，指向长度的地址
 * @param fn 路径
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fread WMKC_OPEN_API
WMKC_OF((wmkcByte **buf, wmkcSize *size, wmkcCSTR fn));

/**
 * @brief 文件完整写入函数
 * @authors SN-Grotesque
 * @note 无
 * @param buf 指针，指向缓冲区的地址
 * @param size 缓冲区的长度
 * @param fn 路径
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fwrite WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size, wmkcCSTR fn));

#endif /* WMKC_FILE */
#endif /* WMKC_SUPPORT */
