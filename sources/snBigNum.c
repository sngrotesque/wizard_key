#include <snBigNum.h>

/*
在C语言中实现大数加法函数，可以使用char数组来实现加法运算，解决精度的问题。
下面是一个简单的例子：

#include <stdio.h>
#include <string.h>

#define MAX 1000

char str1[MAX], str2[MAX], result[MAX];

void reverse(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void sum(char* str1, char* str2) {
    int carry = 0;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int maxLen = len1 > len2 ? len1 : len2;
    for (int i = 0; i < maxLen; i++) {
        int a = i < len1 ? str1[i] - '0' : 0;
        int b = i < len2 ? str2[i] - '0' : 0;
        result[i] = (a + b + carry) % 10 + '0';
        carry = (a + b + carry) / 10;
    }
    if (carry > 0) {
        result[maxLen++] = carry + '0';
    }
    result[maxLen] = '\0';
}

int main() {
    scanf("%s%s", str1, str2);
    reverse(str1);
    reverse(str2);
    sum(str1, str2);
    reverse(result);
    printf("%s\n", result);
}
复制
这个例子中，我们定义了一个sum()函数，它接受两个字符串作为参数，并返回它们的和。
我们首先将两个字符串反转，然后从最低位开始遍历它们，并将每一位相加。
如果有进位，则将其添加到下一位中。最后，我们将结果反转回来并输出它。


如果你不知道传入的数字字符串参数和结果的长度是多少，你可以使用动态内存分配来解决这个问题。
在C语言中，可以使用malloc()函数来动态分配内存。这个函数接受一个参数，即要分配的字节数。
例如，如果你想分配一个长度为1000的字符数组，
你可以这样做：char* str = (char*)malloc(1000 * sizeof(char));。
这将分配一个长度为1000的字符数组，并返回一个指向该数组的指针。

在你的大数加法函数中，你可以使用动态内存分配来分配足够的空间来存储结果。
例如，你可以这样做：

char* sum(char* str1, char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int maxLen = len1 > len2 ? len1 : len2;
    char* result = (char*)malloc((maxLen + 1) * sizeof(char));
    // ...
    return result;
}
复制
在这个例子中，我们使用了malloc()函数来分配足够的空间来存储结果。
我们首先计算出两个字符串中最长的那个，并将其存储在maxLen变量中。
然后，我们使用malloc()函数来分配一个长度为maxLen + 1的字符数组，并将其存储在result指针中。
最后，我们返回指向该数组的指针。
*/

SN_PRIVATE(snError) snBigNum_ArrayReversal
SN_FUNC_OF((snChar *number_string, snSize size))
{
    if(!number_string || !size) {
        return snErr_ErrNullData;
    }
    static snSize i;
    static snByte swap;

    for(i = 0; i < size >> 1; ++i) {
        swap = number_string[i];
        number_string[i] = number_string[size - i - 1];
        number_string[size - i - 1] = swap;
    }

    return snErr_OK;
}

SN_PUBLIC(snError) snBigNum_add SN_OPEN_API
SN_FUNC_OF((snChar **dst, snChar *src1, snChar *src2))
{
    if(!dst || !src1 || !src2) {
        return snErr_ErrNullData;
    }

    snSize src1Size = strlen(src1);
    snSize src2Size = strlen(src2);
    if(!src1Size || !src2Size) {
        return snErr_ErrNullData;
    }

    // snSize *smallerSize = snNull; // 小的数的长度的指针
    // snSize *largerSize = snNull; // 大的数的长度的指针
    // snChar *smaller = snNull; // 小的数的指针
    // snChar *larger = snNull; // 大的数的指针
    // snSize dstSize = 0; // dst的长度
    // snSize index; // 下标
    // snChar buf; // 用来暂时存储结果的缓冲区

    // if(src1Size > src2Size) {
    //     larger = src1;
    //     smaller = src2;
    //     *largerSize = src1Size;
    //     *smallerSize = src2Size;
    // } else {
    //     larger = src2;
    //     smaller = src1;
    //     *largerSize = src2Size;
    //     *smallerSize = src1Size;
    // }

    // snBigNum_ArrayReversal(larger, *largerSize);
    // snBigNum_ArrayReversal(smaller, *smallerSize);

    return snErr_OK;
}

SN_PUBLIC(snError) snBigNum_sub SN_OPEN_API
SN_FUNC_OF((snChar **dst, snChar *src1, snChar *src2))
{
    return snErr_OK;
}

SN_PUBLIC(snError) snBigNum_mul SN_OPEN_API
SN_FUNC_OF((snChar **dst, snChar *src1, snChar *src2))
{
    return snErr_OK;
}

SN_PUBLIC(snError) snBigNum_div SN_OPEN_API
SN_FUNC_OF((snChar **dst, snChar *src1, snChar *src2))
{
    return snErr_OK;
}

SN_PUBLIC(snError) snBigNum_mod SN_OPEN_API
SN_FUNC_OF((snChar **dst, snChar *src1, snChar *src2))
{
    return snErr_OK;
}

SN_PUBLIC(snError) snBigNum_pow SN_OPEN_API
SN_FUNC_OF((snChar **dst, snChar *src1, snChar *src2))
{
    return snErr_OK;
}
