#include <WukPadding.hh>

static inline void throw_error(const void *data, wSize length, wU32 bs, const char *fn)
{
    if (!data || !length) {
        throw wuk::Exception(wuk::Error::NPTR, fn,
            "The data is a null pointer or has a length of 0.");
    }
    if (bs && (bs > 256)) {
        throw wuk::Exception(wuk::Error::ERR, fn,
            "The padding length exceeds the standard length.");
    }
}

uint32_t dynamic_align(uint32_t value, uint32_t alignment) {
    // 检查对齐值合法性
    if (alignment == 0 || (alignment & (alignment - 1)) != 0 || 
        (alignment % 4 != 0)) {
        throw std::invalid_argument("Alignment must be power of 2 and multiple of 4");
    }

    // 处理0值（0对齐任何数都是0）
    if (value == 0) return 0;

    // 检查是否已经对齐
    if ((value & (alignment - 1)) == 0) {
        return value;
    }

    // 防溢出检查
    if (value > UINT32_MAX - (alignment - 1)) {
        throw std::overflow_error("Alignment would cause overflow");
    }

    // 计算对齐后的值（核心算法）
    return (value + alignment - 1) & ~(alignment - 1);
}

void wuk::pad(wByte *data, wSize &length, wU32 blockSize)
{
    throw_error(data, length, blockSize, "wuk::pad");

    wuk::Random random;

    wU32 padLen = (blockSize - length % blockSize);
    wSize totalLen = padLen + length;

    memset(data + length, 0xac, padLen - 1);

    data[(length = totalLen) - 1] = padLen & 0xff;
}

void wuk::unpad(wByte *data, wSize &length)
{
    throw_error(data, length, 0, "wuk::unpad");

    wSize padLen;
    wSize index;

    padLen = data[length - 1];

    for(index = length - padLen; index < length; ++index)
        data[index] = 0x00;

    length = length - padLen;
}

wByte *wuk::pkcs7_pad(const wByte *data, wSize &length, wU32 blockSize)
{
    throw_error(data, length, blockSize, "wuk::pkcs7_pad");

    wU32 padLen = blockSize - length % blockSize;
    wSize totalLen = length + padLen;

    wByte *padded = wuk::m_alloc<wByte *>(totalLen);
    if (!padded) {
        wuk::Exception(wuk::Error::MEMORY, "wuk::pkcs7_pad",
            "Failed to allocate memory for padded.");
    }

    memcpy(padded, data, length);
    memset(padded + length, static_cast<wByte>(padLen), padLen);

    length = totalLen;
    return padded;
}

wByte *wuk::pkcs7_unpad(const wByte *data, wSize &length)
{
    throw_error(data, length, 0, "wuk::pkcs7_unpad");

    wU32 padLen = data[length - 1];
    if (!padLen || padLen > length) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::pkcs7_unpad",
            "Incorrect padding length");
    }

    for (wSize i = length - padLen; i < length; ++i) {
        if (data[i] != padLen) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::pkcs7_unpad",
                "Invalid PKCS#7 padding");
        }
    }

    wSize unpaddedLen = length - padLen;
    wByte *unpadded = wuk::m_alloc<wByte *>(unpaddedLen);
    if (!unpadded) {
        wuk::Exception(wuk::Error::MEMORY, "wuk::pkcs7_pad",
            "Failed to allocate memory for padded.");
    }

    memcpy(unpadded, data, unpaddedLen);
    length = unpaddedLen;

    return unpadded;
}
