#include <snPadding.h>

SN_PUBLIC(snVoid) snPadding_add SN_OPEN_API
SN_FUNC_OF((snByte *buf, snSize *size, sn_u16 BlockSize, snBool randomPadding))
{
    snSize padoffset;
    snSize totalSize;
    snSize index;

    padoffset = snPadding_offset(BlockSize, *size);
    totalSize = padoffset + *size;

    switch(randomPadding) {
        case true:
            snSetRandomTimerSeed();
            for(index = *size; index < totalSize - 1; ++index)
                buf[index] = snRand();
            break;
        default:
            for(index = *size; index < totalSize - 1; ++index)
                buf[index] = snPadding_value;
            break;
    }

    *size = totalSize;

    buf[*size - 1] = padoffset & 0xff;
    if(BlockSize == padoffset && BlockSize == 256) {
        buf[*size - 1] = 0xff;
        buf[*size - 2] = 0xff;
    }
}

SN_PUBLIC(snVoid) snPadding_remove SN_OPEN_API
SN_FUNC_OF((snByte *buf, snSize *size))
{
    snSize padoffset;
    snSize index;

    padoffset = buf[*size - 1];

    if((buf[*size - 1] & buf[*size - 2]) == 0xff)
        padoffset = 256;

    for(index = *size - padoffset; index < *size; ++index)
        buf[index] = 0x00;

    *size = *size - padoffset;
}

SN_PUBLIC(snVoid) snPadding_keyAdd SN_OPEN_API
SN_FUNC_OF((snByte *token, sn_u16 BlockSize))
{
    sn_u16 tokenSize;
    sn_u16 tokenIndex;
    sn_u16 index;
    tokenSize = strlen((char *)token) & 0xffff;
    for(index = tokenSize, tokenIndex = 0; index < BlockSize; ++index, ++tokenIndex) {
        if(tokenIndex == tokenSize)
            tokenIndex = 0;
        token[index] = token[tokenIndex];
    }
}
