#include <snBigNum.h>

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


/*
* 代码问题总览
* E:\Projects\Shark_Coast>python run.py c -Ofast -lws2_32 --std=c18
* src1 size: 128
* src2 size: 93
* dst  size: 128
* 05 03 00 06 00 06 02 00 00 01 03 08 00 00 04 05 08 05 06 01 00 03 06 00 02 00 00 00 06 00 04 05
* 08 03 00 01 05 07 03 05 06 04 03 08 05 08 03 00 05 04 01 00 03 01 04 00 01 06 00 06 02 08 00 00
* 00 07 00 08 06 05 04 04 00 06 06 07 03 06 03 03 04 05 01 01 06 00 09 03 06 07 01 00 08 0a 04 07
* 08 01 09 07 04 08 02 08 04 03 09 00 08 04 02 09 01 00 08 03 00 09 02 01 08 03 09 02 01 03 08 09
* 
* n1:  98312938129038019248093482847918749835320948091840274891794274823741923012835284098047289479159382894087589240189342481349291837
* n2:                                     956442958023593362767651404875340683081293860940536175895944471064914473676396058349653304198
* res: 9831293812903801924809348284791874:801763906115433637660445680700082606104130145038583465375103854060002063016585400831002606035
* 
* 问题描述：
*     由于是习惯性的将n1作为最小的数来使用，导致如果n1比n2大，那么for(index = 0; index < _src2_size; ++index)条件中
*     代码执行就会出现意外结果。
* 
* 解决方案：
*     将src1_size与src2_size变量改名为大的数长度与小的数长度，并且在开始执行前判断n1与n2哪个更大，大的赋值给大长度，
*     小的赋值给小长度。此for(index = 0; index < _src1_size; ++index)中的_src1_size就应改为小长度。而
*     for(index = 0; index < _src2_size; ++index)就应改为大长度。
*/
SN_PUBLIC(snError) snBigNum_add SN_OPEN_API
SN_FUNC_OF((snChar **dst, snChar *_src1, snChar *_src2))
{
    if(!dst || !_src1 || !_src2) {
        return snErr_ErrNullData;
    }

    snSize _src1_size = strlen(_src1); // 第一个数字数组的长度
    snSize _src2_size = strlen(_src2); // 第二个数字数组的长度
    snSize _dst_size = 0;              // dst的长度
    snSize index;                      // 下标
    snByte buf;                        // 用来暂时存储结果的缓冲区

    if(!_src1_size || !_src2_size) {
        // 如果任何一个数字的长度为0，那么也没有必要进行运算了。
        return snErr_ErrNullData;
    }

    // 如果数字1长度等于数字2长度
    if(_src1_size == _src2_size) {
        // 如果数字1的第一位数加数字2的第一位数大于9
        if((SN_BIG_NUM_TO_UINT(_src1[0]) + SN_BIG_NUM_TO_UINT(_src2[0])) > 9) {
            // dst的长度就要在这基础上加一
            _dst_size = _src1_size + 1;
        } else {
            // 否则不变（由于长度相同，所以赋值哪个长度都行）
            _dst_size = _src1_size;
        }
    } else {
        // 数字长度不相同，数字1长度如果大于数字2长度
        if(_src1_size > _src2_size) {
            // 将数字1长度赋值给dst度
            _dst_size = _src1_size;
        } else {
            // 否则将数字2长度赋值给dst长度
            _dst_size = _src2_size;
        }
    }

    // 为dst指针申请内存空间
    if(!((*dst) = (snChar *)malloc(_dst_size + 1))) {
        return snErr_Memory; // 如果申请失败
    }
    (*dst)[_dst_size] = 0x00;

    // 将数字1数组反转
    snBigNum_ArrayReversal(_src1, _src1_size);
    // 将数字2数组反转
    snBigNum_ArrayReversal(_src2, _src2_size);

    // 将数字1数组赋值到dst指针中
    for(index = 0; index < _src1_size; ++index) {
        (*dst)[index] = SN_BIG_NUM_TO_UINT(_src1[index]);
    }
    // 将数字2数组与数字1数组的单位数相加
    for(index = 0; index < _src2_size; ++index) {
        buf = (*dst)[index] + SN_BIG_NUM_TO_UINT(_src2[index]);
        if(buf > 9) {
            // 如果大于9，就逢十进一。
            (*dst)[index] = buf % 10;
            (*dst)[index + 1] += 1;
        } else if(buf == 9) {
            (*dst)[index] = 0;
            (*dst)[index + 1] += 1;
        } else {
            (*dst)[index] = buf;
        }

        if(index == 93) {
            printf("test: %02x\n", (*dst)[index]);
            printf("test: %02x\n", buf);
        }
    }

    printf("src1 size: %llu\n", _src1_size);
    printf("src2 size: %llu\n", _src2_size);
    printf("dst  size: %llu\n", _dst_size);

    snMisc_PRINT((snByte *)(*dst), _dst_size, 32, 1, 0);

    // 将dst数组的数字转回字符
    for(index = 0; index < _dst_size; ++index) {
        (*dst)[index] = SN_BIG_NUM_TO_CHAR((*dst)[index]);
    }

    // 反转结果数组
    snBigNum_ArrayReversal((*dst), _dst_size);
    // 将数字1数组反转（不再次反转的话，函数外的数组会是反回来的）
    snBigNum_ArrayReversal(_src1, _src1_size);
    // 将数字2数组反转（不再次反转的话，函数外的数组会是反回来的）
    snBigNum_ArrayReversal(_src2, _src2_size);

    return snErr_OK;
}
