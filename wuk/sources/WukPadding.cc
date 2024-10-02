#include <WukPadding.hh>

void wuk::pad(wByte *src, wSize &size, wU32 blockSize, bool randVal)
{
    if(!src) {
        throw wuk::Exception(wukErr_ErrNULL, "wuk::pad", "src is NULL.");
    }
    wU32 padLen;
    wSize totalLen;
    wuk::Random random;

    padLen = (blockSize - size % blockSize);
    totalLen = padLen + size;

    if(randVal) {
        std::string tmp = random.urandom(padLen - 1);
        memcpy(src + size, tmp.c_str(), padLen - 1);
    } else {
        memset(src + size, 0xac, padLen - 1);
    }

    src[(size = totalLen) - 1] = padLen & 0xff;
}

void wuk::unpad(wByte *src, wSize &size)
{
    if(!src) {
        throw wuk::Exception(wukErr_ErrNULL, "wuk::pad", "src is NULL.");
    }
    wSize padLen;
    wSize index;

    padLen = src[size - 1];

    for(index = size - padLen; index < size; ++index)
        src[index] = 0x00;

    size = size - padLen;
}

wByte *wuk::pkcs7_pad(const wByte *src, wSize &size, wU32 blockSize)
{
    if(!src) {
        throw wuk::Exception(wukErr_ErrNULL, "wuk::pkcs7_pad", "src is NULL.");
    }
    wU32 padLen;
    wSize totalLen;

    padLen = blockSize - size % blockSize;
    totalLen = size + padLen;

    wByte *padded = new (std::nothrow) wByte[totalLen];
    memcpy(padded, src, size);
    memset(padded + size, padLen, padLen);

    size = totalLen;

    return padded;
}

wByte *wuk::pkcs7_unpad(const wByte *src, wSize &size)
{
    if(!src) {
        throw wuk::Exception(wukErr_ErrNULL, "wuk::pkcs7_unpad", "src is NULL.");
    }
    wU32 padLen = src[size - 1];
    wSize unpaddedLen = size - padLen;

    wByte *unpadded = new (std::nothrow) wByte[unpaddedLen];
    memcpy(unpadded, src, unpaddedLen);

    size = unpaddedLen;

    return unpadded;
}
