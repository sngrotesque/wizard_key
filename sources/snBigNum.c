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

SN_PUBLIC(snError) snBigNum_add SN_OPEN_API
SN_FUNC_OF((snChar **dst, snChar *_src1, snChar *_src2))
{
    if(!dst || !_src1 || !_src2) {
        return snErr_ErrNullData;
    }

    snSize _src1_size = strlen(_src1); // 第一个数字数组的长度
    snSize _src2_size = strlen(_src2); // 第二个数字数组的长度
    snSize *smallerSize = snNull;      // 小的数的长度的指针
    snSize *largerSize = snNull;       // 大的数的长度的指针
    snChar *smaller = snNull;          // 小的数的指针
    snChar *larger = snNull;           // 大的数的指针
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
    snZeroObject((*dst), _dst_size + 1);

    // 将数字1数组反转
    snBigNum_ArrayReversal(_src1, _src1_size);
    // 将数字2数组反转
    snBigNum_ArrayReversal(_src2, _src2_size);

    if(_src1_size < _src2_size) {
        // 将数字1数组赋值到dst指针中
        for(index = 0; index < _src1_size; ++index) {
            SN_BIG_NUM_S_N(dst, _src1, index);
        }
        // 将数字2数组与数字1数组的单位数相加
        for(index = 0; index < _src2_size; ++index) {
            SN_BIG_NUM_L_N(buf, dst, _src2, index);
        }
    } else {
        // 将数字2数组与数字1数组的单位数相加
        for(index = 0; index < _src2_size; ++index) {
            SN_BIG_NUM_S_N(dst, _src2, index);
        }
        // 将数字1数组赋值到dst指针中
        for(index = 0; index < _src1_size; ++index) {
            SN_BIG_NUM_L_N(buf, dst, _src1, index);
        }
    }

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
