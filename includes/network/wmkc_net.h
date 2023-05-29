﻿/*
* 2023年2月7日重新开启对WIndows系统的支持
* 
* Windows:
*   不要手动包含Windows.h或WinSock2.h，WS2tcpip.h中包含了WinSock2.h
*   编译指令：gcc main.c -lws2_32 -o main.exe
*
*/

/*
* 这个函数的注释，慢慢来，不要着急。
* 不要着急，不要着急，不要着急，就这一个了，不要慌。
*/
#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_NETWORK
#define WMKC_NETWORK
#include <math.h>

// TCP套接字
#define wmkcNet_TCP_Socket(family, protocol) \
    socket(family, SOCK_STREAM, protocol)

// UDP套接字
#define wmkcNet_UDP_Socket(family, protocol) \
    socket(family, SOCK_DGRAM, protocol)

// 示例: wmkcNet_GetAddr(server->sin_addr :struct in_addr) -> cbar *
#define wmkcNet_GetAddr(info) inet_ntoa(info)

// 示例: wmkcNet_GetAddr6(server->sin6_addr :struct in6_addr, ipv6String :char *) -> char * || NULL
#define wmkcNet_GetAddr6(info, ipv6String) \
    inet_ntop(AF_INET6, info, ipv6String, INET6_ADDRSTRLEN)

// 示例: wmkcNet_GetPort(server->sin_port :u_short) -> u_short
#define wmkcNet_GetPort(info) ntohs(info)

#define WMKC_NET_DEFAULT_LISTEN 5  // listen函数使用的默认值
#define WMKC_NET_TCP_TYPE 0x54     // TCP类型
#define WMKC_NET_UDP_TYPE 0x55     // UDP类型
#define WMKC_NET_BLOCKLEN 4096     // wmkcNet的块长度，用作传输

#define WMKC_NET_IPV4_ADDR_SIZE 16 // 用于储存IPv4信息的内存大小
#define WMKC_NET_IPV6_ADDR_SIZE 28 // 用于储存IPv6信息的内存大小

#define WMKC_NET_OPENSSL_API 0     // 是否启用OpenSSL的功能

#if defined(WMKC_NET_OPENSSL_API) && (WMKC_NET_OPENSSL_API == 1)
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/tls1.h>
#include <openssl/sslerr.h>
#include <openssl/crypto.h>
#include <openssl/ossl_typ.h>
#endif

#if defined(WMKC_PLATFORM_LINUX)
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
typedef struct sockaddr     SOCKADDR;      // 套接字地址结构
typedef struct addrinfo     ADDRINFO;      // 域名解析结构
typedef struct sockaddr_in  SOCKADDR_IN;   // IPv4网络结构
typedef struct sockaddr_in6 SOCKADDR_IN6;  // IPv6网络结构
typedef        wmkc_u32     wmkcNetSocket; // wmkcNet的socket类型
typedef        wmkcVoid     wmkcNetTimer;  // wmkcNet的计时类型
typedef        wmkcByte     wmkcNetBuf;    // wmkcNet的缓冲区类型
#elif defined(WMKC_PLATFORM_WINOS)
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
typedef SOCKET    wmkcNetSocket; // wmkcNet的socket类型
typedef wmkcChar  wmkcNetTimer;  // wmkcNet的计时类型
typedef wmkcChar  wmkcNetBuf;    // wmkcNet的缓冲区类型
#endif /* #if defined(WMKC_PLATFORM_LINUX) */

typedef wmkcFloat wmkcNetTime; // wmkcNet的时间类型
typedef socklen_t wmkcNetSize; // wmkcNet的长度类型

// wmkcNet对象类型
typedef struct {
    wmkcNetSocket sockfd;         // 套接字【size: win[8], linux[4]】
    wmkcNetSize   sockfdFamily;   // 套接字家族类型
    wmkcNetSize   sockfdType;     // 套接字网络类型（TCP or UDP）
    wmkcNetSize   addr_info_size; // 网络地址信息结构体的长度
    SOCKADDR     *addr_info;      // 用于保留地址信息的结构体【size: 8】
} wmkcNet_obj;

/**
 * @brief 为wmkcNet对象申请内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcNet对象指针的地址。
 * @param family 这是一个网络家族类型，只能是AF_INET或AF_INET6。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_new WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **obj, wmkc_u32 family));

/**
 * @brief 为wmkcNet对象释放内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcNet对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_free WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **obj));

/**
 * @brief 初始化wmkcNet对象
 * @authors SN-Grotesque
 * @note 需要使用wmkcNet_new函数提前申请内存空间
 * @param obj 这是一个指针，指向wmkcNet对象的地址。
 * @param hostname 这是一个指针，指向域名字符串的地址，会自动将域名解析并保存到wmkcNet对象中。
 *                 如果为空，那么port参数失效，并且不会进行域名解析。
 * @param port 这是一个端口号，为需要连接或绑定的端口。
 * @param UDP 这是一个布尔值，为True时将套接字设置为UDP，否则TCP。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_init WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcString hostname, wmkc_u16 port, wmkcBool UDP));

/**
 * @brief 解析域名并将IP地址储存到wmkcNet对象
 * @authors SN-Grotesque
 * @note 根据在wmkcNet_new函数中设定的family来解析对应的IPv4或IPv6地址。
 * @param obj 这是一个指针，指向wmkcNet对象的地址。
 * @param hostname 这是一个指针，指向域名字符串的地址，会自动将域名解析并保存到wmkcNet对象中。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_resolveAddress WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcString hostname));

// 设置发送与接收超时时间
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_timeout WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetTime _user_TimeOut));

// 连接函数
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_connect WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj));

// 绑定函数
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_bind WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetTime _user_TimeOut));

// 监听函数
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_listen WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkc_u32 _Listen));

// 等待连接函数
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_accept WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *dst, wmkcNet_obj *src));

// 发送函数，_tSize用来保存本次传输的长度（可以为空）
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_send WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetSize *_tSize, wmkcNetBuf *buf, wmkcNetSize size));

// 全部发送函数
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_sendall WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBuf *buf, wmkcNetSize size));

// 接收函数，_tSize用来保存本次传输的长度（可以为空）
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_recv WMKC_OPEN_API
WMKC_OF((wmkcNet_obj* obj, wmkcNetSize *_tSize, wmkcNetBuf *buf, wmkcNetSize size));

// 关闭套接字函数
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_close WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj));

#endif /* WMKC_NETWORK */
#endif /* WMKC_SUPPORT */
