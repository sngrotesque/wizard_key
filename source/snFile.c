#include <snFile.h>

SN_PUBLIC(snError) snFile_new SN_OPEN_API
SN_FUNC_OF((snFile_ctx **obj))
{
    if(!(*obj)) {
        if(!((*obj) = (snFile_ctx *)malloc(sizeof(snFile_ctx)))) {
            return snErr_Memory;
        }
    }
    (*obj)->data = snNull;
    (*obj)->fp = snNull;

    return snErr_OK;
}

SN_PUBLIC(snError) snFile_release SN_OPEN_API
SN_FUNC_OF((snFile_ctx **obj, sn_u32 instruction))
{
    if(instruction == (SN_RELEASE_NORMAL | SN_RELEASE_FILE)) {
        free((*obj)->data);
        fclose((*obj)->fp);
    } else if(instruction == SN_RELEASE_NORMAL) {
        free((*obj)->data);
    } else if(instruction == SN_RELEASE_FILE) {
        fclose((*obj)->fp);
    }
    (*obj)->data = snNull;
    (*obj)->fp = snNull;
    free((*obj));
    (*obj) = snNull;
    return snErr_OK;
}

SN_PUBLIC(snBool) snFile_exists SN_OPEN_API
SN_FUNC_OF((const snChar *fn))
{
    if(!fn) {
        return false;
    }
    sn_32 result = access(fn, F_OK);
    if(result == 0) {
        return true;
    } else {
        return false;
    }
}

SN_PUBLIC(snError) snFile_fileSize SN_OPEN_API
SN_FUNC_OF((snSize *size, const snChar *fn))
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
    HANDLE hFile = CreateFile(fn, GENERIC_READ, FILE_SHARE_READ, snNull,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, snNull);
    LARGE_INTEGER W_size;
    GetFileSizeEx(hFile, &W_size);
    *size = W_size.QuadPart;
#   endif

    if(!(*size)) {
#   if defined(_WIN32)
        CloseHandle(hFile);
#   endif
        return snErr_FileNull;
    }
    CloseHandle(hFile);

    return snErr_OK;
}

SN_PUBLIC(snError) snFile_fread SN_OPEN_API
SN_FUNC_OF((snFile_ctx *obj, const snChar *fn))
{
    static snByte *fileData_ptr = snNull;
    static snSize x;
    if(snFile_fileSize(&obj->size, fn) == snErr_FileFolderPath) {
        return snErr_FileFolderPath;
    }
    if(!obj->data) {
        if(!(obj->data = (snByte *)malloc(obj->size + 1))) {
            return snErr_Memory;
        }
    }

    fileData_ptr = obj->data;
    obj->data[obj->size] = 0x00;
    obj->quotient = obj->size / SN_FILE_BLOCKLEN;
    obj->leftover = obj->size % SN_FILE_BLOCKLEN;

    obj->fp = fopen(fn, "rb");

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
SN_FUNC_OF((snFile_ctx *obj, const snChar *fn))
{
    if(!obj->data || !obj->size) {
        return snErr_FileNull;
    }
    static snByte *fileData_ptr = snNull;
    static snSize x;

    fileData_ptr = obj->data;
    obj->quotient = obj->size / SN_FILE_BLOCKLEN;
    obj->leftover = obj->size % SN_FILE_BLOCKLEN;

    obj->fp = fopen(fn, "wb");

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

