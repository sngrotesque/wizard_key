#include <snPadding.h>

WMKC_PUBLIC(wmkcVoid) snPadding_add WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize *size, wmkc_u16 BlockSize, wmkcBool randomPadding))
{
    wmkcSize padoffset;
    wmkcSize totalSize;
    wmkcSize index;

    padoffset = snPadding_offset(BlockSize, *size);
    totalSize = padoffset + *size;

    switch(randomPadding) {
        case true:
            snRand_Seed();
            for(index = *size; index < totalSize - 1; ++index)
                buf[index] = snRand_Rand();
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

WMKC_PUBLIC(wmkcVoid) snPadding_remove WMKC_OPEN_API
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

WMKC_PUBLIC(wmkcVoid) snPadding_keyAdd WMKC_OPEN_API
WMKC_OF((wmkcByte *token, wmkc_u16 BlockSize))
{
    wmkc_u16 tokenSize;
    wmkc_u16 tokenIndex;
    wmkc_u16 index;
    tokenSize = strlen((char *)token) & 0xffff;
    for(index = tokenSize, tokenIndex = 0; index < BlockSize; ++index, ++tokenIndex) {
        if(tokenIndex == tokenSize)
            tokenIndex = 0;
        token[index] = token[tokenIndex];
    }
}
