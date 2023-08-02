#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_ERROR
#define WMKC_ERROR

typedef struct {
    wmkcCSTR message;
    wmkcSSize code;
} wmkcErr_obj;

#define wmkcErr_OK 0 // 表示一切正常，不需要注意任何错误。

#define wmkcErr_Err64           -1LL // 一般的错误，用于表示64位的错误代码。
#define wmkcErr_Err32           -1   // 一般的错误，用于表示32位的错误代码。
#define wmkcErr_ErrNULL         -2LL // 错误的空数据，表示不应该传入一个空的数据（包含指针）。
#define wmkcErr_ErrType         -3LL // 错误的类型，表示传入的参数和函数要求的参数类型不一致。
#define wmkcErr_ErrOutRange     -4LL // 错误的范围，表示传入的范围参数超过函数规定的最大范围。
#define wmkcErr_ErrInvalidRange -5LL // 错误的范围，表示传入的范围参数中起始下标超过了末尾下标。
#define wmkcErr_ErrSysFunc      -6LL // 错误的调用，表示调用系统函数后返回了错误代码。
#define wmkcErr_ErrMemory       -7LL // 错误的内存，只能用作申请内存失败时。
#define wmkcErr_NetDomainResolv  1LL // 解析域名时出现了错误，只能用作需要解析域名的函数中。
#define wmkcErr_NetSocket  2LL // 为套接字赋值时出现错误，只能用作未能成功且正确的使用socket函数时。
#define wmkcErr_NetConnect 5LL // 套接字连接失败，只能用作未能成功且正确的使用connect函数时。
#define wmkcErr_NetBind    6LL // 套接字绑定失败，只能用作未能成功且正确的使用bind函数时。
#define wmkcErr_NetListen  7LL // 套接字监听失败，只能用作未能成功且正确的使用listen函数时。
#define wmkcErr_NetAccept  8LL // 套接字接受连接失败，只能用作未能成功且正确的使用accept函数时。
#define wmkcErr_NetSockfdType 9LL // 错误的套接字类型，只能用作：函数要求是TCP但传入的是UDP套接字或相反。
#define wmkcErr_NetSend    10LL // 套接字发送数据失败，只能用作未能成功且正确的使用send函数时。
#define wmkcErr_NetRecv    11LL // 套接字接收数据失败，只能用作未能成功且正确的使用recv函数时。
#define wmkcErr_NetShutdown 12LL // 套接字关闭失败，用作自己实现的关闭套接字的函数中。
#define wmkcErr_NetClose   13LL // 套接字关闭失败，用作自己实现的关闭套接字的函数中。
#define wmkcErr_NetWsaData 14LL // 表示启动调用WSAStartup函数时出现错误。
#define wmkcErr_NetFamily  15LL // 错误的网络家族，应使用AF_INET（PF_INET）或AF_INET6（PF_INET6）
#define wmkcErr_NetSetSockOpt  15LL // 表示启动调用setsockopt函数时出现错误。
#define wmkcErr_FileFolderPath 32LL // 错误的路径，此错误类型表示用户指定的路径没有文件或（和）文件夹。
#define wmkcErr_FileNull     33LL // 此值表示用户不应该读取一个空文件或不应该写入一个空的数据到文件。
#define wmkcErr_FileOpen     34LL // 此值表示调用fopen函数或_wfopen来打开文件失败了。
#define wmkcErr_FileClose    35LL // 此值表示调用fclose函数来关闭文件失败了。
#define wmkcErr_ThreadCreate 48LL // 此值表示线程创建时出现错误
#define wmkcErr_ThreadJoin   49LL // 此值表示线程阻塞出现错误
#define wmkcErr_ThreadStart  50LL // 此指表示线程启动出现错误

// 将wmkcErr对象赋值并作为返回值返回。
#define wmkcErr_return(error, _code, _message) \
    error.message = _message; \
    error.code = _code; \
    return error

#endif /* WMKC_ERROR */
#endif /* WMKC_SUPPORT */
