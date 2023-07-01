### 解析C语言声明的程序
```text
cdecl

用法：
    进入cdecl终端
    cdecl> explain (?)
```

### Linux平台创建自己的动静态库
```text
参考链接：https://blog.csdn.net/TABE_/article/details/114897323

注意事项
    g++无法使用c文件编写的动静态库
    gcc无法使用cpp文件编写的动静态库
    函数声明不要带'static'

创建动态库
    gcc test.c -fPIC -shared -o libtest.so

    使用
        gcc main.c -L{库路径} -l{库名} -o main
        例如
        gcc main.c -L. -ltest -o main

    -shared     该选项指定生成动态连接库
    -fPIC       表示编译为位置独立的代码
                不用此选项的话编译后的代码是位置相关的所以动态载入时
                是通过代码拷贝的方式来满足不同进程的需要，而不能达到
                真正代码段共享的目的。
    
    -L          表示要连接的库的目录地址
    -l          编译器查找动态连接库时有隐含的命名规则，即在给出的名
                字前面加上lib，后面加上.so来确定库的名称

创建静态库
    gcc -c test.c
    ar cr {库名} {.o文件}

    c           不在必须创建库的时候给出警告
    r           替换归档文件中已有的文件或加入新文件

    查看库内符号链接
        nm -s {静态库文件名}
    
    使用
        gcc main.c -l{库名} -L{路径} -o main
```

### C/C++知识点

```c
/********************************************
* 基本知识                                  *
*                                           *
* 所有整数类型的变量存储方式均为小端排序    *
*                                           *
* 运算符与优先级(从高到底)                  *
* [] 数组取下标                    优先级1  *
* () 圆括号                                 *
* .  成员选择(对象)                         *
* -> 成员选择(指针)                         *

* ~ 取反运算符                              *
* ^ 按位异或运算符                          *
* | 按位或运算符                            *
* & 按位与运算符                            *
* ! 逻辑非运算符                            *

* 1. 数组下标                               *
*    arrayName[index]                       *
*    index[arrayName]                       *
* 2. 符号替换                               *
*    <:    :>    <%    %>                   *
*    [     ]     {     }                    *
********************************************/

/********************************************
* 基本类型                                  *
********************************************/
bool               // (1 byte)      [0, 1]                      布尔型
unsigned           // (1 byte)      [0, 4294967295]             无符号型，单独使用等于unsigned int
char               // (1 byte)      [-128, 127]                 字符型
unsigned char      // (1 byte)      [0,  255]                   无符号字符型
short              // (2 bytes)     [-32768, 32767]             短整型
unsigned short     // (2 bytes)     [0, 65535]                  短整型
int                // (4 bytes)     [-2147483648, 2147483647]   整型
unsigned int       // (4 bytes)     [0, 4294967295]             整型
float              // (4 bytes)     [-3.4e38, 3.4e38]           单精度浮点型
long               // (4 bytes)     [-2147483648, 2147483647]   长整型【Windows】
long               // (8 bytes)     [~((2^64-1)/2), (2^64-1)/2] 长整型【Linux】
long long          // 等于上一个                                长长整型
unsigned long      // (4 bytes)     [0, 4294967295]             无符号长整型【Windows】
unsigned long      // (8 bytes)     [0, 2^64-1]                 无符号长整型【Linux】
unsigned long long // 等于上一个                                无符号长长整型
double             // (8 bytes)     [-1.79769313486231570e308, 1.79769313486231570e308] 双精度浮点型
*p                 // (8 bytes)                                 指针型
// long在Windows系统等于int

/********************************************
* 结构体                                    *
********************************************/
// 此类型为定义一个结构体类型的别名
typedef struct {
    char name[32];
    int age;
} s1;
s1 ctx;
ctx.age = 0;

// 此类型为定义一个结构体类型
// s2为结构体类型，s2_ctx_1与s2_ctx_2是结构体变量可直接使用
struct s2 {
    char name[32];
    int age;
} s2_ctx_1, s2_ctx_2;
struct s2 ctx;
ctx.age = 0;
s2_ctx_1.age = 0;

// 两种方式的结构体均可以包含其他结构体，且不需要特别申请一段内存（除非为指针，但是指针
// 也不需要再特别对内部的typedef struct型结构体申请一段内存）。

/********************************************
* 网络                                      *
********************************************/
#include <winsock.h> // Windows

// Linux - BEGIN
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
// Linux - END

// 【Windows】初始化WSADATA，确定WSADATA版本号
WSADATA ws;
WSAStartup(MAKEWORD(2, 2), &ws);

// 定义一个socket的方式，SOCKET为Windows，int为Linux
SOCKET / int basicSocket = socket(af, sock, flag);
SOCKET / int clientSocket;

// 定义sockaddr，存储目标IP，端口，以及使用各种socket family
struct sockaddr_in server_info;
struct sockaddr_in client_info;

server_info.sin_addr.S_un.S_addr = inet_addr(target_ip); // Windows
server_info.sin_addr.s_addr = inet_addr(target_ip) // Linux
server_info.sin_port = htons(target_port);
server_info.sin_family      = AF_INET;

// 连接服务端
connect(basicSocket, (struct sockaddr *)&server_info, sizeof(server_info));

// 设置socket的bind超时时间
setsockopt(basicSocket, SOL_SOCKET, SO_REUSEADDR, 1);

// 定义客户端sockaddr的大小
uint32_t clientSize = sizeof(client_info);

// 监听basicSocket，监听server_info中的IP与端口
bind(basicSocket, (struct sockaddr *)&server_info, sizeof(server_info));
listen(basicSocket, 5); // 设置最大连接数，此处为5
// 创建一个进程等待客户端连接，并将客户端IP端口存入client_info
clientSocket = accept(basicSocket, (struct sockaddr *)&client_info, &clientSize);

// TCP协议：发送/接收
send(basicSocket, buf, size, flag);
recv(basicSocket, buf, size, flag);

// UDP协议：发送/接收
recvfrom(basicSocket, rbuf, rbuflen, flag, (struct sockaddr *)&client_info, &clientSize);
sendto(basicSocket, sbuf, sbuflen, flag, (struct sockaddr *)&client_info, clientSize);

// 【Windows】关闭WSADATA，关闭socket
WSACleanup();
closesocket(basicSocket);

// 【Linux】关闭socket
close(basicSocket);
shutdown(basicSocket, 2);

/********************************************
* 指针                                      *
********************************************/
char *p;
char **ptr = &p;
printf("%p, %p", p, &p);
/*
* &p 为指针的地址；
* p  为指针指向的数据的地址
* ptr 指向 p 的地址，*ptr 指向为 p 指向的数据的地址
* &p   address: 0x7ffc1d738f38
* &ptr address: 0x7ffc1d738f30
* p    address: 0x559b8ab422a0
* ptr  address: 0x7ffc1d738f38
* *ptr address: 0x559b8ab422a0
*/

uint32_t arr[4] = {0x41, 0x42, 0x43, 0x44};
uint8_t *p[4] = {
    (uint8_t *)&arr[0],
    (uint8_t *)&arr[1],
    (uint8_t *)&arr[2],
    (uint8_t *)&arr[3]
};
/*
* ------- 行视图 -------
* p[0]:       0x7ffe60a0f120 | p[1]:       0x7ffe60a0f124 | p[2]:       0x7ffe60a0f128 | p[3]:       0x7ffe60a0f12c
* &p[0]:      0x7ffe60a0f100 | &p[1]:      0x7ffe60a0f108 | &p[2]:      0x7ffe60a0f110 | &p[3]:      0x7ffe60a0f118
* &((*p)[0]): 0x7ffe60a0f120 | &((*p)[1]): 0x7ffe60a0f121 | &((*p)[2]): 0x7ffe60a0f122 | &((*p)[3]): 0x7ffe60a0f123
* &(*p[0]):   0x7ffe60a0f120 | &(*p[1]):   0x7ffe60a0f124 | &(*p[2]):   0x7ffe60a0f128 | &(*p[3]):   0x7ffe60a0f12c
* ------- 列视图 -------
* p[0]:       0x7ffe60a0f120 | &p[0]:      0x7ffe60a0f100 | &((*p)[0]): 0x7ffe60a0f120 | &(*p[0]):   0x7ffe60a0f120
* p[1]:       0x7ffe60a0f124 | &p[1]:      0x7ffe60a0f108 | &((*p)[1]): 0x7ffe60a0f121 | &(*p[1]):   0x7ffe60a0f124
* p[2]:       0x7ffe60a0f128 | &p[2]:      0x7ffe60a0f110 | &((*p)[2]): 0x7ffe60a0f122 | &(*p[2]):   0x7ffe60a0f128
* p[3]:       0x7ffe60a0f12c | &p[3]:      0x7ffe60a0f118 | &((*p)[3]): 0x7ffe60a0f123 | &(*p[3]):   0x7ffe60a0f12c
*/

/* ============== 指针冷知识 ===============
* 不同函数中的同名指针指向的内存地址是一致的
*/

/********************************************
* 二维数组（本质还是一维数组）的输出方式：  *
********************************************/
array[(N * x) + y];
/*
* N 为array[N][*]，也就是此数组的单组横向占用内存大小
* x 为array[x][*]，也就是此数组的纵向下标
* y 为array[*][y]，也就是此数组的横向下标
*/

/********************************************
* 宏定义                                    *
********************************************/
#define UNIX      _unix  || _unix_  || __unix
#define Linux     _linux || _linux_ || __linux
#define Windows32 _WIN32 || __WIN32
#define Windows64 _WIN64 || __WIN64
#define IOS       __APPLE__
#define Android   __ANDROID__

#define GCC         __GNUC__
#define Visual_Cpp  _MSC_VER
#define Borland_Cpp __BORLANDC__

/********************************************
* 性能优化                                  *
********************************************/
// 单次赋值的性能大于多次赋值
// 两个变量交换时，引入第三变量性能更高

// 运算符并不影响性能（但偶数乘除法请用<< >>运算符）
// 异或运算符和加减运算符性能相同
```

[Win32 和 C++ 入门](https://learn.microsoft.com/zh-cn/windows/win32/learnwin32/learn-to-program-for-windows)<br>
[Win32 API 编程参考](https://learn.microsoft.com/zh-cn/windows/win32/api/)

##### Win API
```c
// 获取当前用户名
TCHAR strBuffer[256];
DWORD dwSize = 256;
GetUserName(strBuffer, &dwSize);
```
