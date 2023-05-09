#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
* 此文档写给以后会看这段代码的自己

* 此算法的想法是将明文加密为密文，但是与一般的加密算法不同的是
* 此算法将是伪非对称加密算法。

* 详细讲解为

* 加密算法
* 首先由使用者提供一个公钥
* 接下来算法将把所有明文加密为0xFF
* 解密只能用私钥
* 私钥的生成原理是由0xFF减去原明文对应位置的值

* 解密算法
* 由私钥不断的逆运算得到后续使用的轮密钥
* 然后与密文0xFF进行运算

* 其中，解密算法可以采取加减乘除等方式来复杂化密文的破解。

* 目前此算法仅停留在理论阶段
*/

// * Bytes * //
#define BLOCK_SIZE 32

static uint8_t *encrypt(uint8_t *data, uint8_t *key)
{
    uint8_t *c = (uint8_t *)malloc(BLOCK_SIZE);
    uint8_t *k = (uint8_t *)malloc(BLOCK_SIZE);
    uint8_t x;

    for(x = 0; x < BLOCK_SIZE; ++x) {
        c[x] = data[x] ^ key[x];
    }





    return c;
}












