#include <wmkc_file.h>

/**
 * @brief 检查是否存在指定的路径
 * @authors SN-Grotesque
 * 
 * 此函数通过调用系统函数进行检查路径是否存在。
 * 
 * @note 错误检查由wmkcFile_exists执行。
 * @param fn 指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcBool值，true表示路径存在，false表示不存在。
*/
WMKC_PRIVATE(wmkcBool) _wmkcFile_exists
WMKC_OF((wmkcCSTR fn))
{
#   ifdef WMKC_PLATFORM_LINUX
    if(access(fn, F_OK) == 0) return true;
#   elif defined(WMKC_PLATFORM_WINOS)
    if(PathFileExistsW((LPWSTR)fn)) return true;
#   endif
    return false;
}

/**
 * @brief 获取文件的大小
 * @authors SN-Grotesque
 * 
 * 此函数通过调用系统函数获取文件的大小。
 * 
 * @note 错误检查由wmkcFile_fileSize执行。
 * @param fn 指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcSize变量，值为文件大小。
*/
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

/**
 * @brief 文件打开函数（私有）
 * @authors SN-Grotesque
 * @note 无
 * @param fn 文件名
 * @param mode 打开模式
 * @return 返回一个FILE指针
 */
WMKC_PUBLIC(FILE *) _wmkcFile_fopen WMKC_OPEN_API
WMKC_OF((wmkcChar *fn, wmkcChar *mode))
{
#   if defined(WMKC_PLATFORM_LINUX)
    return fopen(fn, mode);
#   elif defined(WMKC_PLATFORM_WINOS)
    return _wfopen((LPCWSTR)fn, (LPCWSTR)mode);
#   endif
}

/**
 * @brief 文件路径编码转换函数
 * @authors SN-Grotesque
 * @note 无
 * @param dst 指针，指向结果指针的地址
 * @param src 指针，指向源内容的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PRIVATE(wmkcErr_obj) _wmkcFile_convert WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *src))
{
    wmkcErr_obj error;
    if(!dst || !src) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "_wmkcFile_convert: dst or src is NULL.");
    }
    error = wmkcCoder_convert(dst, src, "UNICODELITTLE<UTF-8");
    if(error.code) return error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
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

WMKC_PUBLIC(wmkcErr_obj) wmkcFile_open WMKC_OPEN_API
WMKC_OF((wmkcFile_obj **obj, wmkcCSTR fn, wmkcCSTR mode))
{
    wmkcErr_obj error;
    if(!obj || !fn || !mode) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcFile_open: obj or fn or mode is NULL.");
    }

    if(!wmkcMem_new(wmkcFile_obj *, (*obj), sizeof(wmkcFile_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcFile_open: Failed to allocate memory for (*obj).");
    }

    wmkcChar *use_fn   = wmkcNull; // 实际使用的文件路径
    wmkcChar *use_mode = wmkcNull; // 实际使用的打开模式
#   if defined(WMKC_PLATFORM_LINUX)
    use_fn   = (wmkcChar *)fn;
    use_mode = (wmkcChar *)mode;
#   elif defined(WMKC_PLATFORM_WINOS)
    // 如果是Windows系统就将路径字符串的编码转为Unicode-Little
    error = _wmkcFile_convert(&use_fn,   (wmkcChar *)fn);
    if(error.code) return error;
    error = _wmkcFile_convert(&use_mode, (wmkcChar *)mode);
    if(error.code) return error;
#   endif

    // 打开文件
    if(!((*obj)->fp = _wmkcFile_fopen(use_fn, use_mode))) {
        (*obj)->status = WMKC_FILE_MODE_FAIL; // 打开失败
        wmkcErr_return(error, wmkcErr_FileOpen, "wmkcFile_open: File opening failed.");
    }
    // 获取打开模式
    if(strchr(mode, 'r')) {
        (*obj)->status = WMKC_FILE_MODE_READ;
        // 获取文件大小
        error = wmkcFile_fileSize(&(*obj)->fsize, use_fn);
        if(error.code) return error;
    } else if(strchr(mode, 'w') || strchr(mode, 'a')) {
        (*obj)->status = WMKC_FILE_MODE_WRITE;
    }
    // 指向文件路径
    (*obj)->fn = fn;

#   ifdef WMKC_PLATFORM_WINOS
    wmkcMem_free(use_fn);
    wmkcMem_free(use_mode);
#   endif

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcFile_close WMKC_OPEN_API
WMKC_OF((wmkcFile_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcFile_close: obj is NULL.");
    }
    if((*obj)->fp) {
        if(fclose((*obj)->fp)) {
            wmkcErr_return(error, wmkcErr_FileClose, "wmkcFile_close: File closing failed.");
        }
    }
    wmkcMem_free((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fread WMKC_OPEN_API
WMKC_OF((wmkcByte **buf, wmkcSize *size, wmkcCSTR fn))
{
    wmkcErr_obj error;
    if(!buf || !size || !fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcFile_fread: obj or buf or size or fn is NULL.");
    }
    wmkcFile_obj *file = wmkcNull;

    error = wmkcFile_open(&file, fn, "rb");
    if(error.code) return error;
    if(!file->fsize) {
        wmkcErr_return(error, wmkcErr_FileNull, "wmkcFile_fread: The file is empty.");
    }
    wmkcSize quotient = file->fsize / WMKC_FILE_BLOCKLEN;
    wmkcSize leftover = file->fsize % WMKC_FILE_BLOCKLEN;
    *size = file->fsize;

    if(!wmkcMem_new(wmkcByte *, (*buf), file->fsize + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcFile_fread: Failed to allocate memory for (*buf).");
    }
    wmkcByte *p = (*buf);
    p[file->fsize] = 0x00;

    for(wmkcSize x = 0; x < quotient; ++x) {
        fread(p, 1, WMKC_FILE_BLOCKLEN, file->fp);
        p += WMKC_FILE_BLOCKLEN;
    }
    if(leftover)
        fread(p, 1, leftover, file->fp);

    wmkcFile_close(&file);
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fwrite WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size, wmkcCSTR fn))
{
    wmkcErr_obj error;
    if(!buf || !size || !fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcFile_fwrite: buf or size or fn is NULL.");
    }
    wmkcFile_obj *file = wmkcNull;

    error = wmkcFile_open(&file, fn, "wb");
    if(error.code) return error;
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
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
