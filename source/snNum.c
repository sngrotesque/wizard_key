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
SN_FUNC_OF((const snChar *src))
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

SN_PUBLIC(snError) snNum_pack SN_OPEN_API
SN_FUNC_OF((const snChar *format, snByte *dst, snVoid *src))
{
    if(!format || !dst || !src) {
        return snErr_ErrNullData;
    }
    if(!snNum_check_format(format)) {
        return snErr_ErrInvalid;
    }
    snBool little_end = snNum_PlatformEnd();
    snByte order  = format[0]; // 字节顺序
    snByte symbol = format[1]; // 格式字符
    snByte swap = 0;           // 用于交换数据的缓冲区
    sn_u32 size = 0;

    switch(symbol) {
        case 'H': size = 2; break;
        case 'I': size = 4; break;
        case 'Q': size = 8; break;
    }

    printf("symbol: %c\n", symbol);
    printf("order:  %c\n", order);
    printf("size:   %u\n", size);

    memcpy(dst, src, size);

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

    return snErr_OK;
}

SN_PUBLIC(snError) snNum_unpack SN_OPEN_API
SN_FUNC_OF((const snChar *format, snVoid *dst, snByte *src))
{

    return snErr_OK;
}