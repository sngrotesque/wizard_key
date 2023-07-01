/*
* 警告！！！（Warning!!!!!）
* 这是一个过时的版本，请不要使用它。
*/
#include <crypto/snc_412.h>

WMKC_PRIVATE_CONST(wmkcByte) SNC_sbox[256] = {
    0xd2, 0xa7, 0xc9, 0xe0, 0x93, 0xc7, 0xb6, 0x43, 0x42, 0x1b, 0x89, 0x79, 0x12, 0xf1, 0x39, 0xd0,
    0xa0, 0x3f, 0xcf, 0x8d, 0xdc, 0x1f, 0xac, 0xe4, 0x01, 0xf7, 0x69, 0xa5, 0xf4, 0xb2, 0xf3, 0x1e,
    0x72, 0x8e, 0x28, 0x47, 0xb4, 0x86, 0x00, 0x7f, 0xf6, 0x1d, 0x51, 0xc1, 0x16, 0xa9, 0xf5, 0xc8,
    0x6c, 0x1c, 0xbb, 0x2c, 0xce, 0x4d, 0xfa, 0xba, 0xc6, 0x04, 0x06, 0x0d, 0x99, 0x40, 0x5e, 0xe1,
    0x27, 0x96, 0x5f, 0x6d, 0xdf, 0x64, 0xbe, 0x2b, 0x0a, 0x74, 0x3a, 0xf8, 0x77, 0x9e, 0x83, 0x81,
    0xee, 0x48, 0x60, 0x4b, 0xd3, 0x23, 0x6a, 0x5b, 0x8c, 0x78, 0xbf, 0xfb, 0xa3, 0x56, 0x37, 0x20,
    0x38, 0xd1, 0x17, 0xb1, 0xe7, 0x36, 0x75, 0x31, 0xcd, 0x09, 0x84, 0x55, 0x7d, 0x4a, 0x22, 0x41,
    0x70, 0x6e, 0x9c, 0xdd, 0x34, 0x18, 0xbc, 0xa8, 0xea, 0xe8, 0x87, 0x9a, 0x3d, 0x59, 0x45, 0x2e,
    0x44, 0x7a, 0x73, 0xf9, 0x07, 0x65, 0xfc, 0xa1, 0xb7, 0x95, 0x9b, 0xd8, 0x21, 0x50, 0x94, 0x4f,
    0xa2, 0x46, 0xf2, 0xdb, 0x05, 0xff, 0x91, 0xd7, 0x3b, 0xa4, 0x30, 0x13, 0xd4, 0x68, 0xb8, 0x26,
    0xd6, 0x15, 0x2a, 0x0c, 0x76, 0x6f, 0x66, 0xc0, 0x4c, 0xf0, 0x02, 0x8a, 0x88, 0xb0, 0x5a, 0x8f,
    0xc5, 0xca, 0xab, 0xfe, 0xcc, 0x7e, 0x2f, 0xaf, 0xfd, 0xaa, 0x11, 0xc3, 0x49, 0x62, 0x97, 0xeb,
    0xe6, 0xae, 0x35, 0x0b, 0x67, 0x57, 0x52, 0xcb, 0x33, 0x85, 0x19, 0xc4, 0x98, 0xc2, 0xec, 0x6b,
    0xda, 0x7c, 0x80, 0x25, 0xa6, 0xd5, 0x1a, 0x63, 0xd9, 0x7b, 0xe5, 0x2d, 0xe2, 0x53, 0x24, 0x9d,
    0x71, 0xb3, 0x29, 0x58, 0x54, 0xef, 0x3c, 0x8b, 0xed, 0x3e, 0xad, 0x03, 0x61, 0x5c, 0x9f, 0xe3,
    0x0e, 0xbd, 0x08, 0xde, 0xb5, 0x82, 0x32, 0x14, 0x10, 0x5d, 0x90, 0x4e, 0x92, 0xe9, 0x0f, 0xb9
};

WMKC_PRIVATE_CONST(wmkcByte) SNC_rsbox[256] = {
    0x26, 0x18, 0xaa, 0xeb, 0x39, 0x94, 0x3a, 0x84, 0xf2, 0x69, 0x48, 0xc3, 0xa3, 0x3b, 0xf0, 0xfe,
    0xf8, 0xba, 0x0c, 0x9b, 0xf7, 0xa1, 0x2c, 0x62, 0x75, 0xca, 0xd6, 0x09, 0x31, 0x29, 0x1f, 0x15,
    0x5f, 0x8c, 0x6e, 0x55, 0xde, 0xd3, 0x9f, 0x40, 0x22, 0xe2, 0xa2, 0x47, 0x33, 0xdb, 0x7f, 0xb6,
    0x9a, 0x67, 0xf6, 0xc8, 0x74, 0xc2, 0x65, 0x5e, 0x60, 0x0e, 0x4a, 0x98, 0xe6, 0x7c, 0xe9, 0x11,
    0x3d, 0x6f, 0x08, 0x07, 0x80, 0x7e, 0x91, 0x23, 0x51, 0xbc, 0x6d, 0x53, 0xa8, 0x35, 0xfb, 0x8f,
    0x8d, 0x2a, 0xc6, 0xdd, 0xe4, 0x6b, 0x5d, 0xc5, 0xe3, 0x7d, 0xae, 0x57, 0xed, 0xf9, 0x3e, 0x42,
    0x52, 0xec, 0xbd, 0xd7, 0x45, 0x85, 0xa6, 0xc4, 0x9d, 0x1a, 0x56, 0xcf, 0x30, 0x43, 0x71, 0xa5,
    0x70, 0xe0, 0x20, 0x82, 0x49, 0x66, 0xa4, 0x4c, 0x59, 0x0b, 0x81, 0xd9, 0xd1, 0x6c, 0xb5, 0x27,
    0xd2, 0x4f, 0xf5, 0x4e, 0x6a, 0xc9, 0x25, 0x7a, 0xac, 0x0a, 0xab, 0xe7, 0x58, 0x13, 0x21, 0xaf,
    0xfa, 0x96, 0xfc, 0x04, 0x8e, 0x89, 0x41, 0xbe, 0xcc, 0x3c, 0x7b, 0x8a, 0x72, 0xdf, 0x4d, 0xee,
    0x10, 0x87, 0x90, 0x5c, 0x99, 0x1b, 0xd4, 0x01, 0x77, 0x2d, 0xb9, 0xb2, 0x16, 0xea, 0xc1, 0xb7,
    0xad, 0x63, 0x1d, 0xe1, 0x24, 0xf4, 0x06, 0x88, 0x9e, 0xff, 0x37, 0x32, 0x76, 0xf1, 0x46, 0x5a,
    0xa7, 0x2b, 0xcd, 0xbb, 0xcb, 0xb0, 0x38, 0x05, 0x2f, 0x02, 0xb1, 0xc7, 0xb4, 0x68, 0x34, 0x12,
    0x0f, 0x61, 0x00, 0x54, 0x9c, 0xd5, 0xa0, 0x97, 0x8b, 0xd8, 0xd0, 0x93, 0x14, 0x73, 0xf3, 0x44,
    0x03, 0x3f, 0xdc, 0xef, 0x17, 0xda, 0xc0, 0x64, 0x79, 0xfd, 0x78, 0xbf, 0xce, 0xe8, 0x50, 0xe5,
    0xa9, 0x0d, 0x92, 0x1e, 0x1c, 0x2e, 0x28, 0x19, 0x4b, 0x83, 0x36, 0x5b, 0x86, 0xb8, 0xb3, 0x95
};

#define SNC_SBOX(x)  (SNC_sbox[(x)])
#define SNC_RSBOX(x) (SNC_rsbox[(x)])

WMKC_PRIVATE(wmkcVoid) SNC_SubBytes(sncState_t *state)
{
    register wmkc_u32 i;
    for(i = 0; i < SNC_NK; ++i) {
        (*state)[0][i] = SNC_SBOX((*state)[0][i]);
        (*state)[1][i] = SNC_SBOX((*state)[1][i]);
        (*state)[2][i] = SNC_SBOX((*state)[2][i]);
        (*state)[3][i] = SNC_SBOX((*state)[3][i]);
        (*state)[4][i] = SNC_SBOX((*state)[4][i]);
        (*state)[5][i] = SNC_SBOX((*state)[5][i]);
        (*state)[6][i] = SNC_SBOX((*state)[6][i]);
        (*state)[7][i] = SNC_SBOX((*state)[7][i]);
    }
}

WMKC_PRIVATE(wmkcVoid) SNC_InvSubBytes(sncState_t *state)
{
    register wmkc_u32 i;
    for(i = 0; i < SNC_NK; ++i) {
        (*state)[0][i] = SNC_RSBOX((*state)[0][i]);
        (*state)[1][i] = SNC_RSBOX((*state)[1][i]);
        (*state)[2][i] = SNC_RSBOX((*state)[2][i]);
        (*state)[3][i] = SNC_RSBOX((*state)[3][i]);
        (*state)[4][i] = SNC_RSBOX((*state)[4][i]);
        (*state)[5][i] = SNC_RSBOX((*state)[5][i]);
        (*state)[6][i] = SNC_RSBOX((*state)[6][i]);
        (*state)[7][i] = SNC_RSBOX((*state)[7][i]);
    }
}

#define SNC_ROWS_MIX(x, y, z, p) (x = ((x ^ y) - z) ^ p)
#define SNC_INV_ROWS_MIX(x, y, z, p) (x = ((x ^ p) + z) ^ y)

WMKC_PRIVATE(wmkcVoid) SNC_RowsMix(sncState_t *state)
{
    register wmkc_u32 i;
    for(i = 0; i < SNC_NB; ++i) {
        SNC_ROWS_MIX((*state)[i][0], (*state)[i][3], (*state)[i][1], 0x07);
        SNC_ROWS_MIX((*state)[i][1], (*state)[i][0], (*state)[i][2], 0x0d);
        SNC_ROWS_MIX((*state)[i][2], (*state)[i][1], (*state)[i][3], 0x17);
        SNC_ROWS_MIX((*state)[i][3], (*state)[i][2], (*state)[i][0], 0x29);
    }
}

WMKC_PRIVATE(wmkcVoid) SNC_InvRowsMix(sncState_t *state)
{
    register wmkc_u32 i;
    for(i = 0; i < SNC_NB; ++i) {
        SNC_INV_ROWS_MIX((*state)[i][3], (*state)[i][2], (*state)[i][0], 0x29);
        SNC_INV_ROWS_MIX((*state)[i][2], (*state)[i][1], (*state)[i][3], 0x17);
        SNC_INV_ROWS_MIX((*state)[i][1], (*state)[i][0], (*state)[i][2], 0x0d);
        SNC_INV_ROWS_MIX((*state)[i][0], (*state)[i][3], (*state)[i][1], 0x07);
    }
}

WMKC_PRIVATE(wmkcVoid) SNC_ColumnShift(sncState_t *state)
{
    static wmkcByte buf;

    buf = (*state)[0][0];
    (*state)[0][0] = (*state)[1][0]; (*state)[1][0] = (*state)[2][0];
    (*state)[2][0] = (*state)[3][0]; (*state)[3][0] = (*state)[4][0];
    (*state)[4][0] = (*state)[5][0]; (*state)[5][0] = (*state)[6][0];
    (*state)[6][0] = (*state)[7][0]; (*state)[7][0] = buf;

    buf = (*state)[0][1];
    (*state)[0][1] = (*state)[2][1]; (*state)[2][1] = (*state)[4][1];
    (*state)[4][1] = (*state)[6][1]; (*state)[6][1] = buf;
    buf = (*state)[1][1];
    (*state)[1][1] = (*state)[3][1]; (*state)[3][1] = (*state)[5][1];
    (*state)[5][1] = (*state)[7][1]; (*state)[7][1] = buf;

    buf = (*state)[0][2];
    (*state)[0][2] = (*state)[3][2]; (*state)[3][2] = (*state)[6][2];
    (*state)[6][2] = (*state)[1][2]; (*state)[1][2] = (*state)[4][2];
    (*state)[4][2] = (*state)[7][2]; (*state)[7][2] = (*state)[2][2];
    (*state)[2][2] = (*state)[5][2]; (*state)[5][2] = buf;

    buf = (*state)[0][3]; (*state)[0][3] = (*state)[4][3]; (*state)[4][3] = buf;
    buf = (*state)[1][3]; (*state)[1][3] = (*state)[5][3]; (*state)[5][3] = buf;
    buf = (*state)[2][3]; (*state)[2][3] = (*state)[6][3]; (*state)[6][3] = buf;
    buf = (*state)[3][3]; (*state)[3][3] = (*state)[7][3]; (*state)[7][3] = buf;
}

WMKC_PRIVATE(wmkcVoid) SNC_InvColumnShift(sncState_t *state)
{
    static wmkcByte buf;

    buf = (*state)[7][3]; (*state)[7][3] = (*state)[3][3]; (*state)[3][3] = buf;
    buf = (*state)[6][3]; (*state)[6][3] = (*state)[2][3]; (*state)[2][3] = buf;
    buf = (*state)[5][3]; (*state)[5][3] = (*state)[1][3]; (*state)[1][3] = buf;
    buf = (*state)[4][3]; (*state)[4][3] = (*state)[0][3]; (*state)[0][3] = buf;

    buf = (*state)[5][2];
    (*state)[5][2] = (*state)[2][2]; (*state)[2][2] = (*state)[7][2];
    (*state)[7][2] = (*state)[4][2]; (*state)[4][2] = (*state)[1][2];
    (*state)[1][2] = (*state)[6][2]; (*state)[6][2] = (*state)[3][2];
    (*state)[3][2] = (*state)[0][2]; (*state)[0][2] = buf;

    buf = (*state)[7][1];
    (*state)[7][1] = (*state)[5][1]; (*state)[5][1] = (*state)[3][1];
    (*state)[3][1] = (*state)[1][1]; (*state)[1][1] = buf;
    buf = (*state)[6][1];
    (*state)[6][1] = (*state)[4][1]; (*state)[4][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[0][1];(*state)[0][1] = buf;

    buf = (*state)[7][0];
    (*state)[7][0] = (*state)[6][0]; (*state)[6][0] = (*state)[5][0];
    (*state)[5][0] = (*state)[4][0]; (*state)[4][0] = (*state)[3][0];
    (*state)[3][0] = (*state)[2][0]; (*state)[2][0] = (*state)[1][0];
    (*state)[1][0] = (*state)[0][0]; (*state)[0][0] = buf;
}

WMKC_PRIVATE(wmkcVoid) SNC_XorWithIV(sncState_t *buf, sncState_t *iv)
{
    register wmkc_u32 i;
    for(i = 0; i < SNC_NK; ++i) {
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

WMKC_PRIVATE(wmkcVoid) SNC_Cipher(SNC_mode mode, sncState_t *state, sncState_t *RoundKey)
{
    register wmkc_u32 i;

    SNC_SubBytes(state);

    for(i = 0; i < SNC_NK; ++i) {
        (*state)[0][i] ^= (*RoundKey)[0][i];
        (*state)[1][i] ^= (*RoundKey)[1][i];
        (*state)[2][i] ^= (*RoundKey)[2][i];
        (*state)[3][i] ^= (*RoundKey)[3][i];
        (*state)[4][i] ^= (*RoundKey)[4][i];
        (*state)[5][i] ^= (*RoundKey)[5][i];
        (*state)[6][i] ^= (*RoundKey)[6][i];
        (*state)[7][i] ^= (*RoundKey)[7][i];

        if(mode == SNC_512 || mode == SNC_768) {
            (*state)[0][i] ^= (*RoundKey)[0][i + 4];
            (*state)[1][i] ^= (*RoundKey)[1][i + 4];
            (*state)[2][i] ^= (*RoundKey)[2][i + 4];
            (*state)[3][i] ^= (*RoundKey)[3][i + 4];
            (*state)[4][i] ^= (*RoundKey)[4][i + 4];
            (*state)[5][i] ^= (*RoundKey)[5][i + 4];
            (*state)[6][i] ^= (*RoundKey)[6][i + 4];
            (*state)[7][i] ^= (*RoundKey)[7][i + 4];
        }

        if(mode == SNC_768) {
            (*state)[0][i] ^= (*RoundKey)[0][i + 8];
            (*state)[1][i] ^= (*RoundKey)[1][i + 8];
            (*state)[2][i] ^= (*RoundKey)[2][i + 8];
            (*state)[3][i] ^= (*RoundKey)[3][i + 8];
            (*state)[4][i] ^= (*RoundKey)[4][i + 8];
            (*state)[5][i] ^= (*RoundKey)[5][i + 8];
            (*state)[6][i] ^= (*RoundKey)[6][i + 8];
            (*state)[7][i] ^= (*RoundKey)[7][i + 8];
        }
    }

    SNC_RowsMix(state);
    SNC_ColumnShift(state);
}

WMKC_PRIVATE(wmkcVoid) SNC_InvCipher(SNC_mode mode, sncState_t *state, sncState_t *RoundKey)
{
    register wmkc_u32 i;

    SNC_InvColumnShift(state);
    SNC_InvRowsMix(state);

    for(i = 0; i < SNC_NK; ++i) {
        if(mode == SNC_768) {
            (*state)[0][i] ^= (*RoundKey)[0][i + 8];
            (*state)[1][i] ^= (*RoundKey)[1][i + 8];
            (*state)[2][i] ^= (*RoundKey)[2][i + 8];
            (*state)[3][i] ^= (*RoundKey)[3][i + 8];
            (*state)[4][i] ^= (*RoundKey)[4][i + 8];
            (*state)[5][i] ^= (*RoundKey)[5][i + 8];
            (*state)[6][i] ^= (*RoundKey)[6][i + 8];
            (*state)[7][i] ^= (*RoundKey)[7][i + 8];
        }

        if(mode == SNC_512 || mode == SNC_768) {
            (*state)[0][i] ^= (*RoundKey)[0][i + 4];
            (*state)[1][i] ^= (*RoundKey)[1][i + 4];
            (*state)[2][i] ^= (*RoundKey)[2][i + 4];
            (*state)[3][i] ^= (*RoundKey)[3][i + 4];
            (*state)[4][i] ^= (*RoundKey)[4][i + 4];
            (*state)[5][i] ^= (*RoundKey)[5][i + 4];
            (*state)[6][i] ^= (*RoundKey)[6][i + 4];
            (*state)[7][i] ^= (*RoundKey)[7][i + 4];
        }

        (*state)[0][i] ^= (*RoundKey)[0][i];
        (*state)[1][i] ^= (*RoundKey)[1][i];
        (*state)[2][i] ^= (*RoundKey)[2][i];
        (*state)[3][i] ^= (*RoundKey)[3][i];
        (*state)[4][i] ^= (*RoundKey)[4][i];
        (*state)[5][i] ^= (*RoundKey)[5][i];
        (*state)[6][i] ^= (*RoundKey)[6][i];
        (*state)[7][i] ^= (*RoundKey)[7][i];
    }

    SNC_InvSubBytes(state);
}

WMKC_PRIVATE(wmkcVoid) SNC_keyExtension(wmkc_u16 keySize, wmkcByte *iv, wmkcByte *key)
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
            iv[(i + 7) % SNC_BLOCKLEN] ^ iv[(i + 16) % SNC_BLOCKLEN];

        iv[i % SNC_BLOCKLEN] ^= buf;

        buf = 
            key[(i + 2)  % keySize] ^ key[(i + 3)  % keySize] ^
            key[(i + 6)  % keySize] ^ key[(i + 7)  % keySize] ^
            key[(i + 10) % keySize] ^ key[(i + 11) % keySize] ^
            key[(i + 14) % keySize] ^ key[(i + 15) % keySize] ^
            key[(i + 18) % keySize] ^ key[(i + 19) % keySize] ^
            key[(i + 22) % keySize] ^ key[(i + 23) % keySize] ^
            key[(i + 26) % keySize] ^ key[(i + 27) % keySize] ^
            key[(i + 29) % keySize] ^ key[(i + 31) % keySize] ^
            iv[(i + 7) % SNC_BLOCKLEN] ^ iv[(i + 16) % SNC_BLOCKLEN];

        iv[(i + 17) % SNC_BLOCKLEN] ^= buf;

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

WMKC_PUBLIC(wmkcVoid) SNC_malloc_init(SNC_ctx **ctx, SNC_mode mode)
{
    if(!(*ctx)) {
        if(!((*ctx) = (SNC_ctx *)malloc(sizeof(SNC_ctx)))) {
        }
    }

    (*ctx)->mode = mode;
    (*ctx)->KN = SNC_KN[mode];
    (*ctx)->NR = SNC_NR[mode];
    (*ctx)->rk = (wmkcByte **)malloc(sizeof(wmkcByte *) * (*ctx)->NR);

    for(wmkcSize x = 0; x < (*ctx)->NR; ++x) {
        if(!((*ctx)->rk[x] = (wmkcByte *)malloc((*ctx)->KN))) {
        }
    }
}

WMKC_PUBLIC(wmkcVoid) SNC_release(SNC_ctx **ctx)
{
    memset((*ctx)->iv, 0x00, SNC_BLOCKLEN);
    free((*ctx)->rk);
    free((*ctx));
    (*ctx)->rk = wmkcNull;
    (*ctx) = wmkcNull;
}

WMKC_PUBLIC(wmkcVoid) SNC_init_ctx(SNC_ctx *ctx, wmkcByte *keyBuf, wmkcByte *ivBuf)
{
    wmkcByte *key = (wmkcByte *)malloc(ctx->KN);
    static wmkcByte iv[SNC_BLOCKLEN];
    static wmkc_u32 r;

    memcpy(key, keyBuf, ctx->KN);
    memcpy(ctx->iv, ivBuf, SNC_BLOCKLEN);
    memcpy(iv, ctx->iv, SNC_BLOCKLEN);

    for(r = 0; r < ctx->NR; ++r) {
        memcpy(ctx->rk[r], key, ctx->KN);
        SNC_keyExtension(ctx->KN, iv, key);
        SNC_XorWithIV((sncState_t *)iv, (sncState_t *)key);
        SNC_keyExtension(ctx->KN, iv, key);
        SNC_Cipher(ctx->mode, (sncState_t *)iv, (sncState_t *)key);
        if(ctx->mode == SNC_512 || ctx->mode == SNC_768) {
            SNC_XorWithIV((sncState_t *)(key + 32), (sncState_t *)iv);
            SNC_Cipher(ctx->mode, (sncState_t *)iv, (sncState_t *)(key + 32));
        }
        if(ctx->mode == SNC_768) {
            SNC_Cipher(ctx->mode, (sncState_t *)iv, (sncState_t *)(key + 64));
        }
    }

    memset(key, 0x00, ctx->KN);
    free(key);
    key = wmkcNull;
}

WMKC_PUBLIC(wmkcVoid) SNC_ECB_Encrypt(SNC_ctx *ctx, wmkcByte *buf, wmkcSize size)
{
    register wmkcSize r;
    register wmkcSize i;

    sncState_t *bufState = (sncState_t *)buf;
    size /= SNC_BLOCKLEN;

    for(r = 0; r < ctx->NR; ++r) {
        for(i = 0; i < size; ++i) {
            SNC_Cipher(ctx->mode, (bufState + i), (sncState_t *)ctx->rk[r]);
        }
    }
}

WMKC_PUBLIC(wmkcVoid) SNC_ECB_Decrypt(SNC_ctx *ctx, wmkcByte *buf, wmkcSize size)
{
    register wmkcSize r;
    register wmkcSize i;

    sncState_t *bufState = (sncState_t *)buf;
    size /= SNC_BLOCKLEN;

    for(r = 0; r < ctx->NR; ++r) {
        for(i = 0; i < size; ++i) {
            SNC_InvCipher(ctx->mode, (bufState + i), (sncState_t *)ctx->rk[ctx->NR - r - 1]);
        }
    }
}

WMKC_PUBLIC(wmkcVoid) SNC_CBC_Encrypt(SNC_ctx *ctx, wmkcByte *buf, wmkcSize size)
{
    register wmkcSize r, i;
    static wmkcByte round_iv[SNC_BLOCKLEN];

    sncState_t *bufState = (sncState_t *)buf;
    sncState_t *ivState = (sncState_t *)round_iv;
    size /= SNC_BLOCKLEN;

    for(r = 0; r < ctx->NR; ++r) {
        memcpy(round_iv, ctx->iv, SNC_BLOCKLEN);
        for(i = 0; i < size; ++i) {
            SNC_XorWithIV(bufState + i, ivState);
            SNC_Cipher(ctx->mode, bufState + i, (sncState_t *)ctx->rk[r]);
            memcpy(ivState, bufState + i, SNC_BLOCKLEN);
        }
    }
}

WMKC_PUBLIC(wmkcVoid) SNC_CBC_Decrypt(SNC_ctx *ctx, wmkcByte *buf, wmkcSize size)
{
    register wmkcSize r;
    register wmkcSize i;
    static wmkcByte round_iv[SNC_BLOCKLEN];
    static wmkcByte round_buf[SNC_BLOCKLEN];

    sncState_t *bufState = (sncState_t *)buf;
    sncState_t *ivState = (sncState_t *)round_iv;
    size /= SNC_BLOCKLEN;

    for(r = 0; r < ctx->NR; ++r) {
        memcpy(round_iv, ctx->iv, SNC_BLOCKLEN);
        for(i = 0; i < size; ++i) {
            memcpy(round_buf, bufState + i, SNC_BLOCKLEN);
            SNC_InvCipher(ctx->mode, bufState + i, (sncState_t *)ctx->rk[ctx->NR - r - 1]);
            SNC_XorWithIV(bufState + i, ivState);
            memcpy(ivState, round_buf, SNC_BLOCKLEN);
        }
    }
}
