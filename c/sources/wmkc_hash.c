#include <wmkc_hash.h>

/**
 * @brief 初始化wmkcHash对象
 * @authors SN-Grotesque
 * 
 * 此函数将wmkcHash对象初始化，并为特定成员申请内存空间。
 * 
 * @note wmkcHash_new函数的私有函数，错误检查应在wmkcHash_new函数
 *       中进行。
 * @param obj 指针，指向wmkcHash对象指针的地址。
 * @param size 是一个长度，代表当前哈希算法digest数据的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PRIVATE(wmkcErr_obj) _wmkcHash_init
WMKC_OF((wmkcHash_obj **obj, wmkcSize size))
{
    wmkcErr_obj error;
    (*obj)->hexdigestSize = size << 1;
    (*obj)->digestSize = size;

    (*obj)->hexdigest = wmkcNull;
    if(!wmkcMem_new(wmkcByte *, (*obj)->digest, size)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "_wmkcHash_init",
            "Failed to allocate memory for (*obj)->digest.");
    }

    wmkcErr_return(error, wmkcErr_OK, "_wmkcHash_init", "OK.");
}

/**
 * @brief 为wmkcHash对象申请内存
 * @authors SN-Grotesque
 *
 * 此函数创建一个新的哈希对象，并使用指定的哈希算法类型对其进行初始化。
 *
 * @note 无
 * @param obj 指针，指向wmkcHash对象指针的地址。
 * @param hashType 为哈希算法类型，此值必须介于[0, 5]。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
**/
WMKC_PUBLIC(wmkcErr_obj) wmkcHash_new WMKC_OPEN_API
WMKC_OF((wmkcHash_obj **obj, wmkcHash_Type hashType))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcHash_new", "obj is NULL.");
    }
    if((wmkc_u32)hashType > 5) {
        wmkcErr_return(error, wmkcErr_ErrType, "wmkcHash_new",
            "Wrong hashType, must be 0 to 5.");
    }
    if(!wmkcMem_new(wmkcHash_obj *, (*obj), sizeof(wmkcHash_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcHash_new",
            "Failed to allocate memory for (*obj).");
    }

    const EVP_MD *wmkcHash_EVP_Type[6] = {
        EVP_md5(),    EVP_sha1(),   EVP_sha224(),
        EVP_sha256(), EVP_sha384(), EVP_sha512()};

    const wmkc_u32 wmkcHash_SIZE[6] = {
    //  MD5      SHA-1      SHA-224
    //  SHA-256  SHA-384    SHA-512
        16,      20,        28,
        32,      48,        64};

    (*obj)->md = wmkcHash_EVP_Type[hashType];
    error = _wmkcHash_init(obj, wmkcHash_SIZE[hashType]);
    if(error.code) return error;

    wmkcErr_return(error, wmkcErr_OK, "wmkcHash_new", "OK.");
}

/**
 * @brief 求出一段完整数据的哈希值
 * @authors SN-Grotesque
 *
 * 此函数得到传入的数据的哈希值后赋值到wmkcHash对象中。
 *
 * [EVP_MD_CTX_new](https://www.openssl.org/docs/man1.1.1/man3/EVP_MD_CTX_new.html)
 * @note 对于OpenSSL库提供的EVP相关功能，我查阅到OpenSSL库中的提示是
 *       返回值为1时表示无错误，为0时表示错误。
 *       为了不与本库产生冲突，暂时不对EVP相关功能进行返回值检查。
 * @param hash 指针，指向wmkcHash对象的地址。
 * @param buf 指针，指向一段数据的地址。
 * @param suze 这是一个长度，代表buf指向的内容的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
**/
WMKC_PUBLIC(wmkcErr_obj) wmkcHash WMKC_OPEN_API
WMKC_OF((wmkcHash_obj *hash, wmkcByte *buf, wmkcSize size))
{
    wmkcErr_obj error;
    if(!hash || !buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcHash", "hash or buf or size is NULL.");
    }
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();

    EVP_DigestInit_ex(md_ctx, hash->md, wmkcNull);
    EVP_DigestUpdate(md_ctx, buf, size);
    EVP_DigestFinal_ex(md_ctx, hash->digest, wmkcNull);
    EVP_MD_CTX_free(md_ctx);

    error = wmkcBinascii_b2a_hex(&hash->hexdigest, hash->digest, hash->digestSize);
    if(error.code) {
        return error;
    }

    wmkcErr_return(error, wmkcErr_OK, "wmkcHash", "OK.");
}

/**
 * @brief 获取指定文件的哈希值
 * @authors SN-Grotesque
 *
 * 此函数获取指定文件的哈希值后赋值到wmkcHash对象中。
 * 当然你需要使用wmkcHash_new函数初始化一个新的wmkcHash对象。
 *
 * [EVP_MD_CTX_new](https://www.openssl.org/docs/man1.1.1/man3/EVP_MD_CTX_new.html)
 * @note 对于OpenSSL库提供的EVP相关功能，我查阅到OpenSSL库中的提示是
 *       返回值为1时表示无错误，为0时表示错误。
 *       为了不与本库产生冲突，暂时不对EVP相关功能进行返回值检查。
 * @param hash 指针，指向wmkcHash对象的地址。
 * @param fn 路径
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
**/
WMKC_PUBLIC(wmkcErr_obj) wmkcHash_file WMKC_OPEN_API
WMKC_OF((wmkcHash_obj *hash, wmkcCSTR fn))
{
    wmkcErr_obj error;
    if(!hash || !fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcHash_file",
            "hash or fn is NULL.");
    }
    EVP_MD_CTX *md_ctx = wmkcNull;
    wmkcFile_obj *file = wmkcNull;
    wmkcByte *buf = wmkcNull;
    wmkcSize index;

    if(!wmkcMem_new(wmkcByte *, buf, SN_HASH_BLOCKLEN)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcHash_file",
            "Failed to allocate memory for buf.");
    }

    error = wmkcFile_open(&file, fn, "rb");
    if(error.code) return error;

    wmkcSize quotient = file->fsize / SN_HASH_BLOCKLEN;
    wmkcSize leftover = file->fsize % SN_HASH_BLOCKLEN;

    md_ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(md_ctx, hash->md, wmkcNull);

    for(index = 0; index < quotient; ++index) {
        fread(buf, 1, SN_HASH_BLOCKLEN, file->fp);
        EVP_DigestUpdate(md_ctx, buf, SN_HASH_BLOCKLEN);
    }
    if(leftover) {
        fread(buf, 1, leftover, file->fp);
        EVP_DigestUpdate(md_ctx, buf, leftover);
    }
    EVP_DigestFinal_ex(md_ctx, hash->digest, wmkcNull);
    EVP_MD_CTX_free(md_ctx);

    error = wmkcBinascii_b2a_hex(&hash->hexdigest, hash->digest, hash->digestSize);
    if(error.code) {
        return error;
    }

    wmkcMem_free(buf);

    error = wmkcFile_close(&file);
    if(error.code) return error;
    wmkcErr_return(error, wmkcErr_OK, "wmkcHash_file", "OK.");
}

/**
 * @brief 为wmkcHash对象释放内存
 * @authors SN-Grotesque
 *
 * 此函数为wmkcHash对象释放内存。
 *
 * @note 无
 * @param obj 指针，指向wmkcHash对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
**/
WMKC_PUBLIC(wmkcErr_obj) wmkcHash_free WMKC_OPEN_API
WMKC_OF((wmkcHash_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcHash_free", "obj is NULL.");
    }
    if((*obj)->hexdigest) {
        wmkcMem_free((*obj)->hexdigest);
    }
    wmkcMem_free((*obj)->digest);
    wmkcMem_free((*obj));
    wmkcErr_return(error, wmkcErr_OK, "wmkcHash_free", "OK.");
}
