#include <snNum.h>

SN_PRIVATE_CONST(snChar) snNum_byte_order[3] = {
    '@', // 按原样顺序
    '>', // 按大端序
    '<'  // 按小端序
};
SN_PRIVATE_CONST(snChar) snNum_format_symbol[5] = {
    'B', // unsigned char  -> uint8_t
    'H', // unsigned short -> uint16_t
    'I', // unsigned int   -> uint32_t
    'Q', // unsigned long  -> uint64_t
    'N'  // unsigned long  -> size_t
};

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
SN_FUNC_OF((const snChar *format, snNum_ctx *obj))
{


    return snErr_OK;
}

SN_PUBLIC(snError) snNum_unpack SN_OPEN_API
SN_FUNC_OF((const snChar *format, snNum_ctx *obj))
{

    return snErr_OK;
}