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

/**
 * @brief 为wmkcFile对象申请内存空间
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcFile对象申请内存空间。
 * 
 * @note 无
 * @param obj 这是一个指针，指向wmkcFile对象指针的地址。
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_new WMKC_OPEN_API
WMKC_OF((wmkcFile_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcFile_new: obj is NULL.");
    }
    if(!wmkcMemoryNew(wmkcFile_obj *, (*obj), sizeof(wmkcFile_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcFile_new: (*obj) Failed to apply for memory.");
    }
    (*obj)->data = wmkcNull;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 为wmkcFile对象释放内存空间
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcFile对象释放内存空间。
 * 
 * @note 无
 * @param obj 这是一个指针，指向wmkcFile对象指针的地址。
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_free WMKC_OPEN_API
WMKC_OF((wmkcFile_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcFile_free: obj is NULL.");
    }
    if((*obj)->data) {
        wmkcMemoryFree((*obj)->data);
    }
    wmkcMemoryFree((*obj));
    wmkcErr_return(error, wmkcErr_OK, "OK.");
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
WMKC_OF((wmkcFileString fn))
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
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fileSize WMKC_OPEN_API
WMKC_OF((wmkcSize *size, wmkcFileString fn))
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
 * 
 * 此函数获取文件的大小后分组将文件的完整内容读取到内存中。
 * 
 * @note 此函数不应用于单次读取超大文件，以免内存空间告急。
 * @param obj 这是一个指针，指向wmkcFile对象的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fread WMKC_OPEN_API
WMKC_OF((wmkcFile_obj *obj, wmkcFileString fn))
{
    wmkcErr_obj error;
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

/**
 * @brief 将缓冲区的数据完整写入一个文件
 * @authors SN-Grotesque
 * 
 * 此函数将缓冲区的数据分组，并且完整写入至一个文件中。
 * 
 * @note 无
 * @param obj 这是一个指针，指向wmkcFile对象的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr_obj对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcFile_fwrite WMKC_OPEN_API
WMKC_OF((wmkcFile_obj *obj, wmkcFileString fn))
{
    wmkcErr_obj error;
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
