#include <WukPadding.hh>

#include <config/WukException.hh>
#include <WukMemory.hh>
#include <WukRandom.hh>

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

static inline wSize dynamic_align(wSize x, wSize alignment)
{
    if ((x & (alignment - 1)) == 0) {
        return x;
    }
    return (x + alignment - 1) & ~(alignment - 1);
}

wByte *wuk::pkcs7_pad(const wByte *data, wSize &length, wU32 blockSize)
{
    throw_error(data, length, blockSize, "wuk::pkcs7_pad");

    wU32 padLen = blockSize - length % blockSize;
    wSize totalLen = dynamic_align(length, blockSize);

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
