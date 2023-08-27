#include <wmkc_file.h>

WMKC_PRIVATE(wmkcBool) _wmkcFile_exists
WMKC_OF((wmkcCSTR fn))
{
#   if defined(WMKC_PLATFORM_LINUX)
    if(access(fn, F_OK) == 0) return true;
#   elif defined(WMKC_PLATFORM_WINOS)
    if(PathFileExistsW((LPWSTR)fn)) return true;
#   endif
    return false;
}

WMKC_PRIVATE(wmkcSize) _wmkcFile_fileSize
WMKC_OF((wmkcCSTR fn))
{
#   if defined(WMKC_PLATFORM_LINUX)
    struct stat info;
    stat(fn, &info);
    return (wmkcSize)info.st_size;
#   elif defined(WMKC_PLATFORM_WINOS)
    LARGE_INTEGER W_size; // Windows大整数类型
    HANDLE        hFile;  // Windows文件句柄
    hFile = CreateFileW((LPWSTR)fn, GENERIC_READ, FILE_SHARE_READ, wmkcNull,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, wmkcNull);
    GetFileSizeEx(hFile, &W_size); // 将文件大小写入大整数类型结构体
    CloseHandle(hFile); // 关闭文件句柄
    return (wmkcSize)W_size.QuadPart;
#   endif
}

WMKC_PUBLIC(FILE *) _wmkcFile_fopen WMKC_OPEN_API
WMKC_OF((wmkcChar *fn, wmkcChar *mode))
{
#   if defined(WMKC_PLATFORM_LINUX)
    return fopen(fn, mode);
#   elif defined(WMKC_PLATFORM_WINOS)
    return _wfopen((LPCWSTR)fn, (LPCWSTR)mode);
#   endif
}

WMKC_PUBLIC(wmkcBool) wmkcFile_exists WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn))
{
    if(!fn) {
        return false;
    }
    return _wmkcFile_exists(fn);
}

WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fileSize WMKC_OPEN_API
WMKC_OF((wmkcSize *size, wmkcCSTR fn))
{
    wmkcErr_obj error;
    if(!wmkcFile_exists(fn)) {
        wmkcErr_func_return(error, wmkcErr_FileFolderPath, "wmkcFile_fileSize",
            "No files or directories.");
    }
    if(!size) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcFile_fileSize", "size is NULL.");
    }
    if(!(*size = _wmkcFile_fileSize(fn))) {
        wmkcErr_func_return(error, wmkcErr_FileNull, "wmkcFile_fileSize", "The file is empty.");
    }
    wmkcErr_func_return(error, wmkcErr_OK, "wmkcFile_fileSize", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcFile_open WMKC_OPEN_API
WMKC_OF((wmkcFile_obj **obj, wmkcCSTR _filePath, wmkcCSTR _openMode))
{
    wmkcErr_obj error;
    if(!obj || !_filePath || !_openMode) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcFile_open",
            "obj or _filePath or _openMode is NULL.");
    }
    if(!wmkcMem_new(wmkcFile_obj *, (*obj), sizeof(wmkcFile_obj))) {
        wmkcErr_func_return(error, wmkcErr_ErrMemory, "wmkcFile_open",
            "Failed to allocate memory for (*obj).");
    }
    wmkcChar *filePath = wmkcNull; // 实际使用的文件路径
    wmkcChar *openMode = wmkcNull; // 实际使用的打开模式

#   if defined(WMKC_PLATFORM_LINUX)
    filePath   = (wmkcChar *)_filePath;
    openMode = (wmkcChar *)_openMode;
#   elif defined(WMKC_PLATFORM_WINOS)
    // 如果是Windows系统就将路径字符串的编码转为UTF-16-LE
    wmkc_u32 filePathLength = MultiByteToWideChar(CP_UTF8, 0, _filePath, -1, wmkcNull, 0);
    wmkc_u32 openModeLength = MultiByteToWideChar(CP_UTF8, 0, _openMode, -1, wmkcNull, 0);

    if(!wmkcMem_new(wmkcChar *, filePath, filePathLength << 1)) {
        wmkcMem_free((*obj));
        wmkcErr_func_return(error, wmkcErr_ErrMemory, "wmkcFile_open",
            "Failed to allocate memory for filePath.");
    }
    if(!wmkcMem_new(wmkcChar *, openMode, openModeLength << 1)) {
        wmkcMem_free((*obj));
        wmkcMem_free(filePath);
        wmkcErr_func_return(error, wmkcErr_ErrMemory, "wmkcFile_open",
            "Failed to allocate memory for openMode.");
    }
    MultiByteToWideChar(CP_UTF8, 0, _filePath, -1, (LPWSTR)filePath, filePathLength);
    MultiByteToWideChar(CP_UTF8, 0, _openMode, -1, (LPWSTR)openMode, openModeLength);
#   endif

    // 打开文件
    if(!((*obj)->fp = _wmkcFile_fopen(filePath, openMode))) {
        (*obj)->status = WMKC_FILE_MODE_FAIL; // 打开失败
#       if defined(WMKC_PLATFORM_WINOS)
        wmkcMem_free(filePath);
        wmkcMem_free(openMode);
#       endif
        wmkcMem_free((*obj));
        wmkcErr_func_return(error, wmkcErr_FileOpen, "wmkcFile_open", "File opening failed.");
    }
    // 获取打开模式
    if(strchr(_openMode, 'r')) {
        (*obj)->status = WMKC_FILE_MODE_READ;
        // 获取文件大小
        error = wmkcFile_fileSize(&(*obj)->fsize, filePath);
        if(error.code) {
#           if defined(WMKC_PLATFORM_WINOS)
            wmkcMem_free(filePath);
            wmkcMem_free(openMode);
#           endif
            return error;
        }
    } else if(strchr(_openMode, 'w') || strchr(_openMode, 'a')) {
        (*obj)->status = WMKC_FILE_MODE_WRITE;
    }
    (*obj)->fn = _filePath; // 指向文件路径

#   ifdef WMKC_PLATFORM_WINOS
    wmkcMem_free(filePath);
    wmkcMem_free(openMode);
#   endif

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcFile_open", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcFile_close WMKC_OPEN_API
WMKC_OF((wmkcFile_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcFile_close", "obj is NULL.");
    }
    if((*obj)->fp) {
        if(fclose((*obj)->fp)) {
            wmkcErr_func_return(error, wmkcErr_FileClose, "wmkcFile_close",
                "File closing failed.");
        }
    }
    wmkcMem_free((*obj));

    wmkcErr_func_return(error, wmkcErr_OK, "wmkcFile_close", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fread WMKC_OPEN_API
WMKC_OF((wmkcByte **buf, wmkcSize *size, wmkcCSTR fn))
{
    wmkcErr_obj error;
    if(!buf || !size || !fn) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcFile_fread",
            "obj or buf or size or fn is NULL.");
    }
    wmkcFile_obj *file = wmkcNull;

    if((error = wmkcFile_open(&file, fn, "rb")).code)
        return error;
    wmkcSize quotient = file->fsize / WMKC_FILE_BLOCKLEN;
    wmkcSize leftover = file->fsize % WMKC_FILE_BLOCKLEN;

    if(!wmkcMem_new(wmkcByte *, (*buf), file->fsize + 1)) {
        wmkcFile_close(&file);
        wmkcErr_func_return(error, wmkcErr_ErrMemory, "wmkcFile_fread",
            "Failed to allocate memory for (*buf).");
    }
    wmkcByte *p = (*buf);
    p[file->fsize] = 0x00;

    for(wmkcSize x = 0; x < quotient; ++x) {
        WMKC_FILE_FREAD(p, WMKC_FILE_BLOCKLEN, file->fp);
        p += WMKC_FILE_BLOCKLEN;
    }
    if(leftover) {
        WMKC_FILE_FREAD(p, leftover, file->fp);
    }
    *size = file->fsize;

    wmkcFile_close(&file);
    wmkcErr_func_return(error, wmkcErr_OK, "wmkcFile_fread", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fwrite WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size, wmkcCSTR fn))
{
    wmkcErr_obj error;
    if(!buf || !size || !fn) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcFile_fwrite",
            "buf or size or fn is NULL.");
    }
    wmkcFile_obj *file = wmkcNull;
    if((error = wmkcFile_open(&file, fn, "wb")).code)
        return error;

    wmkcSize quotient = size / WMKC_FILE_BLOCKLEN;
    wmkcSize leftover = size % WMKC_FILE_BLOCKLEN;
    wmkcByte *p = buf;

    for(wmkcSize x = 0; x < quotient; ++x) {
        fwrite(p, 1, WMKC_FILE_BLOCKLEN, file->fp);
        p += WMKC_FILE_BLOCKLEN;
    }
    if(leftover)
        fwrite(p, 1, leftover, file->fp);

    wmkcFile_close(&file);
    wmkcErr_func_return(error, wmkcErr_OK, "wmkcFile_fwrite", "OK.");
}
