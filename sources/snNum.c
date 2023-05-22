#include <snNum.h>

SN_PRIVATE_CONST(snChar) snNum_byte_order[3] = {
    '@', // 按原样顺序
    '>', // 按大端序
    '<'  // 按小端序
};
SN_PRIVATE_CONST(snChar) snNum_format_symbol[5] = {
    'H', // unsigned short -> uint16_t
    'I', // unsigned int   -> uint32_t
    'Q', // unsigned long  -> uint64_t
};

SN_PRIVATE(snVoid) snNum_swap_bytes
SN_FUNC_OF((snByte *array_1, snByte *array_2))
{
    snByte swap = *array_1;
    *array_1 = *array_2;
    *array_2 = swap;
}

SN_PRIVATE(snBool) snNum_check_format
SN_FUNC_OF((snString src))
{
    if(!src || (strlen(src) != 2)) {
        return false;
    }
    for(sn_u32 i = 0; i < sizeof(snNum_byte_order); ++i) {
        if(src[0] == snNum_byte_order[i]) {
            return true;
        }
    }
    for(sn_u32 i = 0; i < sizeof(snNum_format_symbol); ++i) {
        if(src[1] == snNum_format_symbol[i]) {
            return true;
        }
    }
    return false;
}

// 用来判断当前计算机是大端排序还是小端排序。
SN_PUBLIC(snBool) snNum_PlatformEnd SN_OPEN_API
SN_FUNC_OF((void))
{
    sn_u32 _tmp = 1;
    char *p = (char *)&_tmp;
    if(*p == 1)
        return true;
    return false;
}

SN_PUBLIC(snErr_ctx) snNum_pack SN_OPEN_API
SN_FUNC_OF((snString format, snByte *dst, snSize src))
{
    snErr_ctx error;
    if(!format || !dst || !src) {
        snErr_return(error, snErr_ErrNullData,
            "snNum_pack: format or dst or src is NULL");
    }
    if(!snNum_check_format(format)) {
        snErr_return(error, snErr_ErrInvalid,
            "snNum_pack: Incorrect formatting symbol.");
    }
    snBool little_end = snNum_PlatformEnd();
    snByte order  = format[0]; // 字节顺序
    snByte symbol = format[1]; // 格式字符
    sn_u32 size = 0;

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
                        snNum_swap_bytes(&dst[0], &dst[1]);
                        break;
                    case 4:
                        snNum_swap_bytes(&dst[0], &dst[3]);
                        snNum_swap_bytes(&dst[1], &dst[2]);
                        break;
                    case 8:
                        snNum_swap_bytes(&dst[0], &dst[7]);
                        snNum_swap_bytes(&dst[1], &dst[6]);
                        snNum_swap_bytes(&dst[2], &dst[5]);
                        snNum_swap_bytes(&dst[3], &dst[4]);
                        break;
                }
            }
        } else {
            if(order == '>') {
                switch(size) {
                    case 2:
                        snNum_swap_bytes(&dst[0], &dst[1]);
                        break;
                    case 4:
                        snNum_swap_bytes(&dst[0], &dst[3]);
                        snNum_swap_bytes(&dst[1], &dst[2]);
                        break;
                    case 8:
                        snNum_swap_bytes(&dst[0], &dst[7]);
                        snNum_swap_bytes(&dst[1], &dst[6]);
                        snNum_swap_bytes(&dst[2], &dst[5]);
                        snNum_swap_bytes(&dst[3], &dst[4]);
                        break;
                }
            }
        }
    }

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snNum_unpack SN_OPEN_API
SN_FUNC_OF((snString format, snVoid *dst, snByte *src))
{
    /**
     * 先直接判断本机是大端排序还是小端排序，然后根据用户指定的
     * 格式字符来决定要不要翻转。
     * 比如如果本机是小端序，而用户指定的格式字符也是小端序，那么
     * 就不需要翻转，直接内存复制到数字变量中就行。
    */
    snErr_ctx error;
    if(!format || !dst || !src) {
        snErr_return(error, snErr_ErrNullData,
            "snNum_unpack: format or dst or src is NULL");
    }
    if(!snNum_check_format(format)) {
        snErr_return(error, snErr_ErrInvalid,
            "snNum_unpack: Incorrect formatting symbol.");
    }
    snBool little_end = snNum_PlatformEnd();
    snByte order  = format[0]; // 字节顺序
    snByte symbol = format[1]; // 格式字符
    sn_u32 size = 0;

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
                        snNum_swap_bytes(&src[0], &src[1]);
                        break;
                    case 4:
                        snNum_swap_bytes(&src[0], &src[3]);
                        snNum_swap_bytes(&src[1], &src[2]);
                        break;
                    case 8:
                        snNum_swap_bytes(&src[0], &src[7]);
                        snNum_swap_bytes(&src[1], &src[6]);
                        snNum_swap_bytes(&src[2], &src[5]);
                        snNum_swap_bytes(&src[3], &src[4]);
                        break;
                }
            }
        } else {
            if(order == '>') {
                switch(size) {
                    case 2:
                        snNum_swap_bytes(&src[0], &src[1]);
                        break;
                    case 4:
                        snNum_swap_bytes(&src[0], &src[3]);
                        snNum_swap_bytes(&src[1], &src[2]);
                        break;
                    case 8:
                        snNum_swap_bytes(&src[0], &src[7]);
                        snNum_swap_bytes(&src[1], &src[6]);
                        snNum_swap_bytes(&src[2], &src[5]);
                        snNum_swap_bytes(&src[3], &src[4]);
                        break;
                }
            }
        }
    }

    memcpy(dst, src, size);

    snErr_return(error, snErr_OK, "OK.");
}