/**
 * 这只是一个用来测试SNC加密算法的测试算法，不会提供解密函数。
 * 并且也请不要将它在任何实际项目中进行使用，此算法的安全性并没有得到验证。
 * 并且这个算法的效率低下。
*/
#include <crypto/snt.h>

WMKC_PRIVATE_CONST(wmkcByte) SNT_sbox[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0x25, 0xd1, 0x84, 0xb8, 0x48, 0x35, 0x4a, 0x78, 0x79, 0x74, 0x60, 0xc7, 0x0e, 0xbc, 0x32, 0x30,
    0x91, 0x18, 0xe5, 0xdc, 0xe4, 0x90, 0x81, 0xc8, 0x65, 0x2b, 0xc3, 0x9d, 0x67, 0xa5, 0xf9, 0xdf,
    0x13, 0xf6, 0x1b, 0xd4, 0xa9, 0xb7, 0xc2, 0x5a, 0x85, 0xac, 0x41, 0x8e, 0xeb, 0x72, 0xef, 0xad,
    0x88, 0x5e, 0x1f, 0x6a, 0x52, 0x80, 0xdd, 0x16, 0xbe, 0x46, 0xf4, 0x3f, 0x94, 0x64, 0x04, 0xa8,
    0xb4, 0xc1, 0x8f, 0xee, 0x82, 0x1e, 0x8b, 0x01, 0x20, 0x11, 0xae, 0x8d, 0x17, 0x1c, 0x68, 0xab,
    0x8a, 0xfc, 0xf0, 0x50, 0xf2, 0x42, 0x0a, 0x59, 0x58, 0x6c, 0x45, 0x3e, 0x89, 0xb5, 0xa1, 0x71,
    0x73, 0x4c, 0xbd, 0xd9, 0x2f, 0x4f, 0xed, 0xdb, 0xb2, 0xaf, 0x4e, 0xff, 0x9f, 0x0b, 0x51, 0xe7,
    0x5c, 0x75, 0x53, 0x63, 0xc6, 0x6e, 0x0d, 0xf5, 0x5d, 0x47, 0x23, 0xb0, 0xda, 0xa4, 0xb6, 0x93,
    0x54, 0x19, 0x10, 0xd0, 0xfa, 0xc5, 0xd8, 0x97, 0x87, 0x3c, 0x95, 0x4b, 0xd3, 0x3d, 0xcb, 0xbb,
    0x33, 0x05, 0x0c, 0x4d, 0x08, 0x7d, 0x24, 0x5b, 0x0f, 0x3b, 0xce, 0xe3, 0xe8, 0x2e, 0xba, 0x06,
    0x57, 0xec, 0x86, 0x1d, 0x29, 0x9e, 0xa6, 0xc9, 0x31, 0x66, 0xe1, 0xfe, 0xcc, 0x7e, 0x8c, 0x1a,
    0x9a, 0x03, 0x44, 0xa7, 0xd2, 0x36, 0x2c, 0x83, 0xe2, 0x69, 0x7a, 0xe9, 0x37, 0x96, 0xb1, 0x02,
    0xea, 0x28, 0x9b, 0x21, 0xfd, 0x77, 0x3a, 0xcf, 0xf7, 0xca, 0x7b, 0x98, 0xa3, 0xc4, 0xe0, 0x6b,
    0xb9, 0xf8, 0xa2, 0x22, 0x43, 0xfb, 0xde, 0x26, 0x2d, 0xc0, 0xb3, 0xaa, 0x27, 0x7f, 0xe6, 0xbf,
    0x39, 0x9c, 0x38, 0x40, 0x09, 0x5f, 0x76, 0x34, 0xa0, 0x14, 0x49, 0x6f, 0xd5, 0xd6, 0x00, 0x12,
    0x55, 0x70, 0xf3, 0x62, 0x92, 0x56, 0x6d, 0x15, 0x2a, 0x61, 0xcd, 0x99, 0x7c, 0xf1, 0xd7, 0x07
};

// 用于将通过置换盒的数据还原的逆置换盒
WMKC_PRIVATE_CONST(wmkcByte) SNT_rsbox[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    0xee, 0x47, 0xbf, 0xb1, 0x3e, 0x91, 0x9f, 0xff, 0x94, 0xe4, 0x56, 0x6d, 0x92, 0x76, 0x0c, 0x98,
    0x82, 0x49, 0xef, 0x20, 0xe9, 0xf7, 0x37, 0x4c, 0x11, 0x81, 0xaf, 0x22, 0x4d, 0xa3, 0x45, 0x32,
    0x48, 0xc3, 0xd3, 0x7a, 0x96, 0x00, 0xd7, 0xdc, 0xc1, 0xa4, 0xf8, 0x19, 0xb6, 0xd8, 0x9d, 0x64,
    0x0f, 0xa8, 0x0e, 0x90, 0xe7, 0x05, 0xb5, 0xbc, 0xe2, 0xe0, 0xc6, 0x99, 0x89, 0x8d, 0x5b, 0x3b,
    0xe3, 0x2a, 0x55, 0xd4, 0xb2, 0x5a, 0x39, 0x79, 0x04, 0xea, 0x06, 0x8b, 0x61, 0x93, 0x6a, 0x65,
    0x53, 0x6e, 0x34, 0x72, 0x80, 0xf0, 0xf5, 0xa0, 0x58, 0x57, 0x27, 0x97, 0x70, 0x78, 0x31, 0xe5,
    0x0a, 0xf9, 0xf3, 0x73, 0x3d, 0x18, 0xa9, 0x1c, 0x4e, 0xb9, 0x33, 0xcf, 0x59, 0xf6, 0x75, 0xeb,
    0xf1, 0x5f, 0x2d, 0x60, 0x09, 0x71, 0xe6, 0xc5, 0x07, 0x08, 0xba, 0xca, 0xfc, 0x95, 0xad, 0xdd,
    0x35, 0x16, 0x44, 0xb7, 0x02, 0x28, 0xa2, 0x88, 0x30, 0x5c, 0x50, 0x46, 0xae, 0x4b, 0x2b, 0x42,
    0x15, 0x10, 0xf4, 0x7f, 0x3c, 0x8a, 0xbd, 0x87, 0xcb, 0xfb, 0xb0, 0xc2, 0xe1, 0x1b, 0xa5, 0x6c,
    0xe8, 0x5e, 0xd2, 0xcc, 0x7d, 0x1d, 0xa6, 0xb3, 0x3f, 0x24, 0xdb, 0x4f, 0x29, 0x2f, 0x4a, 0x69,
    0x7b, 0xbe, 0x68, 0xda, 0x40, 0x5d, 0x7e, 0x25, 0x03, 0xd0, 0x9e, 0x8f, 0x0d, 0x62, 0x38, 0xdf,
    0xd9, 0x41, 0x26, 0x1a, 0xcd, 0x85, 0x74, 0x0b, 0x17, 0xa7, 0xc9, 0x8e, 0xac, 0xfa, 0x9a, 0xc7,
    0x83, 0x01, 0xb4, 0x8c, 0x23, 0xec, 0xed, 0xfe, 0x86, 0x63, 0x7c, 0x67, 0x13, 0x36, 0xd6, 0x1f,
    0xce, 0xaa, 0xb8, 0x9b, 0x14, 0x12, 0xde, 0x6f, 0x9c, 0xbb, 0xc0, 0x2c, 0xa1, 0x66, 0x43, 0x2e,
    0x52, 0xfd, 0x54, 0xf2, 0x3a, 0x77, 0x21, 0xc8, 0xd1, 0x1e, 0x84, 0xd5, 0x51, 0xc4, 0xab, 0x6b
};

#define SNT_SBOX(x) (SNT_sbox[(x)])
#define SNT_RSBOX(x) (SNT_rsbox[(x)])

WMKC_PRIVATE_CONST(wmkcByte) SNT_CON64_TABLE[4][8] = {
    {0x14, 0x34, 0x95, 0xa7, 0xc6, 0x35, 0x9c, 0xb6},
    {0x58, 0x1b, 0x91, 0xf9, 0x5d, 0xf4, 0x9d, 0xd7},
    {0x97, 0x33, 0x3e, 0xa7, 0x59, 0xa9, 0x85, 0x67},
    {0xd5, 0x32, 0xad, 0x1c, 0x65, 0x3d, 0x89, 0x5e}
};

WMKC_PRIVATE_CONST(wmkcByte) SNT_CON32_TABLE[8][4] = {
    {0x1f, 0x35, 0x1e, 0x55},
    {0x3f, 0x50, 0xcb, 0xad},
    {0x52, 0x57, 0xe1, 0x91},
    {0x7c, 0x83, 0x70, 0x19},
    {0x9a, 0x9e, 0xb8, 0xd1},
    {0xa7, 0x4d, 0x8a, 0xf0},
    {0xc3, 0x8e, 0xd5, 0xba},
    {0xf6, 0x05, 0x19, 0x5e}
};

// 异或单个纵列
#define SNT_CONFUSION64(i, b0, b1, b2, b3, b4, b5, b6, b7) \
{                                                          \
    b0 ^= SNT_CON64_TABLE[i][0];                           \
    b1 ^= SNT_CON64_TABLE[i][1];                           \
    b2 ^= SNT_CON64_TABLE[i][2];                           \
    b3 ^= SNT_CON64_TABLE[i][3];                           \
    b4 ^= SNT_CON64_TABLE[i][4];                           \
    b5 ^= SNT_CON64_TABLE[i][5];                           \
    b6 ^= SNT_CON64_TABLE[i][6];                           \
    b7 ^= SNT_CON64_TABLE[i][7];                           \
}

#define SNT_MIX64(i0, i1, state)         \
{                                        \
    (*state)[0][i0] ^= (*state)[0][i1];  \
    (*state)[1][i0] ^= (*state)[1][i1];  \
    (*state)[2][i0] ^= (*state)[2][i1];  \
    (*state)[3][i0] ^= (*state)[3][i1];  \
    (*state)[4][i0] ^= (*state)[4][i1];  \
    (*state)[5][i0] ^= (*state)[5][i1];  \
    (*state)[6][i0] ^= (*state)[6][i1];  \
    (*state)[7][i0] ^= (*state)[7][i1];  \
}

// 异或单个横排
#define SNT_CONFUSION32(i, b0, b1, b2, b3) \
{                                          \
    b0 ^= SNT_CON32_TABLE[i][0];           \
    b1 ^= SNT_CON32_TABLE[i][1];           \
    b2 ^= SNT_CON32_TABLE[i][2];           \
    b3 ^= SNT_CON32_TABLE[i][3];           \
}

#define SNT_MIX32(i0, i1, state)         \
{                                        \
    (*state)[i0][0] ^= (*state)[i1][0];  \
    (*state)[i0][1] ^= (*state)[i1][1];  \
    (*state)[i0][2] ^= (*state)[i1][2];  \
    (*state)[i0][3] ^= (*state)[i1][3];  \
}

// 按照列的方式进行置换
WMKC_PRIVATE(wmkcVoid) SNT_SubBytes
WMKC_OF((SNT_State *state))
{
    wmkcFast wmkc_u32 i;
    for(i = 0; i < SNT_NK; ++i) {
        (*state)[0][i] = SNT_SBOX((*state)[0][i]);
        (*state)[1][i] = SNT_SBOX((*state)[1][i]);
        (*state)[2][i] = SNT_SBOX((*state)[2][i]);
        (*state)[3][i] = SNT_SBOX((*state)[3][i]);
        (*state)[4][i] = SNT_SBOX((*state)[4][i]);
        (*state)[5][i] = SNT_SBOX((*state)[5][i]);
        (*state)[6][i] = SNT_SBOX((*state)[6][i]);
        (*state)[7][i] = SNT_SBOX((*state)[7][i]);
    }
}

WMKC_PRIVATE(wmkcVoid) SNT_BlockConfusion
WMKC_OF((SNT_State *state))
{
    wmkcFast wmkc_u32 i;
    for(i = 0; i < SNT_NK; ++i) {
        SNT_CONFUSION64(i,
            (*state)[0][i], (*state)[1][i], (*state)[2][i], (*state)[3][i],
            (*state)[4][i], (*state)[5][i], (*state)[6][i], (*state)[7][i])
    }

    SNT_MIX64(0, 1, state);
    SNT_MIX64(1, 2, state);
    SNT_MIX64(2, 3, state);
    SNT_MIX64(3, 0, state);

    for(i = 0; i < SNT_NB; ++i) {
        SNT_CONFUSION32(i,
            (*state)[i][0], (*state)[i][1], (*state)[i][2], (*state)[i][3])
    }

    SNT_MIX32(0, 1, state);
    SNT_MIX32(1, 2, state);
    SNT_MIX32(2, 3, state);
    SNT_MIX32(3, 4, state);
    SNT_MIX32(4, 5, state);
    SNT_MIX32(5, 6, state);
    SNT_MIX32(6, 7, state);
    SNT_MIX32(7, 0, state);
}

WMKC_PRIVATE(wmkcVoid) SNT_XorWithIV WMKC_OF((SNT_State *buf, SNT_State *iv))
{
    wmkcFast wmkc_u32 i;
    for(i = 0; i < SNT_NK; ++i) {
        (*buf)[0][i] ^= (*iv)[0][i];
        (*buf)[1][i] ^= (*iv)[1][i];
        (*buf)[2][i] ^= (*iv)[2][i];
        (*buf)[3][i] ^= (*iv)[3][i];
        (*buf)[4][i] ^= (*iv)[4][i];
        (*buf)[5][i] ^= (*iv)[5][i];
        (*buf)[6][i] ^= (*iv)[6][i];
        (*buf)[7][i] ^= (*iv)[7][i];
    }
}

WMKC_PRIVATE(wmkcVoid) SNT_Cipher
WMKC_OF((SNT_mode mode, SNT_State *state, wmkcByte *roundkey))
{
    wmkcFast wmkc_u32 i;
    SNT_SubBytes(state);
    for(i = 0; i < SNT_NK; ++i) {
        (*state)[0][i] ^= *(roundkey + (SNT_NK * 0 + i));
        (*state)[1][i] ^= *(roundkey + (SNT_NK * 1 + i));
        (*state)[2][i] ^= *(roundkey + (SNT_NK * 2 + i));
        (*state)[3][i] ^= *(roundkey + (SNT_NK * 3 + i));
        (*state)[4][i] ^= *(roundkey + (SNT_NK * 4 + i));
        (*state)[5][i] ^= *(roundkey + (SNT_NK * 5 + i));
        (*state)[6][i] ^= *(roundkey + (SNT_NK * 6 + i));
        (*state)[7][i] ^= *(roundkey + (SNT_NK * 7 + i));

        if(mode == SNT_512 || mode == SNT_768) {
            (*state)[0][i] ^= *(roundkey + (SNT_NK * 0 + (i + 32)));
            (*state)[1][i] ^= *(roundkey + (SNT_NK * 1 + (i + 32)));
            (*state)[2][i] ^= *(roundkey + (SNT_NK * 2 + (i + 32)));
            (*state)[3][i] ^= *(roundkey + (SNT_NK * 3 + (i + 32)));
            (*state)[4][i] ^= *(roundkey + (SNT_NK * 4 + (i + 32)));
            (*state)[5][i] ^= *(roundkey + (SNT_NK * 5 + (i + 32)));
            (*state)[6][i] ^= *(roundkey + (SNT_NK * 6 + (i + 32)));
            (*state)[7][i] ^= *(roundkey + (SNT_NK * 7 + (i + 32)));
        }

        if(mode == SNT_768) {
            (*state)[0][i] ^= *(roundkey + (SNT_NK * 0 + (i + 64)));
            (*state)[1][i] ^= *(roundkey + (SNT_NK * 1 + (i + 64)));
            (*state)[2][i] ^= *(roundkey + (SNT_NK * 2 + (i + 64)));
            (*state)[3][i] ^= *(roundkey + (SNT_NK * 3 + (i + 64)));
            (*state)[4][i] ^= *(roundkey + (SNT_NK * 4 + (i + 64)));
            (*state)[5][i] ^= *(roundkey + (SNT_NK * 5 + (i + 64)));
            (*state)[6][i] ^= *(roundkey + (SNT_NK * 6 + (i + 64)));
            (*state)[7][i] ^= *(roundkey + (SNT_NK * 7 + (i + 64)));
        }
    }
    SNT_BlockConfusion(state);
}

WMKC_PRIVATE(wmkcVoid) SNT_keyExtension WMKC_OF((wmkc_u16 keySize, wmkcByte *iv, wmkcByte *key))
{
    static wmkcSize i;
    static wmkcByte buf;

    for(i = 0; i < keySize; ++i) {
        buf = 
            key[i]                  ^ key[(i + 1)  % keySize] ^
            key[(i + 4)  % keySize] ^ key[(i + 5)  % keySize] ^
            key[(i + 8)  % keySize] ^ key[(i + 9)  % keySize] ^
            key[(i + 12) % keySize] ^ key[(i + 13) % keySize] ^
            key[(i + 16) % keySize] ^ key[(i + 17) % keySize] ^
            key[(i + 20) % keySize] ^ key[(i + 21) % keySize] ^
            key[(i + 24) % keySize] ^ key[(i + 25) % keySize] ^
            key[(i + 28) % keySize] ^ key[(i + 29) % keySize] ^
            iv[(i + 7) % SNT_BLOCKLEN] ^ iv[(i + 16) % SNT_BLOCKLEN];

        iv[i % SNT_BLOCKLEN] ^= buf;

        buf = 
            key[(i + 2)  % keySize] ^ key[(i + 3)  % keySize] ^
            key[(i + 6)  % keySize] ^ key[(i + 7)  % keySize] ^
            key[(i + 10) % keySize] ^ key[(i + 11) % keySize] ^
            key[(i + 14) % keySize] ^ key[(i + 15) % keySize] ^
            key[(i + 18) % keySize] ^ key[(i + 19) % keySize] ^
            key[(i + 22) % keySize] ^ key[(i + 23) % keySize] ^
            key[(i + 26) % keySize] ^ key[(i + 27) % keySize] ^
            key[(i + 29) % keySize] ^ key[(i + 31) % keySize] ^
            iv[(i + 7) % SNT_BLOCKLEN] ^ iv[(i + 16) % SNT_BLOCKLEN];

        iv[(i + 17) % SNT_BLOCKLEN] ^= buf;

        key[(i + 7)  % keySize] = (buf ^ key[(i + 7)  % keySize]) ^ (keySize % (i + 1));
        key[(i + 14) % keySize] = (buf ^ key[(i + 14) % keySize]) + (keySize % (i + 1));
        key[(i + 21) % keySize] = (buf ^ key[(i + 21) % keySize]) + (keySize % (i + 1));
        key[(i + 28) % keySize] = (buf ^ key[(i + 28) % keySize]) + (keySize % (i + 1));
        key[(i + 35) % keySize] = (buf ^ key[(i + 35) % keySize]) + (keySize % (i + 1));
        key[(i + 42) % keySize] = (buf ^ key[(i + 42) % keySize]) + (keySize % (i + 1));
        key[(i + 47) % keySize] = (buf ^ key[(i + 47) % keySize]) + (keySize % (i + 1));
        key[(i + 51) % keySize] = (buf ^ key[(i + 51) % keySize]) + (keySize % (i + 1));
        key[(i + 54) % keySize] = (buf ^ key[(i + 54) % keySize]) + (keySize % (i + 1));
        key[(i + 63) % keySize] = (buf ^ key[(i + 63) % keySize]) + (keySize % (i + 1));
        key[(i + 67) % keySize] = (buf ^ key[(i + 67) % keySize]) + (keySize % (i + 1));
        key[(i + 73) % keySize] = (buf ^ key[(i + 73) % keySize]) + (keySize % (i + 1));
        key[(i + 79) % keySize] = (buf ^ key[(i + 79) % keySize]) + (keySize % (i + 1));
        key[(i + 85) % keySize] = (buf ^ key[(i + 85) % keySize]) + (keySize % (i + 1));
        key[(i + 92) % keySize] = (buf ^ key[(i + 92) % keySize]) + (keySize % (i + 1));
        key[i] = buf;
    }
}

WMKC_PUBLIC(wmkcErr_obj) SNT_new WMKC_OPEN_API
WMKC_OF((SNT_ctx **ctx, SNT_mode mode))
{
    if(!(*ctx)) {
        if(!((*ctx) = (SNT_ctx *)malloc(sizeof(SNT_ctx)))) {
            return wmkcErr_ErrMemory;
        }
    }

    (*ctx)->mode = mode;
    (*ctx)->KN = SNT_KN[mode];
    (*ctx)->NR = SNT_NR[mode];
    wmkcMem_zero((*ctx)->roundKey, 1056);

    return wmkcErr_OK;
}

WMKC_PUBLIC(wmkcErr_obj) SNT_release WMKC_OPEN_API
WMKC_OF((SNT_ctx **ctx))
{
    wmkcMem_zero((*ctx)->iv, SNT_BLOCKLEN);
    wmkcMem_zero((*ctx)->roundKey, 1056);
    free((*ctx));
    (*ctx) = wmkcNull;

    return wmkcErr_OK;
}

WMKC_PUBLIC(wmkcVoid) SNT_init WMKC_OPEN_API
WMKC_OF((SNT_ctx *ctx, wmkcByte *keyBuf, wmkcByte *ivBuf))
{
    wmkcByte *key = (wmkcByte *)malloc(ctx->KN);
    wmkcByte iv[SNT_BLOCKLEN];
    wmkc_u32 r;

    memcpy(ctx->iv, ivBuf, SNT_BLOCKLEN);
    memcpy(key, keyBuf, ctx->KN);
    memcpy(iv, ivBuf, SNT_BLOCKLEN);

    for(r = 0; r < ctx->NR; ++r) {
        memcpy(ctx->roundKey + (ctx->KN * r), key, ctx->KN);
        SNT_keyExtension(ctx->KN, iv, key);
        SNT_XorWithIV((SNT_State *)iv, (SNT_State *)key);
        SNT_keyExtension(ctx->KN, iv, key);
        if(ctx->mode == SNT_512 || ctx->mode == SNT_768) {
            SNT_XorWithIV((SNT_State *)(key + 32), (SNT_State *)iv);
        }
        if(ctx->mode == SNT_768) {
            SNT_XorWithIV((SNT_State *)(key + 64), (SNT_State *)iv);
        }
        SNT_Cipher(ctx->mode, (SNT_State *)iv, key);
    }

    wmkcMem_zero(key, ctx->KN);
    wmkcMem_zero(iv, SNT_BLOCKLEN);
    free(key);
    key = wmkcNull;
}

WMKC_PUBLIC(wmkcVoid) SNT_CBC_Encrypt WMKC_OPEN_API
WMKC_OF((SNT_ctx *ctx, wmkcByte *buf, wmkcSize size))
{
    wmkcFast wmkcSize r, i;
    static wmkcByte round_iv[SNT_BLOCKLEN];

    SNT_State *bufState = (SNT_State *)buf;
    SNT_State *ivState = (SNT_State *)round_iv;
    size /= SNT_BLOCKLEN;

    for(r = 0; r < ctx->NR; ++r) {
        memcpy(round_iv, ctx->iv, SNT_BLOCKLEN);
        for(i = 0; i < size; ++i) {
            SNT_XorWithIV(bufState + i, ivState);
            SNT_Cipher(ctx->mode, bufState + i, ctx->roundKey + (ctx->KN * r));
            memcpy(ivState, bufState + i, SNT_BLOCKLEN);
        }
    }
}

