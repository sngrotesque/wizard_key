#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#ifndef __SN_OBJECT__
#define __SN_OBJECT__ 1

#define FILL_DATA_N      256    // 256 Bytes
#define FILL_KEY_N       256    // 256 Bytes
#define GROUP_BYTES_N    256    // 单组字长
#define FILL_DATA        121    // 0x79
#define NUMBER_OF_ROUNDS 16     // 加密解密轮数

typedef struct {
    u8 *data;    // 数据
    u64 data_n;  // 数据长度
    u8 *u_key;   // 密码(密钥)
} sn;

// 初始混淆值 (可自定义)
const static u8 iv[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

#ifndef __BASE64_H__
#define __BASE64_H__ 1

// 计算Base64编码后的数据长度
static u64 b64en_n(u64 _n)
{
    switch(_n % 3) {
        case 0:
            _n = _n / 3 * 4; break;
        default:
            _n = (_n / 3 + 1) * 4; break;
    } return _n;
}

// 计算Base64解码码后的数据长度
static u64 b64de_n(char *data)
{
    u64 len = strlen(data), _n;
    if (strstr(data, "==")) {
        _n = len / 4 * 3 - 2;
    } else if (strstr(data, "=")) {
        _n = len / 4 * 3 - 1;
    } else {
        _n = len / 4 * 3;
    } return _n;
}

static u8 *b64en(u8 *data, u64 _n)
{
    const char *B64_T = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    u64 len = b64en_n(_n), i, j;
    u8 *res = (u8 *)malloc(len + 1);
    res[len] = 0x00;

    for (i = 0, j = 0; i < len - 2; j += 3, i += 4) {
        res[i]   = B64_T[data[j]>>2];
        res[i+1] = B64_T[(data[j] & 0x3)   << 4 | (data[j+1] >> 4)];
        res[i+2] = B64_T[(data[j+1] & 0xf) << 2 | (data[j+2] >> 6)];
        res[i+3] = B64_T[data[j+2] & 0x3f];
    }
    
    switch (_n % 3) {
        case 1:
            res[i - 2] = '=';
            res[i - 1] = '=';
            break;
        case 2:
            res[i - 1] = '=';
            break;
    }
    return res;
}

static u8 *b64de(u8 *code)
{
    u32 table[123] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  62, 0,  0,  0,  63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0,  0,
        0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18,
        19, 20, 21, 22, 23, 24, 25, 0,  0,  0,  0,  0,  0,  26, 27, 28, 29, 30, 31, 32, 33,
        34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};
    u64 s_len = strlen((char *)code), len = b64de_n((char *)code), i, j;
    u8 *res = (u8 *)malloc(len + 1);
    res[len] = 0x00;
    
    for (i = 0, j = 0; i < s_len - 2; j += 3, i += 4) {
        res[j]   = ((u8)table[code[i]])    << 2  | (((u8)table[code[i+1]]) >> 4);
        res[j+1] = (((u8)table[code[i+1]]) << 4) | (((u8)table[code[i+2]]) >> 2);
        res[j+2] = (((u8)table[code[i+2]]) << 6) | ((u8)table[code[i+3]]);
    }
    return res;
}
#endif

#ifndef __SN_FUNCTION__
#define __SN_FUNCTION__ 1

// 分组函数
static u8 **SN_GROUP(uint8_t *data, size_t n)
{
    size_t group_n = n / GROUP_BYTES_N;
    uint8_t **p, *temp = data;

    if (n % GROUP_BYTES_N != 0) group_n += 1;
    
    p = (uint8_t **)malloc(group_n * (GROUP_BYTES_N + 1));

    for(size_t x = 0; x < group_n; ++x) {
        p[x] = (uint8_t *)calloc(GROUP_BYTES_N + 1, 1);
        memcpy(p[x], temp, GROUP_BYTES_N);
        temp += GROUP_BYTES_N;
    }

    return p;
}

// 用来精简代码量的函数
static void SN_REPEAT_INIT(u8 *T_DATA, sn *data, u32 FILL_N, int NUM, u64 x)
{
    T_DATA = (u8 *)malloc(FILL_N);
    memcpy(T_DATA, data->data, data->data_n);
    free(data->data); data->data = T_DATA;
    for(; x < FILL_N - NUM; ++x) data->data[x] = FILL_DATA;
}

// 数据填充函数
static int SN_INIT_DATA(sn *data)
{
    if (data->data == NULL || data->data_n == 0) return EOF;
    u64 MOD = data->data_n % FILL_DATA_N;
    u64 x = data->data_n;
    u32 FILL_N, TEMP_N;
    u8 *T_DATA;
    if (MOD == 0) {
        FILL_N = FILL_DATA_N + data->data_n;
        SN_REPEAT_INIT(T_DATA, data, FILL_N, 2, x);
        data->data[FILL_N - 2] = 255;
        data->data[FILL_N - 1] = 255;
    } else {
        TEMP_N = FILL_DATA_N - MOD;
        FILL_N = TEMP_N + data->data_n;
        SN_REPEAT_INIT(T_DATA, data, FILL_N, 1, x);
        data->data[FILL_N - 1] = TEMP_N;
    } data->data_n = FILL_N;
    return 0;
}

// 密钥填充函数
static int SN_INIT_KEY(sn *data)
{
    if (data->u_key == NULL || !data->u_key) return EOF;
    u64 key_len = strlen((char *)data->u_key);
    if (key_len > FILL_KEY_N) {
        u8 *temp = (u8 *)malloc(FILL_KEY_N);
        memcpy(temp, data->u_key, FILL_KEY_N);
        free(data->u_key); data->u_key = temp;
    } else {
        for(int x = key_len, index = 0; x < FILL_KEY_N; ++x, ++index) {
            if (index == key_len) index = 0;
            data->u_key[x] = data->u_key[index];
        }
    } return 0;
}

// 密钥生成函数
static u8 **KEY_OBFUSCATION(sn *data)
{
    u8 **key = (u8 **)malloc(NUMBER_OF_ROUNDS * 8);
    u8 *key_t = (u8 *)malloc(FILL_KEY_N);
    for(u32 x = 0; x < NUMBER_OF_ROUNDS; ++x) {
        key[x] = (u8 *)malloc(FILL_KEY_N);
        memcpy(key[x], data->u_key, FILL_KEY_N);
        for(u32 y = 0; y < FILL_KEY_N; ++y) {
            if(y == 0) {
                key_t[y] = (y ^ ((key[x][y] ^ key[x][255]) ^ key[x][174]) ^ iv[y]) - x;
            } else {
                key_t[y] = (y ^ ((key[x][y] ^ key[x][y-1]) ^ key[x][174]) ^ iv[y]) - x;
            }
        } data->u_key = key_t;
    } free(data->u_key);
    return key;
}

// 加密函数
static int ENCRYPT_DATA(sn *data)
{
    if (data->data == NULL || data->data_n == 0 || data->u_key == NULL) {
        return EOF;}
    u8 **key = KEY_OBFUSCATION(data), keyindex;
    u64 rounds, x;
    for(rounds = 0; rounds < NUMBER_OF_ROUNDS; ++rounds) {
        for(x = keyindex = 0; x < data->data_n; ++x, ++keyindex) {
            data->data[x] ^= key[rounds][keyindex];
        }
    }
    data->data = b64en(data->data, data->data_n);
    data->data_n = b64en_n(data->data_n);
    return 0;
}

// 解密函数
static int DECRYPT_DATA(sn *data)
{
    u8 **key = KEY_OBFUSCATION(data);
    data->data_n = b64de_n((char *)data->data);
    data->data = b64de(data->data);
    u64 r, x, FILL_N; u8 keyindex;
    for(r = 0; r < NUMBER_OF_ROUNDS; ++r) {
        for(x = keyindex = 0; x < data->data_n; ++x, ++keyindex) {
            data->data[x] = data->data[x] ^ key[NUMBER_OF_ROUNDS - r - 1][keyindex];
        }
    }

    u8 *temp;
    if(data->data[data->data_n - 1] && data->data[data->data_n - 2] == 255) {
        data->data_n = data->data_n - FILL_DATA_N;
        temp = (u8 *)malloc(data->data_n);
        memcpy(temp, data->data, data->data_n);
        data->data = temp; temp = NULL;
    } else {
        FILL_N = data->data[data->data_n - 1];
        u64 real_n = data->data_n - FILL_N;
        temp = (u8 *)malloc(real_n);
        memcpy(temp, data->data, real_n);
        data->data = temp; temp = NULL;
        data->data_n = real_n;
    }
    return 0;
}
#endif


