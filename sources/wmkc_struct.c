#include <wmkc_struct.h>

// 端序的格式字符
WMKC_PRIVATE_CONST(wmkcChar) wmkcStruct_byte_order[3] = {
    '@', // 按原样顺序
    '>', // 按大端序
    '<'  // 按小端序
};
// 数据格式的格式字符
WMKC_PRIVATE_CONST(wmkcChar) wmkcStruct_format_symbol[5] = {
    'H', // unsigned short -> uint16_t
    'I', // unsigned int   -> uint32_t
    'Q', // unsigned long  -> uint64_t
};

/**
 * @brief wmkcStruct库的私有函数，用于交换数组中的元素
 * @authors SN-Grotesque
 * 
 * wmkcStruct库的私有函数，用于交换数组中的元素
 * 
 * @note 无
 * @param array_1 指针，指向数组中元素的地址，并将
 *                其与array_2指向的元素进行交换。
 * @param array_2 指针，指向数组中元素的地址，并将
 *                其与array_1指向的元素进行交换。
 * @return 无
 */
WMKC_PRIVATE(wmkcVoid) wmkcStruct_swap_bytes
WMKC_OF((wmkcByte *array_1, wmkcByte *array_2))
{
    wmkcByte swap = *array_1;
    *array_1 = *array_2;
    *array_2 = swap;
}

/**
 * @brief 检查格式符是否正确
 * @authors SN-Grotesque
 * 
 * 此函数检查格式符是否正确
 * 
 * @note 此为wmkcStruct库的私有函数
 * @param src 指针，指向格式符字符串的地址
 * @return 返回一个布尔值，为True时代表格式符完全正确。
 */
WMKC_PRIVATE(wmkcBool) wmkcStruct_check_format
WMKC_OF((wmkcCSTR src))
{
    if(!src || (strlen(src) != 2)) {
        return false;
    }
    for(wmkc_u32 i = 0; i < sizeof(wmkcStruct_byte_order); ++i) {
        if(src[0] == wmkcStruct_byte_order[i]) {
            return true;
        }
    }
    for(wmkc_u32 i = 0; i < sizeof(wmkcStruct_format_symbol); ++i) {
        if(src[1] == wmkcStruct_format_symbol[i]) {
            return true;
        }
    }
    return false;
}

/**
 * @brief 此函数用于判断本机端序
 * @authors SN-Grotesque
 * 
 * 此函数用于判断本机是大端序还是小端序。
 * 
 * @note 无
 * @param 无
 * @return 返回值为一个布尔值，为True时，本机为小端序。否则为大端序。
*/
WMKC_PUBLIC(wmkcBool) wmkcStruct_PlatformEnd WMKC_OPEN_API
WMKC_OF((void))
{
    wmkc_u32 _tmp = 1;
    char *p = (char *)&_tmp;
    if(*p == 1)
        return true;
    return false;
}

/**
 * @brief 此函数用于将数据“打包”
 * @authors SN-Grotesque
 * 
 * 此函数用于将数据“打包”
 * 
 * @note 如果你感到一头雾水，那么请参考Python的struct.pack。
 * @param format 此参数为格式符（必须包含端序符和格式符）。
 *               在端序中，只支持【"\@"，">"，"<"】这三种。
 *               "\@"为按本机原本的端序，也就是保持不变。
 *               ">"为按大端序处理数据（如果本机是大端序，那么等同于"\@"）。
 *               "<"为按小端序处理数据（如果本机是小端序，那么等同于"\@"）。
 *               "H"为使用2字节长度的格式进行处理（uint16_t）。
 *               "I"为使用4字节长度的格式进行处理（uint32_t）。
 *               "Q"为使用8字节长度的格式进行处理（uint64_t）。
 * @param dst 指针，指向结果的地址（结果可以只是8字节长度）。
 * @param src 这是一个数字，它目前只能是整数。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStruct_pack WMKC_OPEN_API
WMKC_OF((wmkcCSTR format, wmkcByte *dst, wmkcSize src))
{
    wmkcErr_obj error;
    if(!format || !dst || !src) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcStruct_pack: format or dst or src is NULL");
    }
    if(!wmkcStruct_check_format(format)) {
        wmkcErr_return(error, wmkcErr_ErrType,
            "wmkcStruct_pack: Incorrect formatting symbol.");
    }
    wmkcBool little_end = wmkcStruct_PlatformEnd();
    wmkcByte order  = format[0]; // 字节顺序
    wmkcByte symbol = format[1]; // 格式字符
    wmkc_u32 size = 0;

    switch(symbol) {
        case 'H': size = 2; break;
        case 'I': size = 4; break;
        case 'Q': size = 8; break;
    }

    memcpy(dst, &src, size);

    if(order != '@') {
        if(!little_end) {
            if(order == '<') {
                switch(size) {
                    case 2:
                        wmkcStruct_swap_bytes(&dst[0], &dst[1]);
                        break;
                    case 4:
                        wmkcStruct_swap_bytes(&dst[0], &dst[3]);
                        wmkcStruct_swap_bytes(&dst[1], &dst[2]);
                        break;
                    case 8:
                        wmkcStruct_swap_bytes(&dst[0], &dst[7]);
                        wmkcStruct_swap_bytes(&dst[1], &dst[6]);
                        wmkcStruct_swap_bytes(&dst[2], &dst[5]);
                        wmkcStruct_swap_bytes(&dst[3], &dst[4]);
                        break;
                }
            }
        } else {
            if(order == '>') {
                switch(size) {
                    case 2:
                        wmkcStruct_swap_bytes(&dst[0], &dst[1]);
                        break;
                    case 4:
                        wmkcStruct_swap_bytes(&dst[0], &dst[3]);
                        wmkcStruct_swap_bytes(&dst[1], &dst[2]);
                        break;
                    case 8:
                        wmkcStruct_swap_bytes(&dst[0], &dst[7]);
                        wmkcStruct_swap_bytes(&dst[1], &dst[6]);
                        wmkcStruct_swap_bytes(&dst[2], &dst[5]);
                        wmkcStruct_swap_bytes(&dst[3], &dst[4]);
                        break;
                }
            }
        }
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 此函数用于将数据“解包”
 * @authors SN-Grotesque
 * 
 * 此函数用于将数据“解包”
 * 
 * @note 如果你感到一头雾水，那么请参考Python的struct.unpack。
 * @param format 此参数为格式符（必须包含端序符和格式符）。
 *               在端序中，只支持【"\@"，">"，"<"】这三种。
 *               "\@"为按本机原本的端序，也就是保持不变。
 *               ">"为按大端序处理数据（如果本机是大端序，那么等同于"\@"）。
 *               "<"为按小端序处理数据（如果本机是小端序，那么等同于"\@"）。
 *               "H"为使用2字节长度的格式进行处理（uint16_t）。
 *               "I"为使用4字节长度的格式进行处理（uint32_t）。
 *               "Q"为使用8字节长度的格式进行处理（uint64_t）。
 * @param dst 指针，指向结果的地址。
 * @param src 指针，指向二进制缓冲区的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcStruct_unpack WMKC_OPEN_API
WMKC_OF((wmkcCSTR format, wmkcVoid *dst, wmkcByte *src))
{
    /**
     * 先直接判断本机是大端排序还是小端排序，然后根据用户指定的
     * 格式字符来决定要不要翻转。
     * 比如如果本机是小端序，而用户指定的格式字符也是小端序，那么
     * 就不需要翻转，直接内存复制到数字变量中就行。
    */
    wmkcErr_obj error;
    if(!format || !dst || !src) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcStruct_unpack: format or dst or src is NULL");
    }
    if(!wmkcStruct_check_format(format)) {
        wmkcErr_return(error, wmkcErr_ErrType,
            "wmkcStruct_unpack: Incorrect formatting symbol.");
    }
    wmkcBool little_end = wmkcStruct_PlatformEnd();
    wmkcByte order  = format[0]; // 字节顺序
    wmkcByte symbol = format[1]; // 格式字符
    wmkc_u32 size = 0;

    switch(symbol) {
        case 'H': size = 2; break;
        case 'I': size = 4; break;
        case 'Q': size = 8; break;
    }

    wmkcByte *buf = wmkcNull;
    if(!wmkcMem_new(wmkcByte *, buf, size)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcStruct_unpack: "
            "Failed to allocate memory for buf.");
    }
    memcpy(buf, src, size);

    if(order != '@') {
        if(!little_end) {
            if(order == '<') {
                switch(size) {
                    case 2:
                        wmkcStruct_swap_bytes(&buf[0], &buf[1]);
                        break;
                    case 4:
                        wmkcStruct_swap_bytes(&buf[0], &buf[3]);
                        wmkcStruct_swap_bytes(&buf[1], &buf[2]);
                        break;
                    case 8:
                        wmkcStruct_swap_bytes(&buf[0], &buf[7]);
                        wmkcStruct_swap_bytes(&buf[1], &buf[6]);
                        wmkcStruct_swap_bytes(&buf[2], &buf[5]);
                        wmkcStruct_swap_bytes(&buf[3], &buf[4]);
                        break;
                }
            }
        } else {
            if(order == '>') {
                switch(size) {
                    case 2:
                        wmkcStruct_swap_bytes(&buf[0], &buf[1]);
                        break;
                    case 4:
                        wmkcStruct_swap_bytes(&buf[0], &buf[3]);
                        wmkcStruct_swap_bytes(&buf[1], &buf[2]);
                        break;
                    case 8:
                        wmkcStruct_swap_bytes(&buf[0], &buf[7]);
                        wmkcStruct_swap_bytes(&buf[1], &buf[6]);
                        wmkcStruct_swap_bytes(&buf[2], &buf[5]);
                        wmkcStruct_swap_bytes(&buf[3], &buf[4]);
                        break;
                }
            }
        }
    }

    memcpy(dst, buf, size);
    wmkcMem_free(buf);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}