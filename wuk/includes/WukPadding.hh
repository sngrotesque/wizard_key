#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>
#include <WukRandom.hh>

namespace wuk {
    /**
    * @brief wukPad提供的默认填充方法（需要内存空间长度大于等于填充后长度）
    * @authors SN-Grotesque
    * @note 无
    * @param data 源内容
    * @param length 传入时作为源内容长度，函数调用结束后作为填充后的长度
    * @param blockSize 填充的块大小
    * @param randVal 是否使用随机数填充
    * @return 无
    */
    LIBWUK_API void pad(wByte *data, wSize &length, wU32 blockSize);

    /**
    * @brief wukPad提供的默认清除填充方法
    * @authors SN-Grotesque
    * @note 无
    * @param data 源内容
    * @param length 传入时作为源内容长度，函数调用结束后作为清除填充后的长度
    * @return 无
    */
    LIBWUK_API void unpad(wByte *data, wSize &length);

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

    // 填充的最后一个字节为填充数，其余数均为0。
    LIBWUK_API wByte *x923_pad(const wByte *data, wSize &length, wU32 blockSize);
    LIBWUK_API wByte *x923_unpad(const wByte *data, wSize &length);

    // 若数据最后一个字节的最高位是1则填充0xff否则0x00。
    LIBWUK_API wByte *tbc_pad(const wByte *data, wSize &length, wU32 blockSize);
    LIBWUK_API wByte *tbc_unpad(const wByte *data, wSize &length);

    // 随机填充
    LIBWUK_API wByte *rand_pad(const wByte *data, wSize &length, wU32 blockSize);
    LIBWUK_API wByte *rand_unpad(const wByte *data, wSize &length);
}

#endif
