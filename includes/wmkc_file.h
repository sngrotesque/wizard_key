/**
 * 后续重新设计一下这个库，让它变得更加易用一些。
 * 针对于Windows系统的一些文本编码兼容问题，想想办法解决。
*/
#ifndef WMKC_FILE
#define WMKC_FILE
#include <wmkc_conf.h>

#define WMKC_FILE_BLOCKLEN 4096

#if defined(WMKC_WINDOWS_SUPPORT)
#include <Windows.h>
#include <shlwapi.h> // -lshlwapi
#define wmkcFile_text(x) L ## x
#define wmkcFile_fopen(fn, mode) _wfopen(fn, mode)
typedef LPCWSTR _wmkcFileString;
#else
#include <sys/stat.h>
#include <unistd.h>
#define wmkcFile_text(x) x
#define wmkcFile_fopen(fn, mode) fopen(fn, mode)
typedef wmkcString _wmkcFileString;
#endif

typedef _wmkcFileString wmkcFileString;
typedef struct {
    wmkcByte *data;    // 文件数据
    wmkcSize size;     // 文件长度
    wmkcSize quotient; // 文件长度除块大小的商
    wmkcSize leftover; // 文件剩余长度
} wmkcFile_ctx;

// 初始化wmkcFile对象
WMKC_PUBLIC(wmkcErr_ctx) wmkcFile_new WMKC_OPEN_API
WMKC_OF((wmkcFile_ctx **obj));

// 释放wmkcFile对象
WMKC_PUBLIC(wmkcErr_ctx) wmkcFile_free WMKC_OPEN_API
WMKC_OF((wmkcFile_ctx **obj));

// 检查路径是否存在
WMKC_PUBLIC(wmkcBool) wmkcFile_exists WMKC_OPEN_API
WMKC_OF((wmkcFileString fn));

// 获取文件大小
WMKC_PUBLIC(wmkcErr_ctx) wmkcFile_fileSize WMKC_OPEN_API
WMKC_OF((wmkcSize *size, wmkcFileString fn));

// 整个读取文件
WMKC_PUBLIC(wmkcErr_ctx) wmkcFile_fread WMKC_OPEN_API
WMKC_OF((wmkcFile_ctx *obj, wmkcFileString fn));

// 整个写入文件
WMKC_PUBLIC(wmkcErr_ctx) wmkcFile_fwrite WMKC_OPEN_API
WMKC_OF((wmkcFile_ctx *obj, wmkcFileString fn));
#endif