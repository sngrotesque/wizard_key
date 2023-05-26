#include <wmkc_file.h>

/**
 * 这个函数仅供wmkcFile_exists调用。
*/
WMKC_PRIVATE(wmkcBool) _wmkcFile_exists
WMKC_OF((wmkcFileString fn))
{
#   ifdef WMKC_PLATFORM_LINUX
    if(access(fn, F_OK) == 0) return true;
#   elif defined(WMKC_PLATFORM_WINOS)
    if(PathFileExistsW(fn)) return true;
#   endif
    return false;
}

/**
 * 这个函数仅供wmkcFile_fileSize调用。
*/
WMKC_PRIVATE(wmkcSize) _wmkcFile_fileSize
WMKC_OF((wmkcFileString fn))
{
#   if defined(WMKC_PLATFORM_LINUX)
    struct stat info;
    stat(fn, &info);
    return (wmkcSize)info.st_size;
#   elif defined(WMKC_PLATFORM_WINOS)
    LARGE_INTEGER W_size; // Windows大整数类型
    HANDLE        hFile;  // Windows文件句柄
    hFile = CreateFileW(fn, GENERIC_READ, FILE_SHARE_READ, wmkcNull,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, wmkcNull); // 开启文件句柄
    GetFileSizeEx(hFile, &W_size); // 将文件大小写入大整数类型结构体
    CloseHandle(hFile); // 关闭文件句柄
    return (wmkcSize)W_size.QuadPart;
#   endif
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcFile_new WMKC_OPEN_API
WMKC_OF((wmkcFile_ctx **obj))
{
    wmkcErr_ctx error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcFile_new: obj is NULL.");
    }
    if(!wmkcMemoryNew(wmkcFile_ctx *, (*obj), sizeof(wmkcFile_ctx))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcFile_new: (*obj) Failed to apply for memory.");
    }
    (*obj)->data = wmkcNull;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcFile_free WMKC_OPEN_API
WMKC_OF((wmkcFile_ctx **obj))
{
    wmkcErr_ctx error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcFile_free: obj is NULL.");
    }
    if((*obj)->data) {
        wmkcMemoryFree((*obj)->data);
    }
    wmkcMemoryFree((*obj));
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcBool) wmkcFile_exists WMKC_OPEN_API
WMKC_OF((wmkcFileString fn))
{
    if(!fn) {
        return false;
    }
    return _wmkcFile_exists(fn);
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcFile_fileSize WMKC_OPEN_API
WMKC_OF((wmkcSize *size, wmkcFileString fn))
{
    wmkcErr_ctx error;
    if(!wmkcFile_exists(fn)) {
        wmkcErr_return(error, wmkcErr_FileFolderPath,
            "wmkcFile_fileSize: No files or directories.");
    }
    if(!size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcFile_fileSize: size is NULL.");
    }
    if(!(*size = _wmkcFile_fileSize(fn))) {
        wmkcErr_return(error, wmkcErr_FileNull, "wmkcFile_fileSize: The file is empty.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcFile_fread WMKC_OPEN_API
WMKC_OF((wmkcFile_ctx *obj, wmkcFileString fn))
{
    wmkcErr_ctx error;
    wmkcByte *fileData_ptr = wmkcNull;
    wmkcFile *fp = wmkcNull;
    wmkcSize x;

    error = wmkcFile_fileSize(&obj->size, fn);
    if(error.code) {
        return error;
    }
    if(!obj->data) {
        if(!wmkcMemoryNew(wmkcByte *, obj->data, obj->size + 1)) {
            wmkcErr_return(error, wmkcErr_ErrMemory,
                "wmkcFile_fread: obj->data Failed to apply for memory.");
        }
    }

    fileData_ptr = obj->data;
    obj->data[obj->size] = 0x00;
    obj->quotient = obj->size / WMKC_FILE_BLOCKLEN;
    obj->leftover = obj->size % WMKC_FILE_BLOCKLEN;

    if(!(fp = wmkcFile_fopen(fn, wmkcFile_text("rb")))) {
        wmkcErr_return(error, wmkcErr_FileOpen, "wmkcFile_fread: File opening failed.");
    }

    for(x = 0; x < obj->quotient; ++x) {
        fread(fileData_ptr, 1, WMKC_FILE_BLOCKLEN, fp);
        fileData_ptr += WMKC_FILE_BLOCKLEN;
    }
    if(obj->leftover) {
        fread(fileData_ptr, 1, obj->leftover, fp);
    }

    if(fclose(fp)) {
        wmkcErr_return(error, wmkcErr_FileClose, "wmkcFile_fread: File closing failed.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcFile_fwrite WMKC_OPEN_API
WMKC_OF((wmkcFile_ctx *obj, wmkcFileString fn))
{
    wmkcErr_ctx error;
    if(!obj || !obj->data || !obj->size || !fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcFile_fwrite: obj or obj->data or obj->size or fn is NULL.");
    }
    wmkcByte *fileData_ptr = wmkcNull;
    wmkcFile *fp = wmkcNull;
    wmkcSize x;

    fileData_ptr = obj->data;
    obj->quotient = obj->size / WMKC_FILE_BLOCKLEN;
    obj->leftover = obj->size % WMKC_FILE_BLOCKLEN;

    if(!(fp = wmkcFile_fopen(fn, wmkcFile_text("wb")))) {
        wmkcErr_return(error, wmkcErr_FileOpen, "wmkcFile_fwrite: File opening failed.");
    }

    for(x = 0; x < obj->quotient; ++x) {
        fwrite(fileData_ptr, 1, WMKC_FILE_BLOCKLEN, fp);
        fileData_ptr += WMKC_FILE_BLOCKLEN;
    }
    if(obj->leftover) {
        fwrite(fileData_ptr, 1, obj->leftover, fp);
    }

    if(fclose(fp)) {
        wmkcErr_return(error, wmkcErr_FileClose, "wmkcFile_fwrite: File closing failed.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
