#ifndef _BASE64_H
#define _BASE64_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
typedef unsigned char Byte;
Byte base64_table[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

Byte *b64en(const Byte *str, long str_len)
{
    long len;
    Byte *res;
    int  i, j;

    (str_len % 3 == 0) ? (len = str_len / 3 * 4) : (len = (str_len / 3 + 1) * 4);

    res = (Byte *)malloc(sizeof(Byte) * len + 1);
    res[len] = '\0';

    //以3个8位字符为一组进行编码
    for (i = 0, j = 0; i < len - 2; j += 3, i += 4) {
        res[i]     = base64_table[str[j] >> 2];                                  //取出第一个字符的前6位并找出对应的结果字符
        res[i + 1] = base64_table[(str[j] & 0x3)     << 4 | (str[j + 1] >> 4)];  //将第一个字符的后位与第二个字符的前4位进行组合并找到对应的结果字符
        res[i + 2] = base64_table[(str[j + 1] & 0xf) << 2 | (str[j + 2] >> 6)];  //将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符
        res[i + 3] = base64_table[str[j + 2] & 0x3f];                            //取出第三个字符的后6位并找出结果字符
    }

    switch (str_len % 3) {
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

Byte *b64de(const Byte *code)
{
    //根据base64表，以字符找到对应的十进制数据
    int table[123] = {
        0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
        0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  62, 0,  0,  0,  63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0, 0,
        0, 0, 0, 0, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0,
        0, 0, 0, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
    };
    long len = strlen((char *)code);
    long str_len;
    Byte *res;
    int i, j;

    (strstr((char *)code, "==")) ? (str_len = len / 4 * 3 - 2) : (strstr((char *)code, "=")) ? (str_len = len / 4 * 3 - 1) : (str_len = len / 4 * 3);

    res = (Byte *)malloc(sizeof(Byte) * str_len + 1);
    res[str_len] = '\0';

    //以4个字符为一位进行解码
    for (i = 0, j = 0; i < len - 2; j += 3, i += 4) {
        res[j]     = ((Byte)table[code[i]]) << 2       | (((Byte)table[code[i + 1]]) >> 4);  //取出第一个字符对应base64表的十进制数的前6位与第二个字符对应base64表的十进制数的后2位进行组合
        res[j + 1] = (((Byte)table[code[i + 1]]) << 4) | (((Byte)table[code[i + 2]]) >> 2);  //取出第二个字符对应base64表的十进制数的后4位与第三个字符对应bas464表的十进制数的后4位进行组合
        res[j + 2] = (((Byte)table[code[i + 2]]) << 6) | ((Byte)table[code[i + 3]]);         //取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合
    }

    return res;
}