#include <crypto/snc.h>

/*
* 用于置换明文块的置换盒。
* Replacement Box for Replacing Clear Text Blocks.
* 
* 比如（Example）
*     0x9b -> sbox -> 0x13
*/
SN_PRIVATE_CONST(snByte) SNC_sbox[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
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

/*
* 用于将通过置换盒的数据还原的逆置换盒。
* An rsbox used to restore data processed by sbox.
* 
* 比如（Example）
*     0x13 -> rsbox -> 0x9b
*/
SN_PRIVATE_CONST(snByte) SNC_rsbox[256] = {
    // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
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

/*
* 使用宏定义更好的使用置换盒与逆置换盒。
* Using macro definitions to better use sbox and rsbox
*
* 比如（Example）
*     SNC_SBOX(0x9b)  -> 0x13
*     SNC_RSBOX(0x13) -> 0x9b
*/
#define SNC_SBOX(x)  (SNC_sbox[(x)])
#define SNC_RSBOX(x) (SNC_rsbox[(x)])

/*
* 块置换函数，用于将分组的明文块进行置换。
* Block substitute function, used to hand over each set of plaintext blocks to SBOX
* for processing.
* 
* 按照每列处理
* Process according to each column.
*/
SN_PRIVATE(snVoid) SNC_SubBytes SN_FUNC_OF((sncState *state))
{
    snFast sn_u32 i;
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

/*
* 块逆置换函数，用于将分组的密文块进行逆置换
* Block reverse substitute function, used to hand over grouped ciphertext blocks to
* RSBOX for reverse restoration.
*
* 按照每列处理
* Process according to each column.
*/
SN_PRIVATE(snVoid) SNC_InvSubBytes SN_FUNC_OF((sncState *state))
{
    snFast sn_u32 i;
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

/*
* SNC_ROWS_MIX:
*     用于行混合的宏定义。
*     A macro definition used for line mixing, which confuses the data of each plaintext block.
*
* 参数（Parameter）:
*     x  第一个字节，会被赋值。     （The first byte will be assigned a value.）
*     a  第二个字节，用于混淆x的值。（The second byte is used to obfuscate the value of x.）
*     b  第三个字节，用于混淆x的值。（The third byte is used to obfuscate the value of x.）
*     c  第四个字节，用于混淆x的值。（The fourth byte is used to obfuscate the value of x.）
*
* 按照每一个行进行处理（Process according to each line.）
*/
#define SNC_ROWS_MIX(x, a, b, c) (x = ((x-7) ^ (c + a)) ^ b)
/*
* SNC_INV_ROWS_MIX:
*     用于逆行混合的宏定义。
*     Macro definition used for reverse line mixing, restoring the data of each plaintext
*     block that is confused.
*
* 参数（Parameter）:
*     同SNC_ROWS_MIX宏定义的解释，但是是逆向还原数据。
*     The explanation is the same as that defined by the "SNC_ROWS_MIX" macro, but it is
*     a reverse restore of data.
*
* 按照每一个行进行处理（Process according to each rows.）
*/
#define SNC_INV_ROWS_MIX(x, a, b, c) (x = ((x ^ b) ^ (c + a)) + 7)

/*
* 行混合函数，基于明文的不确定性与一个以上未知数运算的无数解性来确保安全。
* The row mixing function ensures safety based on the uncertainty of plaintext and the
* countless solutions of more than one unknown number operation.
*/
SN_PRIVATE(snVoid) SNC_RowsMix SN_FUNC_OF((sncState *state))
{
    snFast sn_u32 i;
    for(i = 0; i < SNC_NB; ++i) {
        SNC_ROWS_MIX((*state)[i][0], (*state)[i][2], (*state)[i][1], (*state)[i][3]);
        SNC_ROWS_MIX((*state)[i][1], (*state)[i][0], (*state)[i][3], (*state)[i][2]);
        SNC_ROWS_MIX((*state)[i][2], (*state)[i][3], (*state)[i][1], (*state)[i][0]);
        SNC_ROWS_MIX((*state)[i][3], (*state)[i][1], (*state)[i][2], (*state)[i][0]);
    }
}

/*
* 逆行混合函数，将经过SNC_RowsMix函数处理后的数据还原。
* Reverse row mixing function, which restores the data processed by the "SNC_RowsMix" function.
*/
SN_PRIVATE(snVoid) SNC_InvRowsMix SN_FUNC_OF((sncState *state))
{
    snFast sn_u32 i;
    for(i = 0; i < SNC_NB; ++i) {
        SNC_INV_ROWS_MIX((*state)[i][3], (*state)[i][1], (*state)[i][2], (*state)[i][0]);
        SNC_INV_ROWS_MIX((*state)[i][2], (*state)[i][3], (*state)[i][1], (*state)[i][0]);
        SNC_INV_ROWS_MIX((*state)[i][1], (*state)[i][0], (*state)[i][3], (*state)[i][2]);
        SNC_INV_ROWS_MIX((*state)[i][0], (*state)[i][2], (*state)[i][1], (*state)[i][3]);
    }
}

/*
* 列移位函数，第一列移位一个元素，第二列移位两个元素，以此类推至第四列。
* Column shift function, where the first column shifts one element, the second column
* shifts two elements, and so on until the fourth column.
*
* 按照每列处理（Process according to each column.）
*/
SN_PRIVATE(snVoid) SNC_ColumnShift SN_FUNC_OF((sncState *state))
{
    static snByte buf;

    //* 第一列（First column）
    buf = (*state)[0][0];
    (*state)[0][0] = (*state)[1][0]; (*state)[1][0] = (*state)[2][0];
    (*state)[2][0] = (*state)[3][0]; (*state)[3][0] = (*state)[4][0];
    (*state)[4][0] = (*state)[5][0]; (*state)[5][0] = (*state)[6][0];
    (*state)[6][0] = (*state)[7][0]; (*state)[7][0] = buf;

    //* 第二列（Second column）
    buf = (*state)[0][1];
    (*state)[0][1] = (*state)[2][1]; (*state)[2][1] = (*state)[4][1];
    (*state)[4][1] = (*state)[6][1]; (*state)[6][1] = buf;
    buf = (*state)[1][1];
    (*state)[1][1] = (*state)[3][1]; (*state)[3][1] = (*state)[5][1];
    (*state)[5][1] = (*state)[7][1]; (*state)[7][1] = buf;

    //* 第三列（Third column）
    buf = (*state)[0][2];
    (*state)[0][2] = (*state)[3][2]; (*state)[3][2] = (*state)[6][2];
    (*state)[6][2] = (*state)[1][2]; (*state)[1][2] = (*state)[4][2];
    (*state)[4][2] = (*state)[7][2]; (*state)[7][2] = (*state)[2][2];
    (*state)[2][2] = (*state)[5][2]; (*state)[5][2] = buf;

    //* 第四列（Fourth column）
    buf = (*state)[0][3]; (*state)[0][3] = (*state)[4][3]; (*state)[4][3] = buf;
    buf = (*state)[1][3]; (*state)[1][3] = (*state)[5][3]; (*state)[5][3] = buf;
    buf = (*state)[2][3]; (*state)[2][3] = (*state)[6][3]; (*state)[6][3] = buf;
    buf = (*state)[3][3]; (*state)[3][3] = (*state)[7][3]; (*state)[7][3] = buf;
}

/*
* 逆列移位函数，第四列移位四个元素，第三列移位三个元素，以此类推至第一列。
* The inverse column shift function shifts four elements in the fourth column, three
* elements in the third column, and so on to the first column.
*
* 按照每列处理（Process according to each column.）
*/
SN_PRIVATE(snVoid) SNC_InvColumnShift SN_FUNC_OF((sncState *state))
{
    static snByte buf;

    //* 第四列（Fourth column）
    buf = (*state)[7][3]; (*state)[7][3] = (*state)[3][3]; (*state)[3][3] = buf;
    buf = (*state)[6][3]; (*state)[6][3] = (*state)[2][3]; (*state)[2][3] = buf;
    buf = (*state)[5][3]; (*state)[5][3] = (*state)[1][3]; (*state)[1][3] = buf;
    buf = (*state)[4][3]; (*state)[4][3] = (*state)[0][3]; (*state)[0][3] = buf;

    //* 第三列（Third column）
    buf = (*state)[5][2];
    (*state)[5][2] = (*state)[2][2]; (*state)[2][2] = (*state)[7][2];
    (*state)[7][2] = (*state)[4][2]; (*state)[4][2] = (*state)[1][2];
    (*state)[1][2] = (*state)[6][2]; (*state)[6][2] = (*state)[3][2];
    (*state)[3][2] = (*state)[0][2]; (*state)[0][2] = buf;

    //* 第二列（Second column）
    buf = (*state)[7][1];
    (*state)[7][1] = (*state)[5][1]; (*state)[5][1] = (*state)[3][1];
    (*state)[3][1] = (*state)[1][1]; (*state)[1][1] = buf;
    buf = (*state)[6][1];
    (*state)[6][1] = (*state)[4][1]; (*state)[4][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[0][1];(*state)[0][1] = buf;

    //* 第一列（First column）
    buf = (*state)[7][0];
    (*state)[7][0] = (*state)[6][0]; (*state)[6][0] = (*state)[5][0];
    (*state)[5][0] = (*state)[4][0]; (*state)[4][0] = (*state)[3][0];
    (*state)[3][0] = (*state)[2][0]; (*state)[2][0] = (*state)[1][0];
    (*state)[1][0] = (*state)[0][0]; (*state)[0][0] = buf;
}

/*
* 将明文分组或密文分组与初始向量分组进行异或的函数，用于CBC模式。
* The function of XOR between plaintext blocks or ciphertext blocks and Initialization
* vector, used in CBC mode.
*
* 按照每列处理（Process according to each column.）
*/
SN_PRIVATE(snVoid) SNC_XorWithIV SN_FUNC_OF((sncState *buf, sncState *iv))
{
    snFast sn_u32 i;
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

/*
* 加密块函数，将输入的块进行加密（Encrypt block function to encrypt input blocks）
*/
SN_PRIVATE(snVoid) SNC_Cipher
SN_FUNC_OF((SNC_mode mode, sncState *state, snByte *RoundKey))
{
    snFast sn_u32 i;

    //* 执行块置换函数（Execute block substitute function）
    SNC_SubBytes(state);

    for(i = 0; i < SNC_NK; ++i) {
        //* 此块为基础加密，也就是使用[0 - 31]字节的密钥进行加密。
        //* This block is general encryption, which means using a key of [0-31] bytes
        //* for encryption.
        (*state)[0][i] ^= *(RoundKey + (SNC_NK * 0 + i));
        (*state)[1][i] ^= *(RoundKey + (SNC_NK * 1 + i));
        (*state)[2][i] ^= *(RoundKey + (SNC_NK * 2 + i));
        (*state)[3][i] ^= *(RoundKey + (SNC_NK * 3 + i));
        (*state)[4][i] ^= *(RoundKey + (SNC_NK * 4 + i));
        (*state)[5][i] ^= *(RoundKey + (SNC_NK * 5 + i));
        (*state)[6][i] ^= *(RoundKey + (SNC_NK * 6 + i));
        (*state)[7][i] ^= *(RoundKey + (SNC_NK * 7 + i));

        if(mode == SNC_512 || mode == SNC_768) {
            //* 此块为复合加密，也就是使用[32 - 63]字节的密钥再次加密。
            //* This block is compound encryption, which means using a key of [32-63] bytes
            //* to encrypt the plaintext again.
            (*state)[0][i] ^= *(RoundKey + (SNC_NK * 0 + (i + 32)));
            (*state)[1][i] ^= *(RoundKey + (SNC_NK * 1 + (i + 32)));
            (*state)[2][i] ^= *(RoundKey + (SNC_NK * 2 + (i + 32)));
            (*state)[3][i] ^= *(RoundKey + (SNC_NK * 3 + (i + 32)));
            (*state)[4][i] ^= *(RoundKey + (SNC_NK * 4 + (i + 32)));
            (*state)[5][i] ^= *(RoundKey + (SNC_NK * 5 + (i + 32)));
            (*state)[6][i] ^= *(RoundKey + (SNC_NK * 6 + (i + 32)));
            (*state)[7][i] ^= *(RoundKey + (SNC_NK * 7 + (i + 32)));
        }

        if(mode == SNC_768) {
            //* 此块为复合加密，也就是使用[64 - 95]字节的密钥再次加密。
            //* This block is compound encryption, which means using a key of [64-95] bytes
            //* to encrypt the plaintext again.
            (*state)[0][i] ^= *(RoundKey + (SNC_NK * 0 + (i + 64)));
            (*state)[1][i] ^= *(RoundKey + (SNC_NK * 1 + (i + 64)));
            (*state)[2][i] ^= *(RoundKey + (SNC_NK * 2 + (i + 64)));
            (*state)[3][i] ^= *(RoundKey + (SNC_NK * 3 + (i + 64)));
            (*state)[4][i] ^= *(RoundKey + (SNC_NK * 4 + (i + 64)));
            (*state)[5][i] ^= *(RoundKey + (SNC_NK * 5 + (i + 64)));
            (*state)[6][i] ^= *(RoundKey + (SNC_NK * 6 + (i + 64)));
            (*state)[7][i] ^= *(RoundKey + (SNC_NK * 7 + (i + 64)));
        }
    }

    //* 执行列移位函数。（Execute column shift function）
    SNC_ColumnShift(state);
    //* 执行行混合函数（Execute row mixing function.）
    SNC_RowsMix(state);
}

/*
* 解密块函数，将输入的块进行解密（Decrypt block function to decrypt input blocks）
*/
SN_PRIVATE(snVoid) SNC_InvCipher
SN_FUNC_OF((SNC_mode mode, sncState *state, snByte *RoundKey))
{
    snFast sn_u32 i;

    //* 执行逆行混合函数。（Execute inverse mixed function.）
    SNC_InvRowsMix(state);
    //* 执行逆列移位函数（Execute inverse column shift function）
    SNC_InvColumnShift(state);

    for(i = 0; i < SNC_NK; ++i) {
        if(mode == SNC_768) {
            //* 此块为复合解密，也就是使用[64 - 95]字节的密钥解密。
            //* This block is compound decryption, which means using a key of [64-95] bytes
            //* to decrypt the ciphertext.
            (*state)[0][i] ^= *(RoundKey + (SNC_NK * 0 + (i + 64)));
            (*state)[1][i] ^= *(RoundKey + (SNC_NK * 1 + (i + 64)));
            (*state)[2][i] ^= *(RoundKey + (SNC_NK * 2 + (i + 64)));
            (*state)[3][i] ^= *(RoundKey + (SNC_NK * 3 + (i + 64)));
            (*state)[4][i] ^= *(RoundKey + (SNC_NK * 4 + (i + 64)));
            (*state)[5][i] ^= *(RoundKey + (SNC_NK * 5 + (i + 64)));
            (*state)[6][i] ^= *(RoundKey + (SNC_NK * 6 + (i + 64)));
            (*state)[7][i] ^= *(RoundKey + (SNC_NK * 7 + (i + 64)));
        }

        if(mode == SNC_512 || mode == SNC_768) {
            //* 此块为复合解密，也就是[32 - 63]字节的密钥再次解密。
            //* This block is compound decryption, which means using a key of [32-63] bytes
            //* to decrypt the ciphertext again.
            (*state)[0][i] ^= *(RoundKey + (SNC_NK * 0 + (i + 32)));
            (*state)[1][i] ^= *(RoundKey + (SNC_NK * 1 + (i + 32)));
            (*state)[2][i] ^= *(RoundKey + (SNC_NK * 2 + (i + 32)));
            (*state)[3][i] ^= *(RoundKey + (SNC_NK * 3 + (i + 32)));
            (*state)[4][i] ^= *(RoundKey + (SNC_NK * 4 + (i + 32)));
            (*state)[5][i] ^= *(RoundKey + (SNC_NK * 5 + (i + 32)));
            (*state)[6][i] ^= *(RoundKey + (SNC_NK * 6 + (i + 32)));
            (*state)[7][i] ^= *(RoundKey + (SNC_NK * 7 + (i + 32)));
        }

        //* 此块为基础解密，也就是使用[0 - 31]字节的密钥进行解密。
        //* This block is for general decryption, which means using a key of [0-31] bytes
        //* for decryption.
        (*state)[0][i] ^= *(RoundKey + (SNC_NK * 0 + i));
        (*state)[1][i] ^= *(RoundKey + (SNC_NK * 1 + i));
        (*state)[2][i] ^= *(RoundKey + (SNC_NK * 2 + i));
        (*state)[3][i] ^= *(RoundKey + (SNC_NK * 3 + i));
        (*state)[4][i] ^= *(RoundKey + (SNC_NK * 4 + i));
        (*state)[5][i] ^= *(RoundKey + (SNC_NK * 5 + i));
        (*state)[6][i] ^= *(RoundKey + (SNC_NK * 6 + i));
        (*state)[7][i] ^= *(RoundKey + (SNC_NK * 7 + i));
    }

    //* 执行块逆置换函数（Execute block inverse substitute function）
    SNC_InvSubBytes(state);
}

/*
* 密钥扩展函数，用于根据初始用户密钥生成后续每一轮加密与解密所需的子密钥。
* The key extension function is used to generate sub keys required for each subsequent
* round of encryption and decryption based on the initial user key.
*/
SN_PRIVATE(snVoid) SNC_keyExtension SN_FUNC_OF((sn_u16 keySize, snByte *iv, snByte *key))
{
    static snSize i;
    static snByte buf;

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

        //* 处理整个密钥块来生成新的密钥（Process the entire key block to generate a new key.）
        //* 确保每一轮子密钥都不可能被猜出来（Ensure that every sub key cannot be guessed out.）
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

// 为SNC对象申请内存空间
SN_PUBLIC(snErr_ctx *) SNC_new SN_OPEN_API
SN_FUNC_OF((SNC_ctx **ctx, SNC_mode mode))
{
    snErr_ctx *error = snNull;
    snErr_new(error);

    if(!ctx) {
        snErr_return(error, snErr_ErrNullData, "ctx is NULL.");
    }

    if(!snMemoryNew(SNC_ctx *, (*ctx), sizeof(SNC_ctx))) {
        snErr_return(error, snErr_ErrMemory, "(*ctx) Failed to apply for memory.");
    }

    (*ctx)->mode = mode;
    (*ctx)->KN = SNC_KN[mode];
    (*ctx)->NR = SNC_NR[mode];
    snZeroObject((*ctx)->roundKey, sizeof((*ctx)->roundKey));

    snErr_return(error, snErr_OK, "OK.");
}

// 释放SNC对象
SN_PUBLIC(snErr_ctx *) SNC_free SN_OPEN_API
SN_FUNC_OF((SNC_ctx **ctx))
{
    snErr_ctx *error = snNull;
    snErr_new(error);
    if(!ctx) {
        snErr_return(error, snErr_ErrNullData, "ctx is NULL.");
    }

    memset((*ctx)->iv, 0x00, SNC_BLOCKLEN);
    snZeroObject((*ctx)->roundKey, sizeof((*ctx)->roundKey));
    snMemoryFree((*ctx));

    snErr_return(error, snErr_OK, "OK.");
}

/*
* 初始化SNC数据结构的函数，用于根据用户输入的基本密钥生成每一轮的子密钥。
* Function to initialize the SNC data structure, used to generate sub keys for each
* round based on the basic key input by the user.
*/
SN_PUBLIC(snErr_ctx *) SNC_init SN_OPEN_API
SN_FUNC_OF((SNC_ctx *ctx, snByte *keyBuf, snByte *ivBuf))
{
    snErr_ctx *error = snNull;
    snErr_new(error);
    if(!ctx || !keyBuf || !ivBuf) {
        snErr_return(error, snErr_ErrNullData, "ctx or keyBuf or ivBuf is NULL.");
    }

    snByte *key = snNull;
    snByte iv[SNC_BLOCKLEN];
    sn_u32 r;
    /*
    * snByte *key = snNull;
    *     密钥副本，用于存放每轮的子密钥。
    *     Key copy, used to store sub keys for each round.
    *
    * snByte iv[SNC_BLOCKLEN];
    *     初始向量副本，用于保证输入的初始向量不发生变化。
    *     The copy of Initialization vector is used to ensure that the input Initialization
    *     vector does not change.
    */
    if(!snMemoryNew(snByte *, key, ctx->KN)) {
        snErr_return(error, snErr_ErrMemory, "key Failed to apply for memory.");
    }

    memcpy(ctx->iv, ivBuf, SNC_BLOCKLEN);
    memcpy(key, keyBuf, ctx->KN);
    memcpy(iv, ivBuf, SNC_BLOCKLEN);

    /*
    * 第一轮密钥扩展会先将用户输入的基本密钥写入至第一个子密钥中。
    * The first round of key expansion will first write the basic key entered by the
    * user into the first sub key.
    * 
    * 后续每一轮的子密钥都是基于上一轮的密钥进行密钥扩展得到的。
    * The sub keys for each subsequent round are obtained through key expansion based
    * on the previous round of keys.
    * 
    * 此子密钥初始化函数具有完美的抗分析能力。
    * This sub key initialization function has perfect anti analysis ability.
    */
    for(r = 0; r < ctx->NR; ++r) {
        memcpy(ctx->roundKey + (ctx->KN * r), key, ctx->KN);
        SNC_keyExtension(ctx->KN, iv, key);
        SNC_XorWithIV((sncState *)iv, (sncState *)key);
        SNC_keyExtension(ctx->KN, iv, key);
        if(ctx->mode == SNC_512 || ctx->mode == SNC_768) {
            SNC_XorWithIV((sncState *)(key + 32), (sncState *)iv);
        }
        if(ctx->mode == SNC_768) {
            SNC_XorWithIV((sncState *)(key + 64), (sncState *)iv);
        }
        SNC_Cipher(ctx->mode, (sncState *)iv, key);
    }

    //* 用于确保内存安全（Used to ensure memory security.）
    snZeroObject(key, ctx->KN);
    snZeroObject(iv, SNC_BLOCKLEN);
    free(key);
    key = snNull;
    snErr_return(error, snErr_OK, "OK.");
}

//* ECB模式加密
SN_PUBLIC(snVoid) SNC_ECB_Encrypt SN_OPEN_API
SN_FUNC_OF((SNC_ctx *ctx, snByte *buf, snSize size))
{
    snFast snSize r;
    snFast snSize i;

    //* 将SNC的块结构指向输入的数据，用于提高性能和简化代码
    sncState *bufState = (sncState *)buf;
    //* 因为是直接使用块分组进行操作，所以需要将长度除以块的长度
    size /= SNC_BLOCKLEN;

    for(r = 0; r < ctx->NR; ++r) {
        //* 每轮使用对应的子密钥进行一次数据的整体加密
        for(i = 0; i < size; ++i) {
            SNC_Cipher(ctx->mode, (bufState + i), ctx->roundKey + (ctx->KN * r));
        }
    }
}

//* ECB模式解密
SN_PUBLIC(snVoid) SNC_ECB_Decrypt SN_OPEN_API
SN_FUNC_OF((SNC_ctx *ctx, snByte *buf, snSize size))
{
    snFast snSize r;
    snFast snSize i;

    //* 将SNC的块结构指向输入的数据，用于提高性能和简化代码
    sncState *bufState = (sncState *)buf;
    //* 因为是直接使用块分组进行操作，所以需要将长度除以块的长度
    size /= SNC_BLOCKLEN;

    for(r = 0; r < ctx->NR; ++r) {
        //* 每轮使用对应的子密钥进行一次数据的整体解密
        for(i = 0; i < size; ++i) {
            SNC_InvCipher(ctx->mode, (bufState + i), ctx->roundKey + (ctx->KN * (ctx->NR - r - 1)));
        }
    }
}

//* CBC模式加密
SN_PUBLIC(snVoid) SNC_CBC_Encrypt SN_OPEN_API
SN_FUNC_OF((SNC_ctx *ctx, snByte *buf, snSize size))
{
    snFast snSize r, i;
    static snByte round_iv[SNC_BLOCKLEN];

    sncState *bufState = (sncState *)buf;
    sncState *ivState = (sncState *)round_iv;
    size /= SNC_BLOCKLEN;

    for(r = 0; r < ctx->NR; ++r) {
        memcpy(round_iv, ctx->iv, SNC_BLOCKLEN);
        for(i = 0; i < size; ++i) {
            SNC_XorWithIV(bufState + i, ivState);
            SNC_Cipher(ctx->mode, bufState + i, ctx->roundKey + (ctx->KN * r));
            memcpy(ivState, bufState + i, SNC_BLOCKLEN);
        }
    }
}

//* CBC模式解密
SN_PUBLIC(snVoid) SNC_CBC_Decrypt SN_OPEN_API
SN_FUNC_OF((SNC_ctx *ctx, snByte *buf, snSize size))
{
    snFast snSize r;
    snFast snSize i;
    static snByte round_iv[SNC_BLOCKLEN];
    static snByte round_buf[SNC_BLOCKLEN];

    sncState *bufState = (sncState *)buf;
    sncState *ivState = (sncState *)round_iv;
    size /= SNC_BLOCKLEN;

    for(r = 0; r < ctx->NR; ++r) {
        memcpy(round_iv, ctx->iv, SNC_BLOCKLEN);
        for(i = 0; i < size; ++i) {
            memcpy(round_buf, bufState + i, SNC_BLOCKLEN);
            SNC_InvCipher(ctx->mode, bufState + i, ctx->roundKey + (ctx->KN * (ctx->NR - r - 1)));
            SNC_XorWithIV(bufState + i, ivState);
            memcpy(ivState, round_buf, SNC_BLOCKLEN);
        }
    }
}

