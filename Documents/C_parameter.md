下面是一个C语言的可变参数列表示例，其中函数print()接受一个整数num和任意数量的整数参数。
函数内部使用va_list类型的变量ap来访问可变参数列表。
在循环中，每次使用va_arg()宏获取下一个整数参数，并输出。
最后，在函数结束时使用va_end()宏结束可变参数列表的访问。

```c
#include <stdio.h>
#include <stdarg.h>

double average(int num, ...)
{
    va_list ap;
    int i;
    double sum = 0.0;

    va_start(ap, num); /* 初始化可变参数列表 */
    for (i = 0; i < num; i++) {
        sum += va_arg(ap, int);
    }
    va_end(ap); /* 结束可变参数列表 */

    return sum / num;
}

int main()
{
    printf("Average of 2, 3, 4, 5 = %f\n", average(4, 2, 3, 4, 5));
    printf("Average of 5, 10, 15 = %f\n", average(3, 5, 10, 15));
}
```