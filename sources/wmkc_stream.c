#include <wmkc_stream.h>

/**
 * @brief 此函数将参数二与参数三拼接
 * @authors SN-Grotesque
 * 
 * 此函数将参数二与参数三拼接，会自动申请内存空间。
 * 
 * @note 无
 * @param dst 这是一个指针，指向结果指针的地址。
 * @param left 这是一个指针，指向左侧内容的wmkc对象的地址。
 * @param right 这是一个指针，指向右侧内容的wmkc对象的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Splicing WMKC_OPEN_API
WMKC_OF((wmkc_obj **dst, wmkc_obj *left, wmkc_obj *right))
{
    wmkcErr_obj error;
    if(!dst || !left || !right) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcStream_Splicing: dst or left or right is NULL.");
    }

    (*dst)->size = (left->size + right->size);
    if(!(*dst)->size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcStream_Splicing: dst->size is zero.");
    }

    wmkcObject_new(dst, (left->size + right->size) + 1, true);

    memcpy((*dst)->buf, left->buf, left->size);
    memcpy((*dst)->buf + left->size, right->buf, right->size);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 此函数将内容进行切片
 * @authors SN-Grotesque
 * 
 * 此函数将内容进行切片，会自动申请内存空间。
 * 
 * @note 无
 * @param dst 这是一个指针，指向结果指针的地址。
 * @param src 这是一个指针，指向源内容wmkc对象的地址。
 * @param start 这是起始下标，用于标注切片的起始位置。
 * @param end 这是末尾下标，用于标注切片的末尾位置。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Slice WMKC_OPEN_API
WMKC_OF((wmkc_obj **dst, wmkc_obj *src, wmkcSSize start, wmkcSSize end))
{
    wmkcErr_obj error;
    if(end < 0) {
        if((end = src->size + end) > src->size) {
            wmkcErr_return(error, wmkcErr_ErrOutRange,
                "wmkcStream_Slice: The range at the end is too large.");
        }
    } else if(end > src->size) {
        wmkcErr_return(error, wmkcErr_ErrOutRange,
            "wmkcStream_Slice: The range at the end is too large.");
    }
    if(start < 0) {
        if((start = src->size + start) > src->size) {
            wmkcErr_return(error, wmkcErr_ErrOutRange,
                "wmkcStream_Slice: The range at the start is too large.");
        }
    } else if(start > src->size) {
        wmkcErr_return(error, wmkcErr_ErrOutRange,
            "wmkcStream_Slice: The range at the start is too large.");
    }
    if(start > end) {
        if(end) {
        wmkcErr_return(error, wmkcErr_ErrInvalidRange,
            "wmkcStream_Slice: The starting subscript exceeds the ending subscript.");
        } else {
            end = src->size;
        }
    }

    // printf("size: %lld\n", src->size);
    // printf("start: %lld\n", start);
    // printf("end: %lld\n", end);

    wmkcObject_new(dst, (end - start) + 1, false);
    (*dst)->buf[end - start] = 0x00;
    (*dst)->size = (end - start);

    memcpy((*dst)->buf, src->buf + start, (*dst)->size);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 此函数用于替换scanf函数
 * @authors SN-Grotesque
 * 
 * 此函数内容与scanf函数类型，但更加安全。
 * 
 * @note 无
 * @param buf 这是一个指针，指向缓冲区的地址。
 * @param size 这是一个指针，代表需要输入至缓冲区的内容长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Scanf WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size))
{
    wmkcErr_obj error;
    if(!buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcStream_Scanf: buf or size is NULL.");
    }

    for(wmkcSize x = 0; x < size; ++x) {
        if((buf[x] = getchar()) == 0x0a) {
            buf[x] = 0x00;
            break;
        }
    }

    if(!(*buf)) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcStream_Scanf: buf is NULL.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 此函数用于反转缓冲区里的数据
 * @authors SN-Grotesque
 * 
 * 此函数用于反转缓冲区里的数据。
 * 
 * @note 无
 * @param buf 这是一个指针，指向缓冲区的地址。
 * @param size 这是一个指针，代表缓冲区的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Reverse WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size))
{
    wmkcErr_obj error;
    if(!buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcStream_Reverse: buf or size is NULL.");
    }
    wmkcSize i;
    wmkcByte buf;
    for(i = 0; i < size / 2; ++i) {
        buf = buf[i];
        buf[i] = buf[size - i - 1];
        buf[size - i - 1] = buf;
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 此函数用于求出指定数字的二进制值
 * @authors SN-Grotesque
 * 
 * 此函数用于求出指定数字的二进制值并保存到dst指针指向的地址中。
 * 
 * @note 此函数后续可以考虑一下与大数库结合，或者将源数字类型改为wmkcFloat。
 * @param dst 这是一个指针，指向结果指针的地址。
 * @param src 这是一个数字，为源数字的内容。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStream_Binary WMKC_OPEN_API
WMKC_OF((wmkcByte **dst, wmkcSize src))
{
    wmkcErr_obj error;
    if(!dst || !src) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcStream_Binary: dst or src is NULL.");
    }
    wmkcSize temp;
    wmkcSize size;
    wmkcSize i;

    for(temp = src, size = 0; temp; size += 8, temp >>= 8) {}

    if(!wmkcMemoryNew(wmkcByte *, (*dst), size + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcStream_Binary: (*dst) failed to apply for memory.");
    }
    (*dst)[size] = 0x00;
    memset((*dst), 0x30, size);

    for(i = 0; src; src >>= 1, ++i) {
        (*dst)[i] = (src & 0x1) + 0x30;
    }

    wmkcStream_Reverse((*dst), size);
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 此函数用于判断两个内容是否相同
 * @authors SN-Grotesque
 * 
 * 此函数用于判断两个内容是否相同，内部使用strncmp函数实现。
 * 
 * @note 无
 * @param src1 这是一个指针，指向源数据1。
 * @param src2 这是一个指针，指向源数据2。
 * @param size 这是一个长度，代表源数据的长度（如果长度不同那你还比较什么呢？）
 * @return 返回一个布尔值，为True时两个源数据相同，否则不同。
*/
WMKC_PUBLIC(wmkcBool) wmkcStream_Compare WMKC_OPEN_API
WMKC_OF((wmkcByte *src1, wmkcByte *src2, wmkcSize size))
{
    if(!src1 || !src2 || size) {
        return false;
    }
    if(!strncmp((wmkcChar *)src1, (wmkcChar *)src2, size)) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief 此函数用于求出一个整数的长度位数
 * @authors SN-Grotesque
 * 
 * 此函数用于求出一个整数的长度位数（通过除法判断）
 * 
 * @note 无
 * @param src 这是源数字。
 * @return 返回一个wmkc_u32值为长度。
*/
WMKC_PUBLIC(wmkc_u32) wmkcStream_numlen WMKC_OPEN_API
WMKC_OF((wmkcSize src))
{
    wmkcSize len = 0;
    for(wmkcSize num = src; num > 0; ++len) num /= 10;
    return len;
}
