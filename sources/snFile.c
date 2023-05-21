#include <snFile.h>

SN_PUBLIC(snError) snFile_new SN_OPEN_API
SN_FUNC_OF((snFile_ctx **obj))
{
    if(!obj)
        return snErr_ErrNullData;
    if(!snMemoryNew(snFile_ctx *, (*obj), sizeof(snFile_ctx)))
        return snErr_ErrMemory;
    (*obj)->data = snNull;
    (*obj)->fp = snNull;

    return snErr_OK;
}

SN_PUBLIC(snError) snFile_free SN_OPEN_API
SN_FUNC_OF((snFile_ctx **obj))
{
    if(!obj)
        return snErr_ErrNullData;
    if((*obj)->data) {
        snMemoryFree((*obj)->data);
    }
    if((*obj)->fp) {
        fclose((*obj)->fp);
        (*obj)->fp = snNull;
    }
    snMemoryFree((*obj));
    return snErr_OK;
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

SN_PUBLIC(snError) snFile_fileSize SN_OPEN_API
SN_FUNC_OF((snSize *size, snFileStr fn))
{
    if(!snFile_exists(fn)) {
        return snErr_FileFolderPath;
    }
    if(!size) {
        return snErr_ErrInvalid;
    }

#   if defined(__linux)
    static struct stat info;
    stat(fn, &info);
    *size = info.st_size;
#   elif defined(_WIN32)
    HANDLE hFile = CreateFileW(fn, GENERIC_READ, FILE_SHARE_READ, snNull,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, snNull);
    LARGE_INTEGER W_size;
    GetFileSizeEx(hFile, &W_size);
    *size = W_size.QuadPart;
#   endif

#   if defined(_WIN32)
    CloseHandle(hFile);
#   endif
    if(!(*size))
        return snErr_FileNull;

    return snErr_OK;
}

SN_PUBLIC(snError) snFile_fread SN_OPEN_API
SN_FUNC_OF((snFile_ctx *obj, snFileStr fn))
{
    static snByte *fileData_ptr = snNull;
    static snSize x;
    if(snFile_fileSize(&obj->size, fn) == snErr_FileFolderPath) {
        return snErr_FileFolderPath;
    }
    if(!obj->data) {
        if(!snMemoryNew(snByte *, obj->data, obj->size + 1))
            return snErr_ErrMemory;
    }

    fileData_ptr = obj->data;
    obj->data[obj->size] = 0x00;
    obj->quotient = obj->size / SN_FILE_BLOCKLEN;
    obj->leftover = obj->size % SN_FILE_BLOCKLEN;

    obj->fp = snFile_open(fn, snFile_Char("rb"));

    for(x = 0; x < obj->quotient; ++x) {
        fread(fileData_ptr, 1, SN_FILE_BLOCKLEN, obj->fp);
        fileData_ptr += SN_FILE_BLOCKLEN;
    }
    if(obj->leftover) {
        fread(fileData_ptr, 1, obj->leftover, obj->fp);
    }

    fclose(obj->fp);
    return snErr_OK;
}

SN_PUBLIC(snError) snFile_fwrite SN_OPEN_API
SN_FUNC_OF((snFile_ctx *obj, snFileStr fn))
{
    if(!obj->data || !obj->size) {
        return snErr_FileNull;
    }
    static snByte *fileData_ptr = snNull;
    static snSize x;

    fileData_ptr = obj->data;
    obj->quotient = obj->size / SN_FILE_BLOCKLEN;
    obj->leftover = obj->size % SN_FILE_BLOCKLEN;

    obj->fp = snFile_open(fn, snFile_Char("wb"));

    for(x = 0; x < obj->quotient; ++x) {
        fwrite(fileData_ptr, 1, SN_FILE_BLOCKLEN, obj->fp);
        fileData_ptr += SN_FILE_BLOCKLEN;
    }
    if(obj->leftover) {
        fwrite(fileData_ptr, 1, obj->leftover, obj->fp);
    }

    fclose(obj->fp);
    return snErr_OK;
}

