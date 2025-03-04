#include <WukPadding.hh>

void throw_nullptr(const char *function)
{
    throw wuk::Exception(wuk::Error::NPTR, function,
            "data is nullptr.");
}

void wuk::pad(wByte *data, wSize &length, wU32 blockSize)
{
    if(!data) {
        throw_nullptr("wuk::pad");
    }
    wU32 padLen;
    wSize totalLen;
    wuk::Random random;

    padLen = (blockSize - length % blockSize);
    totalLen = padLen + length;

    memset(data + length, 0xac, padLen - 1);

    data[(length = totalLen) - 1] = padLen & 0xff;
}

void wuk::unpad(wByte *data, wSize &length)
{
    if(!data) {
        throw_nullptr("wuk::unpad");
    }
    wSize padLen;
    wSize index;

    padLen = data[length - 1];

    for(index = length - padLen; index < length; ++index)
        data[index] = 0x00;

    length = length - padLen;
}

wByte *wuk::pkcs7_pad(const wByte *data, wSize &length, wU32 blockSize)
{
    if(!data) {
        throw_nullptr("wuk::pkcs7_pad");
    }
    wU32 padLen;
    wSize totalLen;

    padLen = blockSize - length % blockSize;
    totalLen = length + padLen;

    wByte *padded = wuk::m_alloc<wByte *>(totalLen);
    memcpy(padded, data, length);
    memset(padded + length, padLen, padLen);

    length = totalLen;

    return padded;
}

wByte *wuk::pkcs7_unpad(const wByte *data, wSize &length)
{
    if(!data) {
        throw_nullptr("wuk::pkcs7_unpad");
    }
    wU32 padLen = data[length - 1];
    wSize unpaddedLen = length - padLen;

    wByte *unpadded = wuk::m_alloc<wByte *>(unpaddedLen);
    memcpy(unpadded, data, unpaddedLen);

    length = unpaddedLen;

    return unpadded;
}

wByte *wuk::x923_pad(const wByte *data, wSize &length, wU32 blockSize)
{
    if (!data) {
        throw_nullptr("wuk::x923_pad");
    }

    wU32 padLen = blockSize - length % blockSize;
    wSize totalLen = length + padLen;

    wByte *padded = wuk::m_alloc<wByte *>(totalLen);
    memcpy(padded, data, length);
    memset(padded + length, 0x00, padLen);
    padded[totalLen - 1] = padLen;

    length = totalLen;

    return padded;
}

wByte *wuk::x923_unpad(const wByte *data, wSize &length)
{
    if (!data) {
        throw_nullptr("wuk::x923_unpad");
    }

    wU32 padLen = data[length - 1];
    wSize unpaddedLen = length - padLen;

    wByte *unpadded = wuk::m_alloc<wByte *>(unpaddedLen);
    memcpy(unpadded, data, unpaddedLen);

    length = unpaddedLen;

    return unpadded;
}
