#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_PAD
#define WUK_CPP_PAD
#include <config/WukException.hh>
#include <WukRandom.hh>

namespace wuk {
    /**
    * @brief wukPad提供的默认填充方法（需要内存空间长度大于等于填充后长度）
    * @authors SN-Grotesque
    * @note 无
    * @param src 源内容
    * @param size 传入时作为源内容长度，函数调用结束后作为填充后的长度
    * @param blockSize 填充的块大小
    * @param randVal 是否使用随机数填充
    * @return 无
    */
    LIBWUK_API void pad(wByte *src, wSize &size, wU32 blockSize, bool randVal);

    /**
    * @brief wukPad提供的默认清除填充方法
    * @authors SN-Grotesque
    * @note 无
    * @param src 源内容
    * @param size 传入时作为源内容长度，函数调用结束后作为清除填充后的长度
    * @return 无
    */
    LIBWUK_API void unpad(wByte *src, wSize &size);

    /**
    * @brief PKCS7填充
    * @authors SN-Grotesque
    * @note 无
    * @param src 源内容
    * @param size 传入时作为源内容长度，函数调用结束后作为填充后的长度
    * @param blockSize 填充的块大小
    * @return 一个指向填充后的数据的指针(new [])。
    */
    LIBWUK_API wByte *pkcs7_pad(const wByte *src, wSize &size, wU32 blockSize);

    /**
    * @brief PKCS7填充删除
    * @authors SN-Grotesque
    * @note 无
    * @param src 源内容
    * @param size 传入时作为源内容长度，函数调用结束后作为清除填充后的长度
    * @return 一个指向填充后的数据的指针(new [])。
    */
    LIBWUK_API wByte *pkcs7_unpad(const wByte *src, wSize &size);
}

#endif /* WUK_CPP_PAD */
#endif
