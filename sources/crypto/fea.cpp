#include <crypto/fea.hpp>

static const wByte sbox[256] = {
    // 0     1     2     3     4     5     6     7     8     9     a     n     c     d     e     f
    0x2b, 0x1b, 0xf5, 0xd5, 0x6c, 0x78, 0xe3, 0xef, 0xce, 0x69, 0xb6, 0xda, 0x28, 0x16, 0xc2, 0xbc,
    0x56, 0xe6, 0x65, 0x48, 0x6b, 0xdd, 0xa9, 0x01, 0xcb, 0x94, 0x76, 0xcf, 0x20, 0xa1, 0x19, 0x91,
    0xb1, 0xc0, 0xe7, 0x86, 0x27, 0x2e, 0xa6, 0x36, 0x95, 0xba, 0xc8, 0x0a, 0x66, 0x1f, 0x8b, 0x09,
    0x31, 0x79, 0x5b, 0xd7, 0x87, 0x81, 0x15, 0x82, 0x5f, 0x18, 0x06, 0xc1, 0xf2, 0x44, 0x9e, 0xb5,
    0x7f, 0x74, 0x0f, 0x6a, 0x52, 0x49, 0x9b, 0x70, 0x75, 0xa7, 0xbd, 0xa0, 0x0b, 0xe4, 0x32, 0xfe,
    0x35, 0x4a, 0x1a, 0x29, 0x30, 0xa5, 0x68, 0x3c, 0x4f, 0xae, 0x8c, 0x25, 0xab, 0x6d, 0xc6, 0xf7,
    0x96, 0x72, 0xd1, 0x14, 0xd8, 0x05, 0x22, 0xfc, 0x41, 0x34, 0xd6, 0xe5, 0xca, 0x55, 0x3b, 0xac,
    0xb9, 0x03, 0x11, 0x3d, 0x7d, 0xd9, 0x37, 0x7a, 0x10, 0x9a, 0xa8, 0x93, 0xdc, 0x4c, 0xe9, 0xea,
    0xfd, 0x00, 0xd4, 0xd3, 0xf9, 0x77, 0xbb, 0xeb, 0x85, 0x97, 0xbf, 0x2a, 0x53, 0x4e, 0x73, 0xec,
    0xde, 0x33, 0x57, 0x1c, 0xa3, 0x83, 0x51, 0x26, 0xa4, 0x04, 0x07, 0x5d, 0x47, 0x9c, 0x71, 0x7e,
    0x8d, 0x50, 0xcc, 0x54, 0x08, 0x98, 0x9f, 0x0d, 0xf3, 0x02, 0x7b, 0x88, 0x46, 0xb7, 0xb4, 0x9d,
    0xf1, 0x8a, 0x2d, 0x40, 0x4d, 0x5e, 0xc4, 0x12, 0xdb, 0x23, 0x6f, 0x99, 0x80, 0x64, 0x0e, 0x62,
    0x1e, 0xb2, 0x3e, 0x17, 0x3f, 0xe0, 0x39, 0xcd, 0xd2, 0xc5, 0xe2, 0x42, 0x5c, 0x89, 0x2c, 0x3a,
    0x58, 0x4b, 0xaf, 0xdf, 0xad, 0x59, 0xe8, 0x92, 0xe1, 0xa2, 0x1d, 0xaa, 0xc3, 0xf4, 0x60, 0x0c,
    0x5a, 0xed, 0x63, 0xc7, 0x38, 0x21, 0xbe, 0x61, 0x84, 0x67, 0xb8, 0xb0, 0xfb, 0x2f, 0xff, 0xb3,
    0x13, 0xfa, 0x8e, 0x45, 0x24, 0xf8, 0x6e, 0xee, 0xf0, 0x7c, 0x90, 0xd0, 0xf6, 0x43, 0x8f, 0xc9
};

static const wByte rsbox[256] = {
    // 0     1     2     3     4     5     6     7     8     9     a     n     c     d     e     f
    0x81, 0x17, 0xa9, 0x71, 0x99, 0x65, 0x3a, 0x9a, 0xa4, 0x2f, 0x2b, 0x4c, 0xdf, 0xa7, 0xbe, 0x42,
    0x78, 0x72, 0xb7, 0xf0, 0x63, 0x36, 0x0d, 0xc3, 0x39, 0x1e, 0x52, 0x01, 0x93, 0xda, 0xc0, 0x2d,
    0x1c, 0xe5, 0x66, 0xb9, 0xf4, 0x5b, 0x97, 0x24, 0x0c, 0x53, 0x8b, 0x00, 0xce, 0xb2, 0x25, 0xed,
    0x54, 0x30, 0x4e, 0x91, 0x69, 0x50, 0x27, 0x76, 0xe4, 0xc6, 0xcf, 0x6e, 0x57, 0x73, 0xc2, 0xc4,
    0xb3, 0x68, 0xcb, 0xfd, 0x3d, 0xf3, 0xac, 0x9c, 0x13, 0x45, 0x51, 0xd1, 0x7d, 0xb4, 0x8d, 0x58,
    0xa1, 0x96, 0x44, 0x8c, 0xa3, 0x6d, 0x10, 0x92, 0xd0, 0xd5, 0xe0, 0x32, 0xcc, 0x9b, 0xb5, 0x38,
    0xde, 0xe7, 0xbf, 0xe2, 0xbd, 0x12, 0x2c, 0xe9, 0x56, 0x09, 0x43, 0x14, 0x04, 0x5d, 0xf6, 0xba,
    0x47, 0x9e, 0x61, 0x8e, 0x41, 0x48, 0x1a, 0x85, 0x05, 0x31, 0x77, 0xaa, 0xf9, 0x74, 0x9f, 0x40,
    0xbc, 0x35, 0x37, 0x95, 0xe8, 0x88, 0x23, 0x34, 0xab, 0xcd, 0xb1, 0x2e, 0x5a, 0xa0, 0xf2, 0xfe,
    0xfa, 0x1f, 0xd7, 0x7b, 0x19, 0x28, 0x60, 0x89, 0xa5, 0xbb, 0x79, 0x46, 0x9d, 0xaf, 0x3e, 0xa6,
    0x4b, 0x1d, 0xd9, 0x94, 0x98, 0x55, 0x26, 0x49, 0x7a, 0x16, 0xdb, 0x5c, 0x6f, 0xd4, 0x59, 0xd2,
    0xeb, 0x20, 0xc1, 0xef, 0xae, 0x3f, 0x0a, 0xad, 0xea, 0x70, 0x29, 0x86, 0x0f, 0x4a, 0xe6, 0x8a,
    0x21, 0x3b, 0x0e, 0xdc, 0xb6, 0xc9, 0x5e, 0xe3, 0x2a, 0xff, 0x6c, 0x18, 0xa2, 0xc7, 0x08, 0x1b,
    0xfb, 0x62, 0xc8, 0x83, 0x82, 0x03, 0x6a, 0x33, 0x64, 0x75, 0x0b, 0xb8, 0x7c, 0x15, 0x90, 0xd3,
    0xc5, 0xd8, 0xca, 0x06, 0x4d, 0x6b, 0x11, 0x22, 0xd6, 0x7e, 0x7f, 0x87, 0x8f, 0xe1, 0xf7, 0x07,
    0xf8, 0xb0, 0x3c, 0xa8, 0xdd, 0x02, 0xfc, 0x5f, 0xf5, 0x84, 0xf1, 0xec, 0x67, 0x80, 0x4f, 0xee
};

// 按位循环左移3位
#define WMKC_FEA_SHIFT_BITS_L(x) (((x >> 5) | (x << 3)) & 0xff)
// 按位循环右移3位
#define WMKC_FEA_SHIFT_BITS_R(x) (((x << 5) | (x >> 3)) & 0xff)

#define WMKC_FEA_SBOX(x)  (sbox[(x)])
#define WMKC_FEA_RSBOX(x) (rsbox[(x)])

wVoid wmkc::crypto::fea::subBytes(wByte *block)
{
    for(wU32 i = 0; i < WMKC_FEA_BL; i += 8) {
        *(block + i)     = WMKC_FEA_SBOX(*(block + i));
        *(block + i + 1) = WMKC_FEA_SBOX(*(block + i + 1));
        *(block + i + 2) = WMKC_FEA_SBOX(*(block + i + 2));
        *(block + i + 3) = WMKC_FEA_SBOX(*(block + i + 3));
        *(block + i + 4) = WMKC_FEA_SBOX(*(block + i + 4));
        *(block + i + 5) = WMKC_FEA_SBOX(*(block + i + 5));
        *(block + i + 6) = WMKC_FEA_SBOX(*(block + i + 6));
        *(block + i + 7) = WMKC_FEA_SBOX(*(block + i + 7));
    }
}

wVoid wmkc::crypto::fea::invSubBytes(wByte *block)
{
    for(wU32 i = 0; i < WMKC_FEA_BL; i += 8) {
        *(block + i)     = WMKC_FEA_RSBOX(*(block + i));
        *(block + i + 1) = WMKC_FEA_RSBOX(*(block + i + 1));
        *(block + i + 2) = WMKC_FEA_RSBOX(*(block + i + 2));
        *(block + i + 3) = WMKC_FEA_RSBOX(*(block + i + 3));
        *(block + i + 4) = WMKC_FEA_RSBOX(*(block + i + 4));
        *(block + i + 5) = WMKC_FEA_RSBOX(*(block + i + 5));
        *(block + i + 6) = WMKC_FEA_RSBOX(*(block + i + 6));
        *(block + i + 7) = WMKC_FEA_RSBOX(*(block + i + 7));
    }
}

wVoid wmkc::crypto::fea::shiftBits(wByte *block)
{
    for(wU32 i = 0; i < WMKC_FEA_BL; i += 8) {
        *(block + i)     = WMKC_FEA_SHIFT_BITS_L(*(block + i));
        *(block + i + 1) = WMKC_FEA_SHIFT_BITS_L(*(block + i + 1));
        *(block + i + 2) = WMKC_FEA_SHIFT_BITS_L(*(block + i + 2));
        *(block + i + 3) = WMKC_FEA_SHIFT_BITS_L(*(block + i + 3));
        *(block + i + 4) = WMKC_FEA_SHIFT_BITS_L(*(block + i + 4));
        *(block + i + 5) = WMKC_FEA_SHIFT_BITS_L(*(block + i + 5));
        *(block + i + 6) = WMKC_FEA_SHIFT_BITS_L(*(block + i + 6));
        *(block + i + 7) = WMKC_FEA_SHIFT_BITS_L(*(block + i + 7));
    }
}

wVoid wmkc::crypto::fea::invShiftBits(wByte *block)
{
    for(wU32 i = 0; i < WMKC_FEA_BL; i += 8) {
        *(block + i)     = WMKC_FEA_SHIFT_BITS_R(*(block + i));
        *(block + i + 1) = WMKC_FEA_SHIFT_BITS_R(*(block + i + 1));
        *(block + i + 2) = WMKC_FEA_SHIFT_BITS_R(*(block + i + 2));
        *(block + i + 3) = WMKC_FEA_SHIFT_BITS_R(*(block + i + 3));
        *(block + i + 4) = WMKC_FEA_SHIFT_BITS_R(*(block + i + 4));
        *(block + i + 5) = WMKC_FEA_SHIFT_BITS_R(*(block + i + 5));
        *(block + i + 6) = WMKC_FEA_SHIFT_BITS_R(*(block + i + 6));
        *(block + i + 7) = WMKC_FEA_SHIFT_BITS_R(*(block + i + 7));
    }
}

wVoid wmkc::crypto::fea::shiftRows(wByte *block)
{
    wByte swap_array[8], swap;

    memcpy(swap_array, block, 8);
    memcpy(block, block + 8, 8);
    memcpy(block + 8, swap_array, 8);

    swap = (*(block + 8)  ^ *(block + 9)  ^ *(block + 10) ^ *(block + 11) ^
            *(block + 12) ^ *(block + 13) ^ *(block + 14) ^ *(block + 15));

    *(block + 0) ^= swap;
    *(block + 1) ^= swap;
    *(block + 2) ^= swap;
    *(block + 3) ^= swap;
    *(block + 4) ^= swap;
    *(block + 5) ^= swap;
    *(block + 6) ^= swap;
    *(block + 7) ^= swap;
}

wVoid wmkc::crypto::fea::invShiftRows(wByte *block)
{
    wByte swap_array[8], swap;

    swap = (*(block + 8)  ^ *(block + 9)  ^ *(block + 10) ^ *(block + 11) ^
            *(block + 12) ^ *(block + 13) ^ *(block + 14) ^ *(block + 15));

    *(block + 0) ^= swap;
    *(block + 1) ^= swap;
    *(block + 2) ^= swap;
    *(block + 3) ^= swap;
    *(block + 4) ^= swap;
    *(block + 5) ^= swap;
    *(block + 6) ^= swap;
    *(block + 7) ^= swap;

    memcpy(swap_array, block, 8);
    memcpy(block, block + 8, 8);
    memcpy(block + 8, swap_array, 8);
}

wVoid wmkc::crypto::fea::xorWithIV(wByte *block, wByte *iv)
{
    for(wU32 i = 0; i < WMKC_FEA_BL; i += 8) {
        *(block + i)     ^= *(iv + i);
        *(block + i + 1) ^= *(iv + i + 1);
        *(block + i + 2) ^= *(iv + i + 2);
        *(block + i + 3) ^= *(iv + i + 3);
        *(block + i + 4) ^= *(iv + i + 4);
        *(block + i + 5) ^= *(iv + i + 5);
        *(block + i + 6) ^= *(iv + i + 6);
        *(block + i + 7) ^= *(iv + i + 7);
    }
}

wVoid wmkc::crypto::fea::keyExtension(wByte *key, wByte *iv)
{
    wByte keyBuffer[sizeof(this->key)];
    wByte ivBuffer[sizeof(this->iv)];
    wU32 rkIndex, index;

    memcpy(keyBuffer, key, sizeof(this->key));
    memcpy(ivBuffer, iv, sizeof(this->iv));

    for(rkIndex = 0; rkIndex < sizeof(this->roundKey); rkIndex += sizeof(this->key)) {
        memcpy(this->roundKey + rkIndex, keyBuffer, sizeof(this->key));

        this->subBytes(keyBuffer);
        this->subBytes(keyBuffer + WMKC_FEA_BL);

        this->xorWithIV(keyBuffer, ivBuffer);
        this->xorWithIV(keyBuffer + WMKC_FEA_BL, ivBuffer);

        for(index = 0; index < WMKC_FEA_BL; ++index) {
            ivBuffer[index] ^= (keyBuffer[index] ^ keyBuffer[index + 16]);
        }

        for(index = 0; index < sizeof(this->key); ++index) {
            keyBuffer[index] ^= \
                ivBuffer[0] ^ ivBuffer[2]  ^ ivBuffer[4]  ^ ivBuffer[6] ^
                ivBuffer[8] ^ ivBuffer[10] ^ ivBuffer[12] ^ ivBuffer[14];
            keyBuffer[index] ^= \
                ivBuffer[1] ^ ivBuffer[3]  ^ ivBuffer[5]  ^ ivBuffer[7] ^
                ivBuffer[9] ^ ivBuffer[11] ^ ivBuffer[13] ^ ivBuffer[15];
            keyBuffer[index] ^= (ivBuffer[index & 15] + index);
        }

        this->subBytes(ivBuffer);
        this->shiftBits(ivBuffer);

        for(index = 0; index < sizeof(this->key); ++index) {
            keyBuffer[index] ^= \
                ivBuffer[0] ^ ivBuffer[2]  ^ ivBuffer[4]  ^ ivBuffer[6] ^
                ivBuffer[8] ^ ivBuffer[10] ^ ivBuffer[12] ^ ivBuffer[14];
            keyBuffer[index] ^= \
                ivBuffer[1] ^ ivBuffer[3]  ^ ivBuffer[5]  ^ ivBuffer[7] ^
                ivBuffer[9] ^ ivBuffer[11] ^ ivBuffer[13] ^ ivBuffer[15];
        }

        this->shiftBits(keyBuffer);
        this->subBytes(keyBuffer);
        this->shiftBits(keyBuffer + WMKC_FEA_BL);
        this->subBytes(keyBuffer + WMKC_FEA_BL);
    }

    wmkc::memory_secure(keyBuffer, sizeof(this->key));
    wmkc::memory_secure(ivBuffer, sizeof(this->iv));
}

wVoid wmkc::crypto::fea::cipher(wByte *p, wByte *roundKey)
{
    wU32 r, i;
    wByte *subkey = nullptr;
    for(r = 0; r < WMKC_FEA_NR; ++r) {
        this->subBytes(p);
        subkey = roundKey + (r << 5); // roundKey + r * 32
        for(i = 0; i < (WMKC_FEA_BL << 1); i += 8) {
            *(p + (i       & 15)) ^= *(subkey + i);
            *(p + ((i + 1) & 15)) ^= *(subkey + i + 1);
            *(p + ((i + 2) & 15)) ^= *(subkey + i + 2);
            *(p + ((i + 3) & 15)) ^= *(subkey + i + 3);
            *(p + ((i + 4) & 15)) ^= *(subkey + i + 4);
            *(p + ((i + 5) & 15)) ^= *(subkey + i + 5);
            *(p + ((i + 6) & 15)) ^= *(subkey + i + 6);
            *(p + ((i + 7) & 15)) ^= *(subkey + i + 7);
        }
        this->shiftRows(p);
        this->shiftBits(p);
    }
}

wVoid wmkc::crypto::fea::invCipher(wByte *c, wByte *roundKey)
{
    wU32 r, i;
    wByte *subkey = nullptr;
    for(r = 0; r < WMKC_FEA_NR; ++r) {
        this->invShiftBits(c);
        this->invShiftRows(c);
        subkey = roundKey + ((WMKC_FEA_NR - r - 1) << 5); // roundKey + r * 32
        for(i = 0; i < (WMKC_FEA_BL << 1); i += 8) {
            *(c + (i       & 15)) ^= *(subkey + i);
            *(c + ((i + 1) & 15)) ^= *(subkey + (i + 1));
            *(c + ((i + 2) & 15)) ^= *(subkey + (i + 2));
            *(c + ((i + 3) & 15)) ^= *(subkey + (i + 3));
            *(c + ((i + 4) & 15)) ^= *(subkey + (i + 4));
            *(c + ((i + 5) & 15)) ^= *(subkey + (i + 5));
            *(c + ((i + 6) & 15)) ^= *(subkey + (i + 6));
            *(c + ((i + 7) & 15)) ^= *(subkey + (i + 7));
        }
        this->invSubBytes(c);
    }
}
//////////////////////////////////////////////////////////

wmkc::crypto::fea::fea(const wByte *key, const wByte *iv, const wU32 segmentSize)
: key(), iv(), nonce(), roundKey(), segmentSize(segmentSize)
{
    if(!key || !iv) {
        wmkc::exception(wmkcErr_ErrNULL, "wmkc::crypto::fea::fea",
                                        "key or iv is NULL.");
    }

    memcpy(this->key, key, sizeof(this->key));
    memcpy(this->iv, iv, sizeof(this->iv));

    this->keyExtension(this->key, this->iv);
}

wmkc::crypto::fea::~fea()
{
    wmkc::memory_secure(this->key, sizeof(this->key));
    wmkc::memory_secure(this->iv, sizeof(this->iv));
}

void wmkc::crypto::fea::encrypt(wByte *content, wSize size, xcryptMode mode)
{
    switch(mode) {
        case xcryptMode::ECB:
            this->cipher(content, this->roundKey); break;
        case xcryptMode::CBC:
            this->cbc_encrypt(content, size); break;
        case xcryptMode::CTR:
            this->ctr_xcrypt(content, size); break;
        case xcryptMode::CFB:
            this->cfb_encrypt(content, size, this->segmentSize); break;
    }
}

void wmkc::crypto::fea::decrypt(wByte *content, wSize size, xcryptMode mode)
{
    switch(mode) {
        case xcryptMode::ECB:
            this->invCipher(content, this->roundKey); break;
        case xcryptMode::CBC:
            this->cbc_decrypt(content, size); break;
        case xcryptMode::CTR:
            this->ctr_xcrypt(content, size); break;
        case xcryptMode::CFB:
            this->cfb_decrypt(content, size, this->segmentSize); break;
    }
}


