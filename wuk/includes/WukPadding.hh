#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT

namespace wuk {
    /**
    * @brief PKCS7填充
    * @authors SN-Grotesque
    * @note 无
    * @param data 源内容
    * @param length 传入时作为源内容长度，函数调用结束后作为填充后的长度
    * @param blockSize 填充的块大小
    * @return 一个指向填充后的数据的指针(new [])。
    */
    LIBWUK_API wByte *pkcs7_pad(const wByte *data, wSize &length, wU32 blockSize);

    /**
    * @brief PKCS7填充删除
    * @authors SN-Grotesque
    * @note 无
    * @param data 源内容
    * @param length 传入时作为源内容长度，函数调用结束后作为清除填充后的长度
    * @return 一个指向填充后的数据的指针(new [])。
    */
    LIBWUK_API wByte *pkcs7_unpad(const wByte *data, wSize &length);
}

#endif
