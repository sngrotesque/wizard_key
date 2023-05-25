#include <wmkcFile.h>

/**
 * 这个函数仅供snFile_exists调用。
*/
WMKC_PRIVATE(wmkcBool) _snFile_exists
WMKC_OF((snFileString fn))
{
#   ifdef __linux
    if(access(fn, F_OK) == 0) {
        return true;
    }
#   elif defined(_WIN32)
    if(PathFileExistsW(fn)) {
        return true;
    }
#   endif
    return false;
}

/**
 * 这个函数仅供snFile_fileSize调用。
*/
WMKC_PRIVATE(wmkcSize) _snFile_fileSize
WMKC_OF((snFileString fn))
{
#   if defined(__linux)
    struct stat info;
    stat(fn, &info);
    return (wmkcSize)info.st_size;
#   elif defined(_WIN32)
    LARGE_INTEGER W_size;
    HANDLE        hFile;

    hFile = CreateFileW(fn, GENERIC_READ, FILE_SHARE_READ, wmkcNull,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, wmkcNull);
    GetFileSizeEx(hFile, &W_size);
    CloseHandle(hFile);
    return (wmkcSize)W_size.QuadPart;
#   endif
}

WMKC_PUBLIC(snErr_ctx) snFile_new WMKC_OPEN_API
WMKC_OF((snFile_ctx **obj))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNULL, "snFile_new: obj is NULL.");
    }
    if(!wmkcMemoryNew(snFile_ctx *, (*obj), sizeof(snFile_ctx))) {
        snErr_return(error, snErr_ErrMemory,
            "snFile_new: (*obj) Failed to apply for memory.");
    }
    (*obj)->data = wmkcNull;
    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snFile_free WMKC_OPEN_API
WMKC_OF((snFile_ctx **obj))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNULL, "snFile_free: obj is NULL.");
    }
    if((*obj)->data) {
        wmkcMemoryFree((*obj)->data);
    }
    wmkcMemoryFree((*obj));
    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcBool) snFile_exists WMKC_OPEN_API
WMKC_OF((snFileString fn))
{
    if(!fn) {
        return false;
    }
    return _snFile_exists(fn);
}

WMKC_PUBLIC(snErr_ctx) snFile_fileSize WMKC_OPEN_API
WMKC_OF((wmkcSize *size, snFileString fn))
{
    snErr_ctx error;
    if(!snFile_exists(fn)) {
        snErr_return(error, snErr_FileFolderPath,
            "snFile_fileSize: No files or directories.");
    }
    if(!size) {
        snErr_return(error, snErr_ErrNULL, "snFile_fileSize: size is NULL.");
    }
    if(!(*size = _snFile_fileSize(fn))) {
        snErr_return(error, snErr_FileNull, "snFile_fileSize: The file is empty.");
    }
    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snFile_fread WMKC_OPEN_API
WMKC_OF((snFile_ctx *obj, snFileString fn))
{
    snErr_ctx error;
    wmkcByte *fileData_ptr = wmkcNull;
    wmkcFile *fp = wmkcNull;
    wmkcSize x;

    error = snFile_fileSize(&obj->size, fn);
    if(error.code) {
        return error;
    }
    if(!obj->data) {
        if(!wmkcMemoryNew(wmkcByte *, obj->data, obj->size + 1)) {
            snErr_return(error, snErr_ErrMemory,
                "snFile_fread: obj->data Failed to apply for memory.");
        }
    }

    fileData_ptr = obj->data;
    obj->data[obj->size] = 0x00;
    obj->quotient = obj->size / SN_FILE_BLOCKLEN;
    obj->leftover = obj->size % SN_FILE_BLOCKLEN;

    if(!(fp = snFile_fopen(fn, snFile_text("rb")))) {
        snErr_return(error, snErr_FileOpen, "snFile_fread: File opening failed.");
    }

    for(x = 0; x < obj->quotient; ++x) {
        fread(fileData_ptr, 1, SN_FILE_BLOCKLEN, fp);
        fileData_ptr += SN_FILE_BLOCKLEN;
    }
    if(obj->leftover) {
        fread(fileData_ptr, 1, obj->leftover, fp);
    }

    if(fclose(fp)) {
        snErr_return(error, snErr_FileClose, "snFile_fread: File closing failed.");
    }
    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snFile_fwrite WMKC_OPEN_API
WMKC_OF((snFile_ctx *obj, snFileString fn))
{
    snErr_ctx error;
    if(!obj || !obj->data || !obj->size || !fn) {
        snErr_return(error, snErr_ErrNULL,
            "snFile_fwrite: obj or obj->data or obj->size or fn is NULL.");
    }
    wmkcByte *fileData_ptr = wmkcNull;
    wmkcFile *fp = wmkcNull;
    wmkcSize x;

    fileData_ptr = obj->data;
    obj->quotient = obj->size / SN_FILE_BLOCKLEN;
    obj->leftover = obj->size % SN_FILE_BLOCKLEN;

    if(!(fp = snFile_fopen(fn, snFile_text("wb")))) {
        snErr_return(error, snErr_FileOpen, "snFile_fwrite: File opening failed.");
    }

    for(x = 0; x < obj->quotient; ++x) {
        fwrite(fileData_ptr, 1, SN_FILE_BLOCKLEN, fp);
        fileData_ptr += SN_FILE_BLOCKLEN;
    }
    if(obj->leftover) {
        fwrite(fileData_ptr, 1, obj->leftover, fp);
    }

    if(fclose(fp)) {
        snErr_return(error, snErr_FileClose, "snFile_fwrite: File closing failed.");
    }
    snErr_return(error, snErr_OK, "OK.");
}
