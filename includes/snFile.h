#ifndef __SN_FILE_HANDLER__
#define __SN_FILE_HANDLER__

#include <snConf.h>

#if defined(__linux)
#   include <sys/stat.h>
#   include <unistd.h>
    typedef snString snFileStr;
#   define snFile_Char(x) x
#   define snFile_open(fn, mode) fopen(fn, mode)
#elif defined(_WIN32)
#   include <windows.h>
    typedef LPCWSTR snFileStr;
#   define snFile_Char(x) L ## x
#   define snFile_open(fn, mode) _wfopen(fn, mode)
#endif

#define SN_FILE_BLOCKLEN 4096

typedef struct {
    snByte *data; // 文件数据
    snFile *fp;   // 文件句柄，暂时禁止直接使用
    snSize size;  // 文件长度
    snSize quotient; // 文件长度除块大小的商
    snSize leftover; // 文件剩余长度
} snFile_ctx;

// 初始化snFile对象
SN_PUBLIC(snError) snFile_new SN_OPEN_API
SN_FUNC_OF((snFile_ctx **obj));

// 释放snFile对象
SN_PUBLIC(snError) snFile_free SN_OPEN_API
SN_FUNC_OF((snFile_ctx **obj));

// 检查路径是否存在
SN_PUBLIC(snBool) snFile_exists SN_OPEN_API
SN_FUNC_OF((snFileStr fn));

// 获取文件大小
SN_PUBLIC(snError) snFile_fileSize SN_OPEN_API
SN_FUNC_OF((snSize *size, snFileStr fn));

// 整个读取文件
SN_PUBLIC(snError) snFile_fread SN_OPEN_API
SN_FUNC_OF((snFile_ctx *obj, snFileStr fn));

// 整个写入文件
SN_PUBLIC(snError) snFile_fwrite SN_OPEN_API
SN_FUNC_OF((snFile_ctx *obj, snFileStr fn));
#endif