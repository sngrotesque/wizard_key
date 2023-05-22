/*
* 2023年2月7日重新开启对WIndows系统的支持
* 
* Windows:
*   不要手动包含Windows.h或WinSock2.h，WS2tcpip.h中包含了WinSock2.h
*   编译指令：gcc main.c -lws2_32 -o main.exe
*
*/
#ifndef __SN_NETWORK__
#define __SN_NETWORK__

#include <snConf.h>
#include <math.h>

// TCP套接字
#define snNet_TCP_Socket(family, protocol) socket(family, SOCK_STREAM, protocol)
// UDP套接字
#define snNet_UDP_Socket(family, protocol) socket(family, SOCK_DGRAM, protocol)

// 示例: snNet_GetAddr(server->sin_addr :struct in_addr) -> cbar *
#define snNet_GetAddr(info) inet_ntoa(info)
// 示例: snNet_GetAddr6(server->sin6_addr :struct in6_addr, ipv6String :char *) -> char * || NULL
#define snNet_GetAddr6(info, ipv6String) inet_ntop(AF_INET6, info, ipv6String, INET6_ADDRSTRLEN)
// 示例: snNet_GetPort(server->sin_port :u_short) -> u_short
#define snNet_GetPort(info) ntohs(info)

#define SN_NET_OPENSSL_API 0     // 是否启用OpenSSL的功能

#define SN_NET_DEFAULT_LISTEN 5  // listen函数使用的默认值
#define SN_NET_TCP_TYPE 0x54     // TCP类型
#define SN_NET_UDP_TYPE 0x55     // UDP类型
#define SN_NET_BLOCKLEN 4096     // snNet的块长度，用作传输

#define SN_NET_IPV4_ADDR_SIZE 16 // 用于储存IPv4信息的内存大小
#define SN_NET_IPV6_ADDR_SIZE 28 // 用于储存IPv6信息的内存大小

#if defined(SN_NET_OPENSSL_API) && (SN_NET_OPENSSL_API == 1)
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/tls1.h>
#include <openssl/sslerr.h>
#include <openssl/crypto.h>
#include <openssl/ossl_typ.h>
#endif

#if defined(__linux)
#include <netdb.h>
#include <resolv.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
typedef struct sockaddr     SOCKADDR;     // 套接字地址结构
typedef struct addrinfo     ADDRINFO;     // 域名解析结构
typedef struct sockaddr_in  SOCKADDR_IN;  // IPv4网络结构
typedef struct sockaddr_in6 SOCKADDR_IN6; // IPv6网络结构
typedef sn_u32 snNetSocket;               // snNet的socket类型
typedef snVoid snNetTimer;                // snNet的计时类型
typedef snByte snNetBuf;                  // snNet的缓冲区类型
#endif // #if defined(__linux)

#if defined(_WIN32)
/**
 *  如果在使用此库的同时使用其他库，那么请确保此库为第一个导入的。
 *  或者请不要同时使用那些会导入Windows.h库的库。
*/ 
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
typedef SOCKET  snNetSocket; // snNet的socket类型
typedef snChar  snNetTimer;  // snNet的计时类型
typedef snChar  snNetBuf;    // snNet的缓冲区类型
#endif // #if defined(_WIN32)

typedef snFloat   snNetTime; // snNet的时间类型
typedef socklen_t snNetSize; // snNet的长度类型

// snNet对象类型
typedef struct {
    snNetSocket sockfd;         // 套接字【size: win[8], linux[4]】
    snNetSize   sockfdFamily;   // 套接字家族类型
    snNetSize   sockfdType;     // 套接字网络类型（TCP or UDP）
    snNetSize   addr_info_size; // 网络地址信息结构体的长度
    SOCKADDR   *addr_info;      // 用于保留地址信息的结构体【size: 8】
} snNet_ctx;

// 申请snNet对象的内存空间
SN_PUBLIC(snErr_ctx) snNet_new SN_OPEN_API
SN_FUNC_OF((snNet_ctx **ctx, sn_u32 family));

// 释放snNet对象。
SN_PUBLIC(snErr_ctx) snNet_free SN_OPEN_API
SN_FUNC_OF((snNet_ctx **ctx));

// 初始化snNet对象（需提前申请内存空间）
SN_PUBLIC(snErr_ctx) snNet_init SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snString hostname, sn_u16 port, snBool UDP));

// 使用snNet对象解析域名信息
SN_PUBLIC(snErr_ctx) snNet_resolveAddress
SN_FUNC_OF((snNet_ctx *net, snString hostname));

// 设置发送与接收超时时间
SN_PUBLIC(snErr_ctx) snNet_timeout SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetTime _user_TimeOut));

// 连接函数
SN_PUBLIC(snErr_ctx) snNet_connect SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx));

// 绑定函数
SN_PUBLIC(snErr_ctx) snNet_bind SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetTime _user_TimeOut));

// 监听函数
SN_PUBLIC(snErr_ctx) snNet_listen SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, sn_u32 _Listen));

// 等待连接函数
SN_PUBLIC(snErr_ctx) snNet_accept SN_OPEN_API
SN_FUNC_OF((snNet_ctx *dst, snNet_ctx *src));

// 发送函数，_tSize用来保存本次传输的长度
SN_PUBLIC(snErr_ctx) snNet_send SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetSize *_tSize, snNetBuf *buf, snNetSize size));

// 全部发送函数
SN_PUBLIC(snErr_ctx) snNet_sendall SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetBuf *buf, snNetSize size));

// 接收函数，_tSize用来保存本次传输的长度
SN_PUBLIC(snErr_ctx) snNet_recv SN_OPEN_API
SN_FUNC_OF((snNet_ctx* ctx, snNetSize *_tSize, snNetBuf *buf, snNetSize size));

// 关闭套接字函数
SN_PUBLIC(snErr_ctx) snNet_close SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx));

#endif // #ifndef __SN_NETWORK__
