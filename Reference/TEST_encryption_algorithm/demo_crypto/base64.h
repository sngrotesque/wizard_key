#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint8_t *b64en(const uint8_t *str, uint64_t str_len);
uint8_t *b64de(const uint8_t *code);

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