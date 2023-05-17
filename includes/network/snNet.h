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
#include <snMath.h>

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
typedef snSize snNetType;                 // snNet的协议类型（TCP,UDP）
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
typedef sn_u32  snNetType;   // snNet的协议类型（TCP,UDP）
#endif // #if defined(_WIN32)

// snNet的时间类型
typedef snFloat snNetTime;
// snNet的长度类型
typedef socklen_t snNetSize;

// snNet的网络结构体
typedef struct {
    // 客户端，用于本机作为接收端时保存客户端信息时使用
    SOCKADDR *client;
    // 接收端（也可以叫本机端），用于连接网络服务器或让
    // 客户端连接时使用
    SOCKADDR *receiver;
    // 本机端和客户端的内存占用大小（单位字节）
    snNetSize size;
} snNetInfo;

// snNet对象类型
typedef struct {
    // socket数据
    snNetSocket sockfd;       // sizeof: win[8], linux[4]
    // socket使用的网络家族
    sn_u32      sockfdFamily; // sizeof: 4
    // socket的类型，TCP或UDP
    snNetType   sockfdType;   // sizeof: win[4], linux[8]
    // snNet的网络结构体
    snNetInfo  *info;         // sizeof: 8
} snNet_ctx;

// 申请snNet对象的内存空间
SN_PUBLIC(snError) snNet_new SN_OPEN_API
SN_FUNC_OF((snNet_ctx **ctx, sn_u32 family));

// 释放snNet对象的内存空间，如果你是手动申请的内存，那么请
// 确保未申请内存的指针成员的指向为空地址。
SN_PUBLIC(snError) snNet_release SN_OPEN_API
SN_FUNC_OF((snNet_ctx **ctx));

// 初始化snNet对象（需提前申请内存空间）
SN_PUBLIC(snError) snNet_init SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snString hostname, sn_u16 port, snBool UDP));

// 设置发送与接收超时时间
SN_PUBLIC(snError) snNet_timeout SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetTime _user_TimeOut));

// 连接函数
SN_PUBLIC(snError) snNet_connect SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx));

// 绑定函数
SN_PUBLIC(snError) snNet_bind SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetTime _user_TimeOut));

// 监听函数
SN_PUBLIC(snError) snNet_listen SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, sn_u32 _Listen));

// 等待连接函数
SN_PUBLIC(snError) snNet_accept SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx));

// 发送函数，_tSize用来保存本次传输的长度
SN_PUBLIC(snError) snNet_send SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetSize *_tSize, snNetBuf *buf, snNetSize size));

// 全部发送函数
SN_PUBLIC(snError) snNet_sendall SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx, snNetBuf *buf, snNetSize size));

// 接收函数，_tSize用来保存本次传输的长度
SN_PUBLIC(snError) snNet_recv SN_OPEN_API
SN_FUNC_OF((snNet_ctx* ctx, snNetSize *_tSize, snNetBuf *buf, snNetSize size));

// 关闭套接字函数
SN_PUBLIC(snError) snNet_close SN_OPEN_API
SN_FUNC_OF((snNet_ctx *ctx));

#endif // #ifndef __SN_NETWORK__
