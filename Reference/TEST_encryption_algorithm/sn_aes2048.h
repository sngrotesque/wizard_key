// * 此文件已弃用，请使用'S2048.h' * //

///////////////////////////////////////////////////////////////////////////////
// 作者: SN-Grotesque
// 纲要:
//     1. 任何明文数据的字节长度必须是256的倍数，如明文数据字节长度不等于256的
//        倍数，将会使用单字节数据填充至256的倍数-1的长度，末尾值为填充长度，
//        如明文数据字节长度已为256的倍数则不会被填充。
//     2. 密钥长度必须为256字节，不能多也不能少，如果多了算法会将密钥清空并重
//        新填充单字节数据至256字节长度，如果少了算法会将密钥的数据重复填充直
//        至长度为256字节。
//     3. 加密轮数默认为16轮，可自定义。
//     4. 每轮加密会让密钥被算法刷新，以确保每轮加密使用的密钥均不相同。
//
///////////////////////////////////////////////////////////////////////////////
#include "base_include.h"

#ifndef SN_AES2048_OBJECT
#define SN_AES2048_OBJECT    1
#define FILL_BYTE_DATA       0x1F    // 填充明文数据时使用的单字节数据
#define FILL_BYTE_LENGTH     256     // 填充明文数据时需为此值的倍数
#define FILL_KEY_DATA        0x5E    // 超过256字节的密钥需重新填充的值
#define KEY_BYTE_LENGTH      256     // 密钥标准字节长度
#define NUMBER_OF_ROUNDS     16      // 加密或解密的轮值
#define ZERO_RETURN(x)       (0x00)  // 使值归零
#define ZERO_RETURN_PTR(x)   (NULL)  // 将指针指向NULL

typedef struct {
    uint8_t *data;   // 用于加密或解密的数据
    uint64_t data_n; // 数据长度
    uint8_t *u_key;  // 用户密钥
    uint8_t  fill_n; // 填充长度
} sn_aes2048;
#endif

#ifndef __SN_FILL_AND_CONFUSION_MISC__
#define __SN_FILL_AND_CONFUSION_MISC__ 1
// * * 填充数据函数 * * //
static int FILL_DATA_AND_KEY(sn_aes2048 *data, int _mode)
{
    uint64_t mod, x;
    uint32_t key_len;
    switch(_mode) {
        case 0:  // * 填充数据长度 * //
            mod = data->data_n % FILL_BYTE_LENGTH, x = data->data_n;
            if(mod != 0) {
                data->fill_n = FILL_BYTE_LENGTH - mod;
                data->data_n = data->fill_n + data->data_n;
                uint8_t *temp = (uint8_t *)malloc(data->data_n);
                memcpy(temp, data->data, data->data_n - data->fill_n);
                for(; x < data->data_n - 1; ++x) {temp[x] = FILL_BYTE_DATA;}
                temp[data->data_n - 1] = data->fill_n;
                data->data = temp; ZERO_RETURN_PTR(temp);
            } break;
        case 1:  // * 填充密钥长度 * //
            key_len = strlen((char *)data->u_key);
            if (key_len > KEY_BYTE_LENGTH) {
                data->u_key = (uint8_t *)malloc(KEY_BYTE_LENGTH);
                for(int x = 0; x < KEY_BYTE_LENGTH; ++x) data->u_key[x] = FILL_KEY_DATA;
            } else {
                for(int x = key_len, index = 0; x < KEY_BYTE_LENGTH; ++x, ++index) {
                    if (index == key_len) {ZERO_RETURN(index);}
                    data->u_key[x] = data->u_key[index];
                }
            } break;
        case 2:  // * 填充数据和密钥长度 * //
            mod = data->data_n % FILL_BYTE_LENGTH, x = data->data_n;
            if(mod != 0) {
                data->fill_n = FILL_BYTE_LENGTH - mod;
                data->data_n = data->fill_n + data->data_n;
                uint8_t *temp = (uint8_t *)malloc(data->data_n+data->fill_n);
                memcpy(temp, data->data, data->data_n - data->fill_n);
                for(; x < data->data_n - 1; ++x) {temp[x] = FILL_BYTE_DATA;}
                temp[data->data_n - 1] = data->fill_n;
                data->data = temp; ZERO_RETURN_PTR(temp);
            }
            key_len = strlen((char *)data->u_key);
            if (key_len > KEY_BYTE_LENGTH) {
                data->u_key = (uint8_t *)malloc(KEY_BYTE_LENGTH);
                for(int x = 0; x < KEY_BYTE_LENGTH; ++x) data->u_key[x] = FILL_KEY_DATA;
            } else {
                for(int x = key_len, index = 0; x < KEY_BYTE_LENGTH; ++x, ++index) {
                    if (index == key_len) {ZERO_RETURN(index);}
                    data->u_key[x] = data->u_key[index];
                }
            } break;
        default:  // * 错误输入 * //
            return -1;
    }
    return 0;
}

// * 更换密钥值，用于每轮加密 * //
static uint8_t *KEY_CONFUSION(uint32_t loop_value, uint8_t *key_data)
{
    uint8_t *key_temp = (uint8_t *)malloc(KEY_BYTE_LENGTH);
    for(uint32_t x = 0; x < KEY_BYTE_LENGTH; ++x) {
        switch(x) {
            case 0:
                key_temp[x] = (x ^ ((key_data[x] ^ key_data[255]) ^ key_data[174])) - loop_value;
                break;
            default:
                key_temp[x] = (x ^ ((key_data[x] ^ key_data[x - 1]) ^ key_data[174])) - loop_value;
                break;
        }
    } return key_temp;
}

// * 将每轮密钥的值保存为一个个一级指针，总体为一个二级指针 * //
static uint8_t **KEY_ANALYSIS(sn_aes2048 *data)
{
    FILL_DATA_AND_KEY(data, 1);
    uint8_t **key_data = (uint8_t **)malloc(NUMBER_OF_ROUNDS);
    for(uint32_t x = 0; x < NUMBER_OF_ROUNDS; ++x) {
        key_data[x] = (uint8_t *)malloc(256);
        key_data[x] = data->u_key;
        data->u_key = KEY_CONFUSION(x, data->u_key);
    }
    return key_data;
}
#endif

#ifndef __SN_AES2048_ENCRYPT_AND_DECRYPT_FUNCTION__
#define __SN_AES2048_ENCRYPT_AND_DECRYPT_FUNCTION__ 1
// * 加密函数 * //
static int ENCRYPT_DATA(sn_aes2048 *data)
{
    FILL_DATA_AND_KEY(data, 2);
    uint64_t x, r;
    uint8_t keyindex;
    for(r = 0; r < NUMBER_OF_ROUNDS; ++r) {
        for(x = keyindex = 0; x < data->data_n; ++x, ++keyindex) {
            data->data[x] = data->data[x] ^ data->u_key[keyindex];
        }
        data->u_key = KEY_CONFUSION(r, data->u_key);
    }
    data->data = b64en(data->data, data->data_n);
    data->data_n = b64_n(data->data_n);
    return 0;
}

// * 对称解密函数 * //
static int DECRYPT_DATA(sn_aes2048 *data)
{
    uint8_t **key = KEY_ANALYSIS(data);
    uint64_t len = strlen((char *)data->data);
    if (strstr((char *)data->data, "==")) {
        data->data_n = len / 4 * 3 - 2;
    } else if (strstr((char *)data->data, "=")) {
        data->data_n = len / 4 * 3 - 1;
    } else {
        data->data_n = len / 4 * 3;
    }

    data->data = b64de(data->data);
    uint64_t r, x;
    uint8_t keyindex;
    for(r = 0; r < NUMBER_OF_ROUNDS; ++r) {
        for(x = keyindex = 0; x < data->data_n; ++x, ++keyindex) {
            data->data[x] = data->data[x] ^ key[NUMBER_OF_ROUNDS - r - 1][keyindex];
        }
    }

    data->fill_n = data->data[data->data_n - 1];
    // printf("填充长度: %d\n数据长度: %lu\n", data->fill_n, data->data_n);
    for(x = data->data_n - data->fill_n; x < data->data_n; ++x) {
        data->data[x] = '\x00';
    }

    return 0;
}
#endif









