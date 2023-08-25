### 在Ming-w64使用内存对齐
1. https://sourceforge.net/projects/mingw-w64/
2. https://stackoverflow.com/questions/13131893/c-attribute-aligned-meaning
3. https://sourceforge.net/p/mingw-w64/wiki2/Missing%20_aligned_malloc/

ming-w64 是一个为 Windows 64 位和 32 位操作系统提供 gcc 运行环境的项目1。
它支持 attribute((aligned(n))) 这种语法，用来指定变量的对齐宽度2。
你可以在变量定义时加上这个属性，例如：

这样就可以让结构体内的成员首地址是自身大小的整数倍，也就是按照 8 字节对齐。
如果你想使用 _aligned_malloc 和 _aligned_free 这些函数来分配和释放对齐的内存，
你需要按照一定的顺序包含头文件3，例如：

```c
#include <stdlib.h>
#include <intrin.h>
#include <malloc.h>
#include <windows.h>

struct foo {
    int a;
    char b;
} __attribute__((aligned(8)));
```

### 为什么需要内存对齐，如何对齐
1. https://cloud.tencent.com/developer/article/1703257
2. https://blog.csdn.net/little_rookie__/article/details/112060171
3. https://zhuanlan.zhihu.com/p/30007037
4. https://blog.csdn.net/weixin_39918693/article/details/80709939

C语言编写的程序做到了内存对齐，可以提高程序的运行效率和稳定性，但也可能增加内存的占用。
你可以采用以下的方法来确保内存对齐：

使用 #pragma pack(n) 宏来指定结构体的对齐宽度，n 只能是 1, 2, 4, 8 或 16 的整数，表示按照 n 字节对齐。
这个宏可以放在结构体定义之前或之后，也可以用 #pragma pack() 来恢复默认的对齐规则1。

遵循结构体的默认对齐规则，即第一个成员的首地址为0，每个成员的首地址是自身大小的整数倍，
结构体的总大小是其成员中最大类型的整数倍2。
在定义变量时，使用 attribute((aligned(n))) 来指定变量的对齐宽度，n 只能是 2 的幂次方3。
