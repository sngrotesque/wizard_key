/*
您好，这是Bing。我可以帮您了解二进制数据的循环位移操作。😊

二进制数据的循环位移操作是指将二进制数向左或向右循环地移动指定数量的位，并将被移出的位重新插入到另一端¹。
例如，将二进制数1101（十进制13）左移两次后得到0110（十进制6），再左移一次得到1001（十进制9），再左移一次得到0011（十进制3）。

循环位移操作可以用以下公式表示：

- 循环左移n位：`(val >> (N - n) | (val << n))`
- 循环右移n位：`(val << (N - n) | (val >> n))`

其中，`val`是要移位的数，`N`是`val`的位数，`n`是要移动的位数，`|`是按位或运算，`>>`是右移运算，`<<`是左移运算。

具体来说，您给出的例子可以用以下步骤实现：

- `00001010`左移三位后得到`01010000`：
    * 将`00001010`右移`8 - 3 = 5`位，得到`00000000`
    * 将`00001010`左移`3`位，得到`01010000`
    * 将两个结果按位或，得到`00000000 | 01010000 = 01010000`
- `11101101`左移三位后得到`01101111`：
    * 将`11101101`右移`8 - 3 = 5`位，得到`00000111`
    * 将`11101101`左移`3`位，得到`01101000`
    * 将两个结果按位或，得到`00000111 | 01101000 = 01101111`
- `01000000`左移三位后得到`00000100`：
    * 将`01000000`右移`8 - 3 = 5`位，得到`00000010`
    * 将`01000000`左移`3`位，得到`00000000`
    * 将两个结果按位或，得到`00000010 | 00000000 = 00000100`

希望这能回答您的问题，如果您还有其他问题，欢迎继续提问。😊

¹: 循环移位：循环左移和循环右移-CSDN博客

源: 与必应的对话， 2023/12/21
(1) 循环移位：循环左移和循环右移-CSDN博客. https://blog.csdn.net/LiuBo_01/article/details/80149708.
(2) 快速掌握左移＜＜、右移＞＞位运算及使用技巧 - 知乎. https://zhuanlan.zhihu.com/p/390085789.
(3) verilog移位操作符_百度文库. https://wenku.baidu.com/view/01915d7e51d380eb6294dd88d0d233d4b04e3f36.html.
*/
#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_FEA
#define WMKC_CPP_FEA
#include <wmkc_memory.hpp>
#include <wmkc_exception.hpp>

#define WMKC_FEA_NB 4
#define WMKC_FEA_NK 4
#define WMKC_FEA_NR 4

#define WMKC_FEA_BLOCKLEN 16

namespace wmkcCrypto {
    enum class FEA_XcryptMode {
        ECB, CBC, CTR, CFB
    };

    class wmkcFEA {
        private:
            wmkcVoid subBytes(wmkcByte *block);
            wmkcVoid shiftBits(wmkcByte *block);

            wmkcVoid invSubBytes(wmkcByte *block);
            wmkcVoid invShiftBits(wmkcByte *block);

            wmkcVoid shiftRows(wmkcByte *block);
            wmkcVoid invShiftRows(wmkcByte *block);

            wmkcVoid xorWithIV(wmkcByte *block, wmkcByte *iv);
            wmkcVoid keyExtension(wmkcByte *key, wmkcByte *iv);

            wmkcVoid cipher(wmkcByte *p, wmkcByte *roundKey);
            wmkcVoid invCipher(wmkcByte *c, wmkcByte *roundKey);

        public:
            wmkcByte key[WMKC_FEA_BLOCKLEN << 1];
            wmkcByte iv[WMKC_FEA_BLOCKLEN];
            wmkcByte nonce[WMKC_FEA_BLOCKLEN >> 1];
            wmkcByte roundKey[sizeof(key) * WMKC_FEA_NR]; // len(key) * WMKC_FEA_NR
            wmkc_u32 segmentSize;

            wmkcVoid ecb_encrypt(wmkcByte *p);
            wmkcVoid ecb_decrypt(wmkcByte *c);

            wmkcVoid cbc_encrypt(wmkcByte *p, wmkcSize n);
            wmkcVoid cbc_decrypt(wmkcByte *c, wmkcSize n);

            wmkcVoid ctr_xcrypt(wmkcByte *d, wmkcSize n);

            wmkcVoid cfb_encrypt(wmkcByte *p, wmkcSize n, wmkc_u32 segmentSize);
            wmkcVoid cfb_decrypt(wmkcByte *c, wmkcSize n, wmkc_u32 segmentSize);

            //////////////////////////////////////////////////////////////////

            wmkcFEA(const wmkcByte *key, const wmkcByte *iv, const wmkc_u32 segmentSize = 128);
            ~wmkcFEA();
            void encrypt(wmkcByte *content, wmkcSize size, FEA_XcryptMode mode);
            void decrypt(wmkcByte *content, wmkcSize size, FEA_XcryptMode mode);
    };
}

#endif
#endif
