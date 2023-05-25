/**
 * 后续重新设计一下这个库，让它变得更加易用一些。
 * 针对于Windows系统的一些文本编码兼容问题，想想办法解决。
*/
#ifndef __SN_FILE__
#define __SN_FILE__

#include <snConf.h>

#if defined(_WIN32)
#include <Windows.h>
#include <shlwapi.h>
#define snFile_text(x) L ## x
#define snFile_fopen(fn, mode) _wfopen(fn, mode)
typedef LPCWSTR _snFileString;
#else
#include <sys/stat.h>
#include <unistd.h>
#define snFile_text(x) x
#define snFile_fopen(fn, mode) fopen(fn, mode)
typedef wmkcString _snFileString;
#endif

typedef _snfilestring snFileString;

#define SN_FILE_BLOCKLEN 4096

typedef struct {
    wmkcByte *data;    // 文件数据
    wmkcSize size;     // 文件长度
    wmkcSize quotient; // 文件长度除块大小的商
    wmkcSize leftover; // 文件剩余长度
} snFile_ctx;

// 初始化snFile对象
WMKC_PUBLIC(snErr_ctx) snFile_new WMKC_OPEN_API
WMKC_OF((snFile_ctx **obj));

// 释放snFile对象
WMKC_PUBLIC(snErr_ctx) snFile_free WMKC_OPEN_API
WMKC_OF((snFile_ctx **obj));

// 检查路径是否存在
WMKC_PUBLIC(wmkcBool) snFile_exists WMKC_OPEN_API
WMKC_OF((snFileString fn));

// 获取文件大小
WMKC_PUBLIC(snErr_ctx) snFile_fileSize WMKC_OPEN_API
WMKC_OF((wmkcSize *size, snFileString fn));

// 整个读取文件
WMKC_PUBLIC(snErr_ctx) snFile_fread WMKC_OPEN_API
WMKC_OF((snFile_ctx *obj, snFileString fn));

// 整个写入文件
WMKC_PUBLIC(snErr_ctx) snFile_fwrite WMKC_OPEN_API
WMKC_OF((snFile_ctx *obj, snFileString fn));
#endif