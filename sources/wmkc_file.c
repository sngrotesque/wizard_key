#include <wmkc_file.h>

/**
 * @brief 检查是否存在指定的路径
 * @authors SN-Grotesque
 * 
 * 此函数通过调用系统函数进行检查路径是否存在。
 * 
 * @note 错误检查由wmkcFile_exists执行。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcBool值，true表示路径存在，false表示不存在。
*/
WMKC_PRIVATE(wmkcBool) _wmkcFile_exists
WMKC_OF((wmkcCSTR fn))
{
#   ifdef WMKC_PLATFORM_LINUX
    if(access(fn, F_OK) == 0) return true;
#   elif defined(WMKC_PLATFORM_WINOS)
    // 后续修改一下吧，看看到底留不留这个函数。
    // 因为每次就因为这一个函数就得额外链接一个库好烦啊。
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
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
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
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, wmkcNull); // 开启文件句柄
    GetFileSizeEx(hFile, &W_size); // 将文件大小写入大整数类型结构体
    CloseHandle(hFile); // 关闭文件句柄
    return (wmkcSize)W_size.QuadPart;
#   endif
}

/**
 * @brief 创建一个文件指针
 * @authors SN-Grotesque
 * @note 此函数在Windows系统中使用_wfopen函数创建文件指针
 * @param fn 这是一个字符串，必须是UTF-8编码，为文件路径
 * @param mode 这是一个字符串，必须是UTF-8编码，为文件打开模式
 * @return 返回一个文件指针
 */
WMKC_PUBLIC(FILE *) wmkcFile_fopen WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn, wmkcCSTR mode))
{
#   if defined(WMKC_PLATFORM_LINUX)
    return fopen(fn, mode);
#   elif defined(WMKC_PLATFORM_WINOS)
    wmkcChar *win_fn = wmkcNull;
    wmkcChar *win_mode = wmkcNull;
    wmkcCoder_convert(&win_fn, (wmkcChar *)fn, "UNICODELITTLE<UTF-8");
    wmkcCoder_convert(&win_mode, (wmkcChar *)mode, "UNICODELITTLE<UTF-8");
    return _wfopen((LPCWSTR)win_fn, (LPCWSTR)win_mode);
#   endif
}

/**
 * @brief 检查是否存在指定的路径
 * @authors SN-Grotesque
 * 
 * 此函数通过调用系统函数进行检查路径是否存在。
 * 
 * @note 无
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcBool值，true表示路径存在，false表示不存在。
*/
WMKC_PUBLIC(wmkcBool) wmkcFile_exists WMKC_OPEN_API
WMKC_OF((wmkcCSTR fn))
{
    if(!fn) {
        return false;
    }
    return _wmkcFile_exists(fn);
}

/**
 * @brief 获取文件的大小
 * @authors SN-Grotesque
 * 
 * 此函数通过调用系统函数获取文件的大小。
 * 
 * @note 无
 * @param size 这是一个指针，指向wmkcSize类型变量的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
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

/**
 * @brief 完整读取一个文件的内容
 * @authors SN-Grotesque
 * @note 此函数不应用于单次读取超大文件，以免内存空间告急。
 * @param buf 这是一个指针，指向缓冲区指针的地址
 * @param size 这是一个指针，指向长度的地址
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fread WMKC_OPEN_API
WMKC_OF((wmkcByte **buf, wmkcSize *size, wmkcCSTR fn))
{
    wmkcErr_obj error;
    if(!buf || !size || !fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcFile_fread: buf or size or fn is NULL.");
    }
    wmkcByte *fileData_ptr = wmkcNull;
    FILE *fp = wmkcNull;
    wmkcSize quotient, leftover;
    wmkcSize x;

    if(!(fp = wmkcFile_fopen(fn, "rb"))) {
        wmkcErr_return(error, wmkcErr_FileOpen, "wmkcFile_fread: File opening failed.");
    }

    error = wmkcFile_fileSize(size, fn);
    if(error.code) return error;

    if(!wmkcMemoryNew(wmkcByte *, (*buf), *size + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcFile_fread: (*buf) failed to apply for memory.");
    }

    fileData_ptr = (*buf);
    fileData_ptr[*size] = 0x00;

    quotient = *size / WMKC_FILE_BLOCKLEN;
    leftover = *size % WMKC_FILE_BLOCKLEN;

    for(x = 0; x < quotient; ++x) {
        fread(fileData_ptr, 1, WMKC_FILE_BLOCKLEN, fp);
        fileData_ptr += WMKC_FILE_BLOCKLEN;
    }
    if(leftover) {
        fread(fileData_ptr, 1, leftover, fp);
    }

    if(fclose(fp)) {
        wmkcErr_return(error, wmkcErr_FileClose, "wmkcFile_fread: File closing failed.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 将缓冲区的数据完整写入一个文件
 * @authors SN-Grotesque
 * @note 无
 * @param buf 这是一个指针，指向缓冲区的地址
 * @param size 这是一个长度，代表缓冲区的长度
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fwrite WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size, wmkcCSTR fn))
{
    wmkcErr_obj error;
    if(!buf || !size || !fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcFile_fwrite: buf or size or fn is NULL.");
    }
    wmkcByte *fileData_ptr = wmkcNull;
    FILE *fp = wmkcNull;
    wmkcSize quotient, leftover;
    wmkcSize x;

    if(!(fp = wmkcFile_fopen(fn, "wb"))) {
        wmkcErr_return(error, wmkcErr_FileOpen, "wmkcFile_fwrite: File opening failed.");
    }

    fileData_ptr = buf;
    quotient = size / WMKC_FILE_BLOCKLEN;
    leftover = size % WMKC_FILE_BLOCKLEN;

    for(x = 0; x < quotient; ++x) {
        fwrite(fileData_ptr, 1, WMKC_FILE_BLOCKLEN, fp);
        fileData_ptr += WMKC_FILE_BLOCKLEN;
    }
    if(leftover) {
        fwrite(fileData_ptr, 1, leftover, fp);
    }

    if(fclose(fp)) {
        wmkcErr_return(error, wmkcErr_FileClose, "wmkcFile_fwrite: File closing failed.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
