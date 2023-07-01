#include <wmkc_key.h>

WMKC_PRIVATE_CONST(wmkcByte) wmkcKey_SBOX[256] = {
    0x63, 0x10, 0x6f, 0xcc, 0x70, 0x2b, 0xfc, 0x40, 0xda, 0x5b, 0x31, 0xaf, 0x54, 0xc0, 0xdd, 0xfa,
    0x16, 0xbf, 0x8f, 0x1c, 0x61, 0xf9, 0x77, 0xf6, 0x85, 0x5a, 0x4c, 0x1b, 0x30, 0x27, 0x01, 0x4e,
    0x9d, 0x71, 0xf3, 0x6d, 0x7a, 0x15, 0x73, 0x60, 0x92, 0xce, 0x37, 0x72, 0x32, 0x35, 0xea, 0xa3,
    0x84, 0x68, 0x3f, 0x55, 0x0a, 0x0b, 0x24, 0xe0, 0x4f, 0x5f, 0x69, 0x67, 0x59, 0x8e, 0xf1, 0x21,
    0xf5, 0x49, 0xe5, 0x4d, 0x80, 0xa6, 0x6a, 0x0f, 0xf7, 0x8c, 0xc1, 0xc6, 0xa0, 0x08, 0xab, 0xd3,
    0x05, 0xf4, 0x3a, 0x9a, 0x91, 0x0d, 0x4b, 0xd0, 0x6e, 0x79, 0x07, 0xd2, 0x87, 0x11, 0xf0, 0x1a,
    0xc7, 0xb7, 0x03, 0x39, 0x1d, 0x53, 0x28, 0xbd, 0xec, 0xb8, 0xfe, 0x04, 0x9c, 0x7e, 0x62, 0x06,
    0xd5, 0xe4, 0x7c, 0xfb, 0x74, 0x9f, 0xca, 0x18, 0xff, 0xb2, 0x0c, 0xa1, 0x86, 0x36, 0xc8, 0x9e,
    0xb0, 0x9b, 0x2e, 0x33, 0x48, 0x09, 0xdf, 0xef, 0x23, 0x3d, 0xc9, 0xa4, 0x56, 0xb5, 0x8b, 0xa2,
    0x3c, 0x76, 0x82, 0xb3, 0x2a, 0x26, 0x83, 0x98, 0x64, 0x81, 0xbe, 0xe6, 0x38, 0xe1, 0xf8, 0x58,
    0xb4, 0xd1, 0xc2, 0x96, 0xdb, 0x93, 0x19, 0xac, 0x52, 0xae, 0x0e, 0xe3, 0xa8, 0x51, 0x88, 0xd6,
    0xeb, 0x22, 0x20, 0x8d, 0x1e, 0x2d, 0x3e, 0x43, 0xc3, 0x78, 0x45, 0xa7, 0xde, 0x57, 0x44, 0xbb,
    0x13, 0xa9, 0x94, 0x14, 0x7f, 0x25, 0x7b, 0x5d, 0x42, 0x34, 0xee, 0x97, 0xb9, 0x29, 0xed, 0x2f,
    0xd9, 0xad, 0xfd, 0xe9, 0xc5, 0xd4, 0x99, 0xbc, 0xe8, 0xc4, 0xd7, 0xcd, 0xd8, 0xdc, 0xf2, 0x02,
    0x89, 0x90, 0x50, 0x1f, 0x41, 0xb1, 0xba, 0x8a, 0x95, 0xb6, 0xe2, 0xcb, 0xa5, 0x6b, 0x6c, 0x46,
    0x5c, 0x65, 0x75, 0x2c, 0x5e, 0x3b, 0x7d, 0x12, 0xaa, 0x17, 0xcf, 0x47, 0xe7, 0x4a, 0x00, 0x66
};

WMKC_PRIVATE_CONST(wmkcByte) wmkcKey_RSBOX[256] = {
    0xfe, 0x1e, 0xdf, 0x62, 0x6b, 0x50, 0x6f, 0x5a, 0x4d, 0x85, 0x34, 0x35, 0x7a, 0x55, 0xaa, 0x47,
    0x01, 0x5d, 0xf7, 0xc0, 0xc3, 0x25, 0x10, 0xf9, 0x77, 0xa6, 0x5f, 0x1b, 0x13, 0x64, 0xb4, 0xe3,
    0xb2, 0x3f, 0xb1, 0x88, 0x36, 0xc5, 0x95, 0x1d, 0x66, 0xcd, 0x94, 0x05, 0xf3, 0xb5, 0x82, 0xcf,
    0x1c, 0x0a, 0x2c, 0x83, 0xc9, 0x2d, 0x7d, 0x2a, 0x9c, 0x63, 0x52, 0xf5, 0x90, 0x89, 0xb6, 0x32,
    0x07, 0xe4, 0xc8, 0xb7, 0xbe, 0xba, 0xef, 0xfb, 0x84, 0x41, 0xfd, 0x56, 0x1a, 0x43, 0x1f, 0x38,
    0xe2, 0xad, 0xa8, 0x65, 0x0c, 0x33, 0x8c, 0xbd, 0x9f, 0x3c, 0x19, 0x09, 0xf0, 0xc7, 0xf4, 0x39,
    0x27, 0x14, 0x6e, 0x00, 0x98, 0xf1, 0xff, 0x3b, 0x31, 0x3a, 0x46, 0xed, 0xee, 0x23, 0x58, 0x02,
    0x04, 0x21, 0x2b, 0x26, 0x74, 0xf2, 0x91, 0x16, 0xb9, 0x59, 0x24, 0xc6, 0x72, 0xf6, 0x6d, 0xc4,
    0x44, 0x99, 0x92, 0x96, 0x30, 0x18, 0x7c, 0x5c, 0xae, 0xe0, 0xe7, 0x8e, 0x49, 0xb3, 0x3d, 0x12,
    0xe1, 0x54, 0x28, 0xa5, 0xc2, 0xe8, 0xa3, 0xcb, 0x97, 0xd6, 0x53, 0x81, 0x6c, 0x20, 0x7f, 0x75,
    0x4c, 0x7b, 0x8f, 0x2f, 0x8b, 0xec, 0x45, 0xbb, 0xac, 0xc1, 0xf8, 0x4e, 0xa7, 0xd1, 0xa9, 0x0b,
    0x80, 0xe5, 0x79, 0x93, 0xa0, 0x8d, 0xe9, 0x61, 0x69, 0xcc, 0xe6, 0xbf, 0xd7, 0x67, 0x9a, 0x11,
    0x0d, 0x4a, 0xa2, 0xb8, 0xd9, 0xd4, 0x4b, 0x60, 0x7e, 0x8a, 0x76, 0xeb, 0x03, 0xdb, 0x29, 0xfa,
    0x57, 0xa1, 0x5b, 0x4f, 0xd5, 0x70, 0xaf, 0xda, 0xdc, 0xd0, 0x08, 0xa4, 0xdd, 0x0e, 0xbc, 0x86,
    0x37, 0x9d, 0xea, 0xab, 0x71, 0x42, 0x9b, 0xfc, 0xd8, 0xd3, 0x2e, 0xb0, 0x68, 0xce, 0xca, 0x87,
    0x5e, 0x3e, 0xde, 0x22, 0x51, 0x40, 0x17, 0x48, 0x9e, 0x15, 0x0f, 0x73, 0x06, 0xd2, 0x6a, 0x78
};

/**
 * @brief 这堆私有函数的注释我实在不想写。
 * @authors SN-Grotesque
 * @example 反正就那样，也不需要写，这谁看不懂。
*/
WMKC_PRIVATE(wmkcVoid) _wmkcKey_SubBytes
WMKC_OF((wmkcKey_state *buf))
{
    (*buf)[0][0] = wmkcKey_SBOX[(*buf)[0][0]];
    (*buf)[0][1] = wmkcKey_SBOX[(*buf)[0][1]];
    (*buf)[1][0] = wmkcKey_SBOX[(*buf)[1][0]];
    (*buf)[1][1] = wmkcKey_SBOX[(*buf)[1][1]];
}

WMKC_PRIVATE(wmkcVoid) _wmkcKey_InvSubBytes
WMKC_OF((wmkcKey_state *buf))
{
    (*buf)[0][0] = wmkcKey_RSBOX[(*buf)[0][0]];
    (*buf)[0][1] = wmkcKey_RSBOX[(*buf)[0][1]];
    (*buf)[1][0] = wmkcKey_RSBOX[(*buf)[1][0]];
    (*buf)[1][1] = wmkcKey_RSBOX[(*buf)[1][1]];
}

WMKC_PRIVATE(wmkcVoid) _wmkcKey_MatrixRows
WMKC_OF((wmkcKey_state *buf))
{
    wmkcByte swap;
    swap = (*buf)[0][0];
    (*buf)[0][0] = (*buf)[0][1];
    (*buf)[0][1] = swap;
    swap = (*buf)[1][0];
    (*buf)[1][0] = (*buf)[1][1];
    (*buf)[1][1] = swap;
}

WMKC_PRIVATE(wmkcVoid) _wmkcKey_MatrixMixs
WMKC_OF((wmkcKey_state *buf))
{
    (*buf)[0][0] = (*buf)[0][0] ^ (*buf)[0][1] ^ (*buf)[1][0];
    (*buf)[0][1] = (*buf)[0][1] ^ (*buf)[1][0] ^ (*buf)[1][1];
    (*buf)[1][0] = (*buf)[1][0] ^ (*buf)[1][1] ^ (*buf)[0][0];
    (*buf)[1][1] = (*buf)[1][1] ^ (*buf)[0][0] ^ (*buf)[0][1];
}

WMKC_PRIVATE(wmkcVoid) _wmkcKey_InvMatrixMixs
WMKC_OF((wmkcKey_state *buf))
{
    (*buf)[1][1] = (*buf)[1][1] ^ (*buf)[0][0] ^ (*buf)[0][1];
    (*buf)[1][0] = (*buf)[1][0] ^ (*buf)[1][1] ^ (*buf)[0][0];
    (*buf)[0][1] = (*buf)[0][1] ^ (*buf)[1][0] ^ (*buf)[1][1];
    (*buf)[0][0] = (*buf)[0][0] ^ (*buf)[0][1] ^ (*buf)[1][0];
}

WMKC_PRIVATE(wmkcVoid) _wmkcKey_Cipher
WMKC_OF((wmkcKey_state *buf))
{
    for(wmkc_u32 x = 0; x < WMKC_KEY_CRYPTO_NR; ++x) {
        _wmkcKey_MatrixRows(buf);
        _wmkcKey_SubBytes(buf);
        _wmkcKey_MatrixMixs(buf);
    }
}

WMKC_PRIVATE(wmkcVoid) _wmkcKey_InvCipher
WMKC_OF((wmkcKey_state *buf))
{
    for(wmkc_u32 x = 0; x < WMKC_KEY_CRYPTO_NR; ++x) {
        _wmkcKey_InvMatrixMixs(buf);
        _wmkcKey_InvSubBytes(buf);
        _wmkcKey_MatrixRows(buf);
    }
}

/**
 * @brief 为wmkcKey对象申请内存空间
 * @authors SN-Grotesque
 * 
 * 为wmkcKey对象申请内存空间并初始化
 * 
 * @note 如果key或者keySize为空，那么mode参数无效。
 * @param obj 指针，指向wmkcKey对象指针的地址。
 * @param key 指针，指向密钥数据的地址，可以为空。
 * @param keySize 这是一个长度，为key指针指向的密钥数据的长度。
 * @param mode 这是一个模式，1为加密，0为解密。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcKey_new WMKC_OPEN_API
WMKC_OF((wmkcKey_obj **obj, wmkcByte *key, wmkc_u32 keySize, wmkc_u32 mode))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcKey_new: obj is NULL.");
    }

    if(!wmkcMem_new(wmkcKey_obj *, (*obj), sizeof(wmkcKey_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcKey_new: Failed to allocate memory for (*obj).");
    }

    if(key && keySize) {
        if(!wmkcMem_new(wmkcByte *, (*obj)->key, keySize)) {
            wmkcErr_return(error, wmkcErr_ErrMemory,
                "wmkcKey_new: Failed to allocate memory for (*obj)->key.");
        }
        memcpy((*obj)->key, key, keySize);
        (*obj)->size = keySize;
        (*obj)->mode = mode;
    } else {
        (*obj)->key = wmkcNull;
        (*obj)->size = 0;
        (*obj)->mode = mode;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 为wmkcKey对象释放内存空间
 * @authors SN-Grotesque
 * 
 * 为wmkcKey对象释放内存空间
 * 
 * @note 无
 * @param obj 指针，指向wmkcKey对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcKey_free WMKC_OPEN_API
WMKC_OF((wmkcKey_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcKey_free: obj is NULL.");
    }

    if((*obj)->key) {
        wmkcMem_free((*obj)->key);
    }
    wmkcMem_free(*obj);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 将密钥数据进行混淆与弱加密
 * @authors SN-Grotesque
 * 
 * 混淆与弱加密wmkcKey对象key成员指针指向的密钥
 * 
 * @note 此函数只能处理长度为4的倍数的数据。
 * @param obj 指针，指向wmkcKey对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcKey_CryptKey WMKC_OPEN_API
WMKC_OF((wmkcKey_obj *obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcKey_CryptKey: obj is NULL.");
    }
    if(obj->size % WMKC_KEY_CRYPTO_BLOCKLEN) {
        wmkcErr_return(error, wmkcErr_ErrType,
            "wmkcKey_CryptKey: The length of the key should be a multiple of 4.");
    }
    wmkc_u32 x;

    if(obj->mode) {
        for(x = 0; x < obj->size; x += WMKC_KEY_CRYPTO_BLOCKLEN)
            _wmkcKey_Cipher((wmkcKey_state *)(obj->key + x));
    } else {
        for(x = 0; x < obj->size; x += WMKC_KEY_CRYPTO_BLOCKLEN)
            _wmkcKey_InvCipher((wmkcKey_state *)(obj->key + x));
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 导入密钥
 * @authors SN-Grotesque
 * 
 * 根据指定的路径导入密钥，如果obj->mode等于false，那么会同时进行解密。
 * 
 * @note 未完成
 * @param obj 指针，指向wmkcKey对象指针的地址。
 * @param fn 指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcKey_loadKey WMKC_OPEN_API
WMKC_OF((wmkcKey_obj *obj, wmkcCSTR fn))
{
    wmkcErr_obj error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 写入密钥
 * @authors SN-Grotesque
 * 
 * 根据指定的路径写入密钥，如果obj->mode等于true，那么会同时进行加密。
 * 
 * @note 未完成
 * @param obj 指针，指向wmkcKey对象指针的地址。
 * @param fn 指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcKey_saveKey WMKC_OPEN_API
WMKC_OF((wmkcKey_obj *obj, wmkcCSTR fn))
{
    wmkcErr_obj error;
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
