#include <wmkc_pad.h>

/**
 * @brief 填充函数
 * @authors SN-Grotesque
 * 
 * 将内容填充，直到块长度的倍数
 * 
 * @note 如果randomPadding为True则忽略WMKC_PAD_VALUE。否则使用WMKC_PAD_VALUE。
 *       后续考虑一下参数是否应改为传入wmkc对象或src与dst这种形式。
 *       因为当前这个函数成功执行的前提是必须保证传入的指针指向的内容长度
 *       大于等于总长度的（也就是加上填充数据后的长度）。
 * @param buf 这是一个指针，指向需要被填充的内容的地址。
 * @param size 这是一个指针，指向buf指向的内容的长度的值（会被改变）。
 * @param BlockSize 这是块长度，数据将被填充至这个数字的倍数。
 * @param randomPadding 如果为True，那么使用随机数填充。
 * @return 无
*/
WMKC_PUBLIC(wmkcVoid) wmkcPad_add WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize *size, wmkc_u32 BlockSize, wmkcBool randomPadding))
{
    wmkcSize padoffset;
    wmkcSize totalSize;
    wmkcSize index;

    padoffset = wmkcPad_offset(BlockSize, *size);
    totalSize = padoffset + *size;

    switch(randomPadding) {
        case true:
            wmkcRandom_seed();
            for(index = *size; index < totalSize - 1; ++index)
                buf[index] = wmkcRandom_rand();
            break;
        default:
            for(index = *size; index < totalSize - 1; ++index)
                buf[index] = WMKC_PAD_VALUE;
            break;
    }

    *size = totalSize;

    buf[*size - 1] = padoffset & 0xff;
    if(BlockSize == padoffset && BlockSize == 256) {
        buf[*size - 1] = 0xff;
        buf[*size - 2] = 0xff;
    }
}

/**
 * @brief 删除填充的值
 * @authors SN-Grotesque
 * 
 * 删除填充的数据，全部归零
 * 
 * @note 此函数不会改变buf指向的内容的总长度，只是将填充的数据全部替换成00。
 * @param buf 这是一个指针，指向需要被填充的内容的地址。
 * @param size 这是一个指针，指向buf指向的内容的长度的值（会被改变）。
 * @return 无
*/
WMKC_PUBLIC(wmkcVoid) wmkcPad_remove WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize *size))
{
    wmkcSize padoffset;
    wmkcSize index;

    padoffset = buf[*size - 1];

    if((buf[*size - 1] & buf[*size - 2]) == 0xff)
        padoffset = 256;

    for(index = *size - padoffset; index < *size; ++index)
        buf[index] = 0x00;

    *size = *size - padoffset;
}

/**
 * @brief Token填充函数
 * @authors SN-Grotesque
 * 
 * 将Token填充，直到块长度的倍数
 * 
 * @note 请确保key指向的地址的已申请内存长度大于等于BlockSize，
 *       并且这个函数只能处理不含00字节的Token。
 *       此函数的填充逻辑是使用Token本身循环填充。
 * @example token = "1234567." -> Pad(12) -> "1234567.1234"
 * @param token 这是一个指针，指向需要被填充的内容的地址。
 * @param BlockSize 这是块长度，数据将被填充至这个数字的倍数。
 * @return 无
*/
WMKC_PUBLIC(wmkcVoid) wmkcPad_TokenAdd WMKC_OPEN_API
WMKC_OF((wmkcByte *token, wmkc_u32 BlockSize))
{
    wmkc_u32 tokenSize;
    wmkc_u32 tokenIndex;
    wmkc_u32 index;
    tokenSize = strlen((char *)token) & 0xffff;
    for(index = tokenSize, tokenIndex = 0; index < BlockSize; ++index, ++tokenIndex) {
        if(tokenIndex == tokenSize)
            tokenIndex = 0;
        token[index] = token[tokenIndex];
    }
}
