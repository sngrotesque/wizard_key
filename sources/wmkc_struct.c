#include <wmkc_struct.h>

WMKC_PRIVATE_CONST(wmkcChar) wmkcStruct_byte_order[3] = {
    '@', // 按原样顺序
    '>', // 按大端序
    '<'  // 按小端序
};
WMKC_PRIVATE_CONST(wmkcChar) wmkcStruct_format_symbol[5] = {
    'H', // unsigned short -> uint16_t
    'I', // unsigned int   -> uint32_t
    'Q', // unsigned long  -> uint64_t
};

WMKC_PRIVATE(wmkcVoid) wmkcStruct_swap_bytes
WMKC_OF((wmkcByte *array_1, wmkcByte *array_2))
{
    wmkcByte swap = *array_1;
    *array_1 = *array_2;
    *array_2 = swap;
}

WMKC_PRIVATE(wmkcBool) wmkcStruct_check_format
WMKC_OF((wmkcString src))
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

// 用来判断当前计算机是大端排序还是小端排序。
WMKC_PUBLIC(wmkcBool) wmkcStruct_PlatformEnd WMKC_OPEN_API
WMKC_OF((void))
{
    wmkc_u32 _tmp = 1;
    char *p = (char *)&_tmp;
    if(*p == 1)
        return true;
    return false;
}

WMKC_PUBLIC(wmkcErr_ctx) wmkcStruct_pack WMKC_OPEN_API
WMKC_OF((wmkcString format, wmkcByte *dst, wmkcSize src))
{
    wmkcErr_ctx error;
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

WMKC_PUBLIC(wmkcErr_ctx) wmkcStruct_unpack WMKC_OPEN_API
WMKC_OF((wmkcString format, wmkcVoid *dst, wmkcByte *src))
{
    /**
     * 先直接判断本机是大端排序还是小端排序，然后根据用户指定的
     * 格式字符来决定要不要翻转。
     * 比如如果本机是小端序，而用户指定的格式字符也是小端序，那么
     * 就不需要翻转，直接内存复制到数字变量中就行。
    */
    wmkcErr_ctx error;
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

    if(order != '@') {
        if(!little_end) {
            if(order == '<') {
                switch(size) {
                    case 2:
                        wmkcStruct_swap_bytes(&src[0], &src[1]);
                        break;
                    case 4:
                        wmkcStruct_swap_bytes(&src[0], &src[3]);
                        wmkcStruct_swap_bytes(&src[1], &src[2]);
                        break;
                    case 8:
                        wmkcStruct_swap_bytes(&src[0], &src[7]);
                        wmkcStruct_swap_bytes(&src[1], &src[6]);
                        wmkcStruct_swap_bytes(&src[2], &src[5]);
                        wmkcStruct_swap_bytes(&src[3], &src[4]);
                        break;
                }
            }
        } else {
            if(order == '>') {
                switch(size) {
                    case 2:
                        wmkcStruct_swap_bytes(&src[0], &src[1]);
                        break;
                    case 4:
                        wmkcStruct_swap_bytes(&src[0], &src[3]);
                        wmkcStruct_swap_bytes(&src[1], &src[2]);
                        break;
                    case 8:
                        wmkcStruct_swap_bytes(&src[0], &src[7]);
                        wmkcStruct_swap_bytes(&src[1], &src[6]);
                        wmkcStruct_swap_bytes(&src[2], &src[5]);
                        wmkcStruct_swap_bytes(&src[3], &src[4]);
                        break;
                }
            }
        }
    }

    memcpy(dst, src, size);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}