#include <snFile.h>

SN_PUBLIC(snErr_ctx) snFile_new SN_OPEN_API
SN_FUNC_OF((snFile_ctx **obj))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNULL, "snFile_new: obj is NULL.");
    }
    if(!snMemoryNew(snFile_ctx *, (*obj), sizeof(snFile_ctx))) {
        snErr_return(error, snErr_ErrMemory,
            "snFile_new: (*obj) Failed to apply for memory.");
    }
    (*obj)->data = snNull;

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snFile_free SN_OPEN_API
SN_FUNC_OF((snFile_ctx **obj))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNULL, "snFile_free: obj is NULL.");
    }
    if((*obj)->data) {
        snMemoryFree((*obj)->data);
    }
    snMemoryFree((*obj));
    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snBool) snFile_exists SN_OPEN_API
SN_FUNC_OF((snFileStr fn))
{
    if(!fn) {
        return false;
    }

#   ifdef __linux
    if(access(fn, F_OK) == 0) {
        return true;
    }
#   elif defined(_WIN32)
    DWORD dwAttrib = GetFileAttributesW(fn);
    if(dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
        return true;
    }
#   endif

    return false;
}

SN_PUBLIC(snErr_ctx) snFile_fileSize SN_OPEN_API
SN_FUNC_OF((snSize *size, snFileStr fn))
{
    snErr_ctx error;
    if(!snFile_exists(fn)) {
        snErr_return(error, snErr_FileFolderPath,
            "snFile_fileSize: No files or directories.");
    }
    if(!size) {
        snErr_return(error, snErr_ErrNULL, "snFile_fileSize: size is NULL.");
    }

#   if defined(__linux)
    struct stat info;
    stat(fn, &info);
    *size = info.st_size;
#   elif defined(_WIN32)
    HANDLE hFile = CreateFileW(fn, GENERIC_READ, FILE_SHARE_READ, snNull, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, snNull);
    LARGE_INTEGER W_size;
    GetFileSizeEx(hFile, &W_size);
    *size = W_size.QuadPart;
#   endif

#   if defined(_WIN32)
    CloseHandle(hFile);
#   endif
    if(!(*size)) {
        snErr_return(error, snErr_FileNull, "snFile_fileSize: The file is empty.");
    }

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snFile_fread SN_OPEN_API
SN_FUNC_OF((snFile_ctx *obj, snFileStr fn))
{
    snErr_ctx error;
    snByte *fileData_ptr = snNull;
    snFile *fp = snNull;
    snSize x;

    error = snFile_fileSize(&obj->size, fn);
    if(error.code) {
        return error;
    }
    if(!obj->data) {
        if(!snMemoryNew(snByte *, obj->data, obj->size + 1)) {
            snErr_return(error, snErr_ErrMemory,
                "snFile_fread: obj->data Failed to apply for memory.");
        }
    }

    fileData_ptr = obj->data;
    obj->data[obj->size] = 0x00;
    obj->quotient = obj->size / SN_FILE_BLOCKLEN;
    obj->leftover = obj->size % SN_FILE_BLOCKLEN;

    if(!(fp = snFile_open(fn, snFile_Char("rb")))) {
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

SN_PUBLIC(snErr_ctx) snFile_fwrite SN_OPEN_API
SN_FUNC_OF((snFile_ctx *obj, snFileStr fn))
{
    snErr_ctx error;
    if(!obj || !obj->data || !obj->size || !fn) {
        snErr_return(error, snErr_ErrNULL,
            "snFile_fwrite: obj or obj->data or obj->size or fn is NULL.");
    }
    snByte *fileData_ptr = snNull;
    snFile *fp = snNull;
    snSize x;

    fileData_ptr = obj->data;
    obj->quotient = obj->size / SN_FILE_BLOCKLEN;
    obj->leftover = obj->size % SN_FILE_BLOCKLEN;

    if(!(fp = snFile_open(fn, snFile_Char("wb")))) {
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

