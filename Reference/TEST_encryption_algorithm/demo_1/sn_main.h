#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

uint8_t *encrypt(const uint8_t *p, uint64_t _n);
uint8_t *decrypt(const uint8_t *c, uint64_t _n);
uint8_t *b64en(const uint8_t *str, uint64_t str_len);
uint8_t *b64de(const uint8_t *code);
char *slwr(const char *str);
char *supr(const char *str);
int to_int(int c);
uint8_t *stoh(const uint8_t *s, uint64_t _n);
uint8_t *htos(const char *str);
uint64_t numlen(uint64_t num);
char *dtob(uint64_t num);

// 加密算法函数开始
#define E_XoR(x, y) (0x1f + (x ^ y) ^ (50 - y ^ y)) - 0xc4
#define D_XoR(x, y) (x + 0xc4 ^ (50 - y ^ y)) - 0x1f ^ y
#define E_XoRL(x, y) (x ^ y)

const uint8_t key[256] = {
    0x59, 0x71, 0x68, 0x80, 0x9b, 0x35, 0xa9, 0x0f, 0xcf, 0x50, 0x48, 0x6e, 0x8f, 0xe4, 0xf3, 0x00,
    0x35, 0xae, 0x98, 0xc3, 0xc1, 0x50, 0x65, 0xfb, 0x09, 0x1f, 0xbc, 0x21, 0x21, 0x90, 0x56, 0xe3,
    0x42, 0x12, 0xee, 0x21, 0x0e, 0x32, 0xd6, 0x5a, 0xc7, 0x19, 0x20, 0x1d, 0x50, 0xd5, 0x1d, 0x61,
    0xa2, 0xc3, 0x71, 0x7f, 0x0c, 0x9e, 0xcd, 0x71, 0x45, 0x3a, 0xe5, 0x14, 0xd4, 0x26, 0x8c, 0x15,
    0x73, 0xd8, 0x90, 0x1e, 0x4d, 0xfd, 0x3f, 0xff, 0xfb, 0x29, 0xc8, 0x04, 0xcb, 0xcb, 0x34, 0xab,
    0x3f, 0x05, 0x38, 0x5b, 0x46, 0xea, 0x7f, 0xf3, 0x8e, 0x6a, 0x8d, 0x11, 0x75, 0x87, 0xd1, 0x31,
    0x9d, 0x3c, 0xfb, 0xdd, 0xb4, 0x22, 0xb5, 0x8d, 0xcf, 0x42, 0xda, 0x56, 0x75, 0x61, 0x86, 0xa7,
    0xe6, 0x51, 0x33, 0x9c, 0x46, 0x87, 0x89, 0xb3, 0x08, 0xd7, 0xb2, 0xf2, 0x09, 0x26, 0x8d, 0x9d,
    0xd4, 0x62, 0x65, 0xa3, 0x72, 0x0e, 0x66, 0x22, 0xfd, 0x45, 0x17, 0x92, 0x7e, 0xee, 0x58, 0xa9,
    0x15, 0xba, 0x8d, 0x23, 0xe9, 0xf1, 0xdd, 0xac, 0xf9, 0xb9, 0xc5, 0xf8, 0x67, 0xdc, 0xfc, 0x91,
    0xd6, 0xbf, 0x82, 0x04, 0x63, 0x2f, 0x36, 0xc4, 0xb4, 0xb0, 0xeb, 0x34, 0x5a, 0x86, 0x70, 0x61,
    0x8a, 0x65, 0x61, 0x0a, 0x4c, 0x9b, 0x35, 0xbb, 0xd9, 0x42, 0x9a, 0x73, 0x19, 0x14, 0x55, 0xae,
    0x53, 0xc9, 0x69, 0x62, 0xba, 0x81, 0x08, 0xe7, 0xf6, 0x54, 0x10, 0xae, 0x2f, 0x3d, 0x6b, 0x15,
    0xd6, 0xbf, 0x3f, 0x2b, 0x17, 0x34, 0xb2, 0x0a, 0xe6, 0xb6, 0xed, 0x33, 0xdb, 0x57, 0xba, 0x6f,
    0x9d, 0xed, 0xc2, 0x91, 0x3c, 0x42, 0xdb, 0x97, 0xd7, 0x48, 0x05, 0xe5, 0xa7, 0xc2, 0x88, 0xbd,
    0x71, 0xa4, 0x48, 0xc5, 0x46, 0x92, 0x1b, 0x38, 0xe9, 0x8b, 0xe9, 0x91, 0x4b, 0x15, 0x07, 0x20
};
const uint16_t keyL[256] = {
    3339, 5393, 9899, 1236, 5587, 4826, 6431, 2278, 3426, 1579, 8642, 2857, 5132, 5301, 8937, 7294,
    1814, 7837, 6576, 2030, 7232, 9439, 7481, 3283, 1384, 5185, 3147, 4119, 1667, 4470, 3113, 4223,
    6352, 8702, 9439, 1709, 5021, 6295, 9738, 7455, 1556, 3375, 5073, 3712, 1583, 2966, 2684, 7214,
    6385, 1460, 6947, 9241, 9535, 4864, 2170, 3813, 1491, 1872, 8088, 1222, 4246, 1357, 7037, 1556,
    5566, 4582, 3791, 3630, 5545, 1104, 1150, 1776, 2314, 3395, 2329, 8923, 2556, 5283, 4503, 4163,
    7197, 7283, 7606, 3288, 4504, 1316, 7733, 3792, 5630, 5831, 7388, 9184, 6193, 3737, 4215, 2460,
    9382, 8469, 6923, 7063, 5476, 3559, 6027, 3683, 7100, 7204, 3485, 1439, 5281, 5449, 2918, 4437,
    1831, 1858, 6091, 5034, 9380, 8264, 5488, 9067, 8933, 9317, 7014, 4405, 3905, 4422, 9026, 6471,
    6083, 6281, 1704, 6422, 8289, 3402, 8891, 7895, 1544, 2260, 9491, 5497, 9563, 5054, 5669, 8564,
    3636, 6554, 3815, 4835, 2017, 1267, 6240, 1235, 6589, 4005, 6053, 2081, 2657, 3865, 4622, 8627,
    9383, 3033, 1497, 3602, 6433, 3166, 1923, 7376, 7756, 9137, 6604, 4943, 2480, 5634, 2354, 3216,
    4324, 4126, 6476, 5513, 8208, 4651, 4097, 3717, 6756, 3801, 5553, 3176, 8527, 1364, 8245, 4795,
    2589, 6533, 4071, 2295, 5574, 3719, 2838, 9175, 3660, 3208, 8081, 1371, 6434, 2974, 9889, 7214,
    7149, 3356, 9588, 2749, 6082, 3036, 3590, 4949, 7731, 6753, 3088, 3176, 6253, 9622, 1410, 6340,
    7075, 9799, 8896, 1306, 5932, 3716, 9554, 9247, 3344, 2233, 9594, 1335, 8751, 8598, 5810, 6463,
    2285, 8853, 2274, 4524, 2388, 9346, 8686, 8627, 8989, 1875, 6098, 3827, 8828, 3803, 4859, 4839
};

inline uint8_t *encrypt(const uint8_t *p, uint64_t _n)
{
    uint64_t x, y, tmp;
    uint8_t t1, *t2 = (uint8_t *)malloc(4), *t3 = (uint8_t *)malloc(2);
    uint8_t *res = (uint8_t *)malloc(_n * 2);
    uint8_t index;
    for(x = y = index = 0; x < _n; ++x, y+=2, ++index) {
        t1 = E_XoR(p[x], key[index]);
        tmp = t1 ^ keyL[index];
        sprintf((char *)t2, "%lu", tmp);
        t3 = htos((char *)t2);
        res[y] = t3[0];
        res[y + 1] = t3[1];
    }
    return res;
}

// static uint8_t *encrypt(const uint8_t *p, uint64_t _n)
// {uint64_t x, index; uint8_t keyIndex = 0, *c = (uint8_t *)malloc(_n);
// for(x = 0; x < _n; x++, keyIndex++) {c[x] = E_XoR(p[x], key[keyIndex]);}return c;}

inline uint8_t *decrypt(const uint8_t *c, uint64_t _n)
{
    uint64_t x;
    uint8_t keyIndex = 0, *p = (uint8_t *)calloc(_n, 8);
    for(x = 0; x < _n; x++, keyIndex++) {
        p[x] = D_XoR(c[x], key[keyIndex]);
    }
    return p;
}
// 加密算法函数结束

// Base64编码函数开始
inline uint8_t *b64en(const uint8_t *str, uint64_t str_len)
{
    uint8_t base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    long len;
    uint8_t *res;
    int i, j;
    (str_len % 3 == 0) ? (len = str_len / 3 * 4) : (len = (str_len / 3 + 1) * 4);
    res = (uint8_t *)malloc(sizeof(uint8_t) * len + 1);
    res[len] = '\0';
    for (i = 0, j = 0; i < len - 2; j += 3, i += 4) {
        res[i]     = base64_table[str[j] >> 2];
        res[i + 1] = base64_table[(str[j] & 0x3) << 4     | (str[j + 1] >> 4)];
        res[i + 2] = base64_table[(str[j + 1] & 0xf) << 2 | (str[j + 2] >> 6)];
        res[i + 3] = base64_table[str[j + 2] & 0x3f];
    }
    switch (str_len % 3) {
        case 1: res[i - 2] = '='; res[i - 1] = '='; break;
        case 2: res[i - 1] = '='; break;
    }
    return res;
}

inline uint8_t *b64de(const uint8_t *code)
{
    int table[123] = {
        0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
        0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  62, 0,  0,  0,  63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0, 0,
        0, 0, 0, 0, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0,
        0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
    };
    long len = strlen((char *)code);
    long str_len;
    uint8_t *res;
    int i, j;
    (strstr((char *)code, "==")) ? (str_len = len / 4 * 3 - 2) : (strstr((char *)code, "=")) ? (str_len = len / 4 * 3 - 1) : (str_len = len / 4 * 3);
    res = (uint8_t *)malloc(sizeof(uint8_t) * str_len + 1);
    res[str_len] = '\0';
    for (i = 0, j = 0; i < len - 2; j += 3, i += 4) {
        res[j]     = ((uint8_t)table[code[i]])      << 2  | (((uint8_t)table[code[i + 1]]) >> 4);
        res[j + 1] = (((uint8_t)table[code[i + 1]]) << 4) | (((uint8_t)table[code[i + 2]]) >> 2);
        res[j + 2] = (((uint8_t)table[code[i + 2]]) << 6) | ((uint8_t)table[code[i + 3]]);
    }
    return res;
}
// Base64编码函数结束

// 处理字符串函数开始
inline char *slwr(const char *str)
{
    uint32_t x, len = strlen(str);
    char *res = (char *)calloc(len, 8);
    for(x = 0; x < len; ++x) {res[x] = str[x]; if (str[x] >= 0x41 && str[x] <= 0x5A) {res[x] += 0x20;}}
    return res;
}
inline char *supr(const char *str)
{
    uint32_t x, len = strlen(str);
    char *res = (char *)calloc(len, 8);
    for(x = 0; x < len; ++x) {res[x] = str[x]; if (str[x] >= 0x61 && str[x] <= 0x7A) {res[x] -= 0x20;}}
    return res;
}

inline int to_int(int c)
{
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    else if (c >= '0' && c <= '9')
        return c - '0';
    return -1;
}

inline uint8_t *stoh(const uint8_t *s, uint64_t _n)
{
    uint32_t x, y;
    char tmp[3];
    uint8_t *str = (uint8_t *)calloc(_n * 2, 8);
    for(x = 0, y = 0; x < _n; ++x, y += 2) {
        sprintf(tmp, "%02x", s[x]);
        str[y] = tmp[0];
        str[y+1] = tmp[1];
    }
    return str;
}

inline uint8_t *htos(const char *str)
{
    const char *hex_ascii = slwr(str);
    const uint32_t len = strlen(hex_ascii);
    if (len % 2 != 0) {return NULL;}
    long long i, j, x, top, bot;
    uint8_t *ch = (uint8_t *)calloc(len / 2, 8);
    for (i = j = x = 0; i < len; i += 2, x++) {
        top = to_int(hex_ascii[i]);
        bot = to_int(hex_ascii[i+1]);
        if (top == -1 || bot == -1) {
            printf("Non Hex!\n");
            return NULL;
        }
        ch[x] = (top << 4) + bot;
    }
    return ch;
}
// 处理字符串函数结束

// 处理数字函数开始
inline uint64_t numlen(uint64_t num)
{
    uint64_t len = 0;
    for(; num > 0; ++len) {
        num /= 10;
    }
    return len;
}

inline char *dtob(uint64_t num)
{
    if (num == 0) {return NULL;}
    uint64_t x, len, temp = num;
    for(len = 0; temp > 0; ++len) temp /= 2;
    char *str = (char *)calloc(len + 2, 8);
    for(x = 0; x < len + 2; ++x) {
        str[(len + 2 - 1) - x] = (num % 2) + 0x30;
        num /= 2;
    }
    str[0] = '0', str[1] = 'b';
    return str;
}
// 处理数字函数结束






