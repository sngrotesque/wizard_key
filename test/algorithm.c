#include <snConf.h>

/* 递归算法 *** 开始 ***************************/
int snAlgorithm_Recursion(snSize n)
{
    if(n < 4096) {
        return snAlgorithm_Recursion(n << 1);
    }
}
/* 递归算法 *** 结束 ***************************/

/* 杨辉三角 *** 开始 **************************/
#define YHT_height 10
#define YHT_width  ((2 * YHT_height) - 1)

void snAlgorithm_YangHuiTriangle()
{
    snSize array[YHT_height][YHT_width];
    sn_u32 i, j, temp;

    snMemoryZero(array, (YHT_width * YHT_height) * sizeof(snSize));
    array[0][0] = 1;

    for(i = 1; i < YHT_height; ++i) {
        for(j = 1; j <= i; ++j) {
            array[i][j] = array[i-1][j-1] + array[i-1][j];
        }
    }

    for(i = 1; i < YHT_height; ++i) {
        for(temp = i; temp <= (YHT_height - 1); ++temp) {
            printf(" ");
        }
        for(j = 1; j <= i; ++j) {
            printf("%3d", array[i][j]);
        }
        printf("\n");
    }
}
/* 杨辉三角 *** 结束 **************************/


int main()
{
    // YangHuiTriangle();
    // printf("%llu\n", snAlgorithm_Recursion(3));
}
