#ifndef WMKC
#define WMKC // Wizard Magic Key Cyber（网络巫师的魔法钥匙）

#include <stdio.h>    // 标准输入输出库
#include <string.h>   // 标准字符串库
#include <stdint.h>   // 标准数字类型库
#include <signal.h>   // 信号库
#include <stdbool.h>  // 标准布尔值库
#include <inttypes.h> // 用于在跨平台打印同样的数据类型

#if __cplusplus > 199711L
#define register
#endif

// 判断是否是64位系统，不是的话，不进行支持。
#if (__SIZEOF_SIZE_T__ == 8) || defined(__x86_64__) || defined(__LP64__)
//  如果是Linux或Windows操作系统，那么支持。否则不支持。
#if defined(__linux) || defined(__linux__)
#   define WMKC_LINUX_SUPPORT
#   define WMKC_SUPPORT true
#elif defined(_WIN32) || defined(_WIN64)
#   define WMKC_WINDOWS_SUPPORT
#   define WMKC_SUPPORT true
#else
#   define WMKC_SUPPORT false
#endif // #if defined(__linux) || defined(__linux__)

// 定义版本号
#define WMKC_VERSION     "v5.0.0"
#define WMKC_VERNUM       0x500
#define WMKC_VER_MAJOR    5
#define WMKC_VER_MINOR    0
#define WMKC_VER_REVISION 0

// 定义一些一般类型
#ifndef WMKC_TYPE_DEFINED
#define WMKC_TYPE_DEFINED
#define WMKC_OPEN_API             // 公共函数请加上此宏用于标识
#define WMKC_PRIVATE_INLINE(type) static inline type // 静态内联函数
#define WMKC_PRIVATE_CONST(type)  static const  type // 静态常量
#define WMKC_PRIVATE(type)        static type // 静态函数（私有函数）
#define WMKC_PUBLIC(type)         type        // 动态函数（公共函数）
#define WMKC_OF(args)             args        // 函数取消调用消耗
#define wmkcNull                  NULL        // 空指针
#define wmkcFast                  register    // 寄存器类型
typedef const char *              wmkcString; // 固定字符串类型
typedef char                      wmkcChar;   // 字符类型
typedef uint8_t                   wmkcByte;   // 字节类型
typedef double                    wmkcFloat;  // 浮点数类型
typedef int64_t                   wmkcSSize;  // 长整数类型
typedef size_t                    wmkcSize;   // 长度类型
typedef void                      wmkcVoid;   // 空类型
typedef bool                      wmkcBool;   // 布尔类型
typedef FILE                      wmkcFile;   // 文件类型
typedef int16_t                   wmkc_s16;   // 宽字节类型
typedef int32_t                   wmkc_s32;   // 整数类型
typedef uint16_t                  wmkc_u16;   // 无符号宽字节类型
typedef uint32_t                  wmkc_u32;   // 无符号整数类型
#endif // #ifndef WMKC_TYPE_DEFINED

// 定义宏函数
/***字节交换：0x91 -> 0x19 ****************************/
#define wmkcSwapByte(x) ((((x) & 0xf) << 4) ^ ((x) >> 4))
/***宽字交换：0x91ba -> 0xab19 ********************************************/
#define wmkcSwapWord(x) ((wmkcSwapByte((x) & 0xff) << 8) ^ wmkcSwapByte((x) >> 8))
/***长字交换：0x91ba4951 -> 0x1594ab19 ***************************************/
#define wmkcSwapLong(x) ((wmkcSwapWord(x & 0xffff) << 16) ^ (wmkcSwapWord(x >> 16)))
/***内存空间申请 ********************************************/
#define wmkcMemoryNew(type, x, size) (x = (type)malloc((size)))
/***内存空间释放 **************************/
#define wmkcMemoryFree(x) free(x); x = wmkcNull
/***内存内容初始化为零 **************************/
#define wmkcMemoryZero(x, s) memset((x), 0x00, (s))

// 定义错误类型
#ifndef WMKC_ERROR_CODE
#define WMKC_ERROR_CODE
// 用于返回错误信息的数据类型
typedef struct {
    wmkcString message;
    wmkcSSize code;
} wmkcErr_ctx;

// 表示一切正常，不需要注意任何错误。
#define wmkcErr_OK                0ULL
// 一般的错误，用于表示64位的错误代码。
#define wmkcErr_Err64            -1ULL
// 一般的错误，用于表示32位的错误代码。
#define wmkcErr_Err32            -1U
// 错误的空数据，表示不应该传入一个空的数据（包含指针）。
#define wmkcErr_ErrNULL          -2ULL
// 错误的类型，表示传入的参数和函数要求的参数类型不一致。
#define wmkcErr_ErrType          -3ULL
// 错误的范围，表示传入的范围参数超过函数规定的最大范围。
#define wmkcErr_ErrOutRange      -4ULL
// 错误的范围，表示传入的范围参数中起始下标超过了末尾下标。
#define wmkcErr_ErrInvalidRange  -5ULL
// 错误的内存，只能用作申请内存失败时。
#define wmkcErr_ErrMemory        -6ULL
// 解析域名时出现了错误，只能用作需要解析域名的函数中。
#define wmkcErr_NetDomainResolv   1ULL
// 为套接字赋值时出现错误，只能用作未能成功且正确的使用socket函数时。
#define wmkcErr_NetSocket         2ULL
// 套接字连接失败，只能用作未能成功且正确的使用connect函数时。
#define wmkcErr_NetConnect        5ULL
// 套接字绑定失败，只能用作未能成功且正确的使用bind函数时。
#define wmkcErr_NetBind           6ULL
// 套接字监听失败，只能用作未能成功且正确的使用listen函数时。
#define wmkcErr_NetListen         7ULL
// 套接字接受连接失败，只能用作未能成功且正确的使用accept函数时。
#define wmkcErr_NetAccept         8ULL
// 错误的套接字类型，只能用作：函数要求是TCP但传入的是UDP套接字或相反。
#define wmkcErr_NetSockfdType     9ULL
// 套接字发送数据失败，只能用作未能成功且正确的使用send函数时。
#define wmkcErr_NetSend           10ULL
// 套接字接收数据失败，只能用作未能成功且正确的使用recv函数时。
#define wmkcErr_NetRecv           11ULL
// 套接字接收数据失败，用作自己实现的关闭套接字的函数中。
#define wmkcErr_NetClose          12ULL
// 表示启动调用WSAStartup函数时出现错误。
#define wmkcErr_NetWsaData        13ULL
// 错误的网络家族，应使用AF_INET（PF_INET）或AF_INET6（PF_INET6）
#define wmkcErr_NetFamily         14ULL
// 表示启动调用setsockopt函数时出现错误。
#define wmkcErr_NetSetSockOpt     15ULL
// 错误的路径，此错误类型表示用户指定的路径没有文件或（和）文件夹。
#define wmkcErr_FileFolderPath    32ULL
// 此值表示用户不应该读取一个空文件或不应该写入一个空的数据到文件。
#define wmkcErr_FileNull          33ULL
// 此值表示调用fopen函数或_wfopen来打开文件失败了。
#define wmkcErr_FileOpen          34ULL
// 此值表示调用fclose函数来关闭文件失败了。
#define wmkcErr_FileClose         35ULL
// 将wmkcErr对象赋值并作为返回值返回。
#define wmkcErr_return(error, _code, _message) \
    error.message = _message; error.code = _code; return error
#endif // #ifndef WMKC_ERROR_CODE

// 定义功能
#define WMKC_ENABLE_FEATURES
#if defined(WMKC_ENABLE_FEATURES)
#   define WMKC_MEMORY
#   define WMKC_COLOR
#   if defined(WMKC_MEMORY)
#       include <stdlib.h>
#       include <malloc.h>
#   endif
#   if defined(WMKC_COLOR)
#       define WMKC_ALL_COLOR_RESET         "\x1b[0m"  // 重置所有颜色
#       define WMKC_ALL_COLOR_FLICKER       "\x1b[5m"  // 闪烁的字符
#       define WMKC_FORE_COLOR_BLACK        "\x1b[30m" // 黑色（前景）
#       define WMKC_FORE_COLOR_RED          "\x1b[31m" // 红色（前景）
#       define WMKC_FORE_COLOR_GREEN        "\x1b[32m" // 绿色（前景）
#       define WMKC_FORE_COLOR_YELLOW       "\x1b[33m" // 黄色（前景）
#       define WMKC_FORE_COLOR_BLUE         "\x1b[34m" // 蓝色（前景）
#       define WMKC_FORE_COLOR_MAGENTA      "\x1b[35m" // 紫色（前景）
#       define WMKC_FORE_COLOR_CYAN         "\x1b[36m" // 青色（前景）
#       define WMKC_FORE_COLOR_WHITE        "\x1b[37m" // 白色（前景）
#       define WMKC_FORE_COLOR_LIGHTBLACK   "\x1b[90m" // 亮黑色（前景）
#       define WMKC_FORE_COLOR_LIGHTRED     "\x1b[91m" // 亮红色（前景）
#       define WMKC_FORE_COLOR_LIGHTGREEN   "\x1b[92m" // 亮绿色（前景）
#       define WMKC_FORE_COLOR_LIGHTYELLOW  "\x1b[93m" // 亮黄色（前景）
#       define WMKC_FORE_COLOR_LIGHTBLUE    "\x1b[94m" // 亮蓝色（前景）
#       define WMKC_FORE_COLOR_LIGHTMAGENTA "\x1b[95m" // 亮紫色（前景）
#       define WMKC_FORE_COLOR_LIGHTCYAN    "\x1b[96m" // 亮青色（前景）
#       define WMKC_FORE_COLOR_LIGHTWHITE   "\x1b[97m" // 亮白色（前景）
#       define WMKC_BACK_COLOR_RED          "\x1b[41m" // 红色（背景）
#       define WMKC_BACK_COLOR_GREEN        "\x1b[42m" // 绿色（背景）
#       define WMKC_BACK_COLOR_YELLOW       "\x1b[43m" // 黄色（背景）
#       define WMKC_BACK_COLOR_BLUE         "\x1b[44m" // 蓝色（背景）
#       define WMKC_BACK_COLOR_MAGENTA      "\x1b[45m" // 紫色（背景）
#       define WMKC_BACK_COLOR_CYAN         "\x1b[46m" // 青色（背景）
#       define WMKC_BACK_COLOR_WHITE        "\x1b[47m" // 白色（背景）
#       define WMKC_BACK_COLOR_LIGHTBLACK   "\x1b[100m" // 亮黑色（背景）
#       define WMKC_BACK_COLOR_LIGHTRED     "\x1b[101m" // 亮红色（背景）
#       define WMKC_BACK_COLOR_LIGHTGREEN   "\x1b[102m" // 亮绿色（背景）
#       define WMKC_BACK_COLOR_LIGHTYELLOW  "\x1b[103m" // 亮黄色（背景）
#       define WMKC_BACK_COLOR_LIGHTBLUE    "\x1b[104m" // 亮蓝色（背景）
#       define WMKC_BACK_COLOR_LIGHTMAGENTA "\x1b[105m" // 亮紫色（背景）
#       define WMKC_BACK_COLOR_LIGHTCYAN    "\x1b[106m" // 亮青色（背景）
#       define WMKC_BACK_COLOR_LIGHTWHITE   "\x1b[107m" // 亮白色（背景）
#       define WMKC_SET_TEXT_COLOR(color, text) color text WMKC_ALL_COLOR_RESET
#   endif // #if defined(WMKC_COLOR)
#endif // #if defined(WMKC_ENABLE_FEATURES)
#else
#   define WMKC_SUPPORT false
#endif
#endif // #ifndef WMKC_CONF__
