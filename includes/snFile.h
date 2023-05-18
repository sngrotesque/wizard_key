#ifndef __SN_FILE_HANDLER__
#define __SN_FILE_HANDLER__

#include <snConf.h>

#include <sys/stat.h>
#if defined(__linux)
#include <unistd.h>
#elif defined(_WIN32)
#   include <windows.h>
#   include <io.h>
#   ifndef F_OK
#       define F_OK 0
#   endif
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

/**
 *  释放snFile对象
 *      SN_RELEASE_NORMAL                 -> obj->data
 *      SN_RELEASE_FILE                   -> obj->fp
 *      SN_RELEASE_FILE | SN_RELEASE_FILE -> obj->data and obj->fp
*/
SN_PUBLIC(snError) snFile_free SN_OPEN_API
SN_FUNC_OF((snFile_ctx **obj));

// 检查路径是否存在
SN_PUBLIC(snBool) snFile_exists SN_OPEN_API
SN_FUNC_OF((snString fn));

// 获取文件大小
SN_PUBLIC(snError) snFile_fileSize SN_OPEN_API
SN_FUNC_OF((snSize *size, snString fn));

// 整个读取文件
SN_PUBLIC(snError) snFile_fread SN_OPEN_API
SN_FUNC_OF((snFile_ctx *obj, snString fn));

// 整个写入文件
SN_PUBLIC(snError) snFile_fwrite SN_OPEN_API
SN_FUNC_OF((snFile_ctx *obj, snString fn));
#endif