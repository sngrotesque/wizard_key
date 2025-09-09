#include <WukPadding.hh>

#include <core/WukException.hh>
#include <WukMemory.hh>
#include <WukRandom.hh>

static inline void throw_error(const void *data, wuk::ulong length, wuk::u32 bs, const char *fn)
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

static inline wuk::ulong dynamic_align(wuk::ulong x, wuk::ulong alignment)
{
    if ((x & (alignment - 1)) == 0) {
        return x;
    }
    return (x + alignment - 1) & ~(alignment - 1);
}

wuk::byte *wuk::pkcs7_pad(const wuk::byte *data, wuk::ulong &length, wuk::u32 blockSize)
{
    throw_error(data, length, blockSize, "wuk::pkcs7_pad");

    wuk::u32 padLen = blockSize - length % blockSize;
    wuk::ulong totalLen = dynamic_align(length, blockSize);

    wuk::byte *padded = wuk::m_alloc<wuk::byte *>(totalLen);
    if (!padded) {
        wuk::Exception(wuk::Error::MEMORY, "wuk::pkcs7_pad",
            "Failed to allocate memory for padded.");
    }

    memcpy(padded, data, length);
    memset(padded + length, static_cast<wuk::byte>(padLen), padLen);

    length = totalLen;
    return padded;
}

wuk::byte *wuk::pkcs7_unpad(const wuk::byte *data, wuk::ulong &length)
{
    throw_error(data, length, 0, "wuk::pkcs7_unpad");

    wuk::u32 padLen = data[length - 1];
    if (!padLen || padLen > length) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::pkcs7_unpad",
            "Incorrect padding length");
    }

    for (wuk::ulong i = length - padLen; i < length; ++i) {
        if (data[i] != padLen) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::pkcs7_unpad",
                "Invalid PKCS#7 padding");
        }
    }

    wuk::ulong unpaddedLen = length - padLen;
    wuk::byte *unpadded = wuk::m_alloc<wuk::byte *>(unpaddedLen);
    if (!unpadded) {
        wuk::Exception(wuk::Error::MEMORY, "wuk::pkcs7_pad",
            "Failed to allocate memory for padded.");
    }

    memcpy(unpadded, data, unpaddedLen);
    length = unpaddedLen;

    return unpadded;
}
