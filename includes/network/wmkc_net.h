/*
* 2023年2月7日重新开启对WIndows系统的支持
* 
* Windows:
*   不要手动包含Windows.h或WinSock2.h，WS2tcpip.h中包含了WinSock2.h
*   编译指令：gcc main.c -lws2_32 -o main.exe
*
*/
#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_NETWORK
#define WMKC_NETWORK
#include <wmkc_error.h>
#include <math.h>

#define WMKC_NET_DEFAULT_LISTEN 5  // listen函数使用的默认值
#define WMKC_NET_TCP_TYPE 0x54     // TCP类型
#define WMKC_NET_UDP_TYPE 0x55     // UDP类型
#define WMKC_NET_BLOCKLEN 4096     // wmkcNet的块长度，用作传输

#define WMKC_NET_IPV4_ADDR_SIZE 16 // 用于储存IPv4信息的内存大小
#define WMKC_NET_IPV6_ADDR_SIZE 28 // 用于储存IPv6信息的内存大小

#define WMKC_NET_OPENSSL_API true     // 是否启用OpenSSL的功能

#if WMKC_NET_OPENSSL_API
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/sslerr.h>
#include <openssl/crypto.h>
#endif /* WMKC_NET_OPENSSL_API */

#if defined(WMKC_PLATFORM_LINUX)
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
typedef struct sockaddr     SOCKADDR;      // 套接字地址结构
typedef struct addrinfo     ADDRINFO;      // 域名解析结构
typedef struct sockaddr_in  SOCKADDR_IN;   // IPv4网络结构
typedef struct sockaddr_in6 SOCKADDR_IN6;  // IPv6网络结构
typedef        wmkc_u32     wmkcNetSocket; // wmkcNet的socket类型
typedef        wmkcVoid     wmkcNetTimer;  // wmkcNet的计时类型
typedef        wmkcByte     wmkcNetBuf;    // wmkcNet的缓冲区类型
#elif defined(WMKC_PLATFORM_WINOS)
#include <WS2tcpip.h>
#if (!defined(__MINGW32__) && !defined(__MINGW64__))
#   pragma comment(lib, "WS2_32.lib")
#endif
typedef SOCKET    wmkcNetSocket; // wmkcNet的socket类型
typedef wmkcChar  wmkcNetTimer;  // wmkcNet的计时类型
typedef wmkcChar  wmkcNetBuf;    // wmkcNet的缓冲区类型
#endif /* WMKC_PLATFORM_LINUX */

#include <wmkc_memory.h>

typedef double    wmkcNetTime; // wmkcNet的时间类型
typedef socklen_t wmkcNetSize; // wmkcNet的长度类型
typedef wmkc_u32  wmkcNetType; // wmkcNet的类型类型

typedef struct {
    SSL     *ssl;
    SSL_CTX *ssl_ctx;
} wmkcNet_ssl;

// wmkcNet对象类型
typedef struct {
    wmkcNetSocket sockfd;         // 套接字【size: win[8], linux[4]】
    wmkcNetType   sockfdFamily;   // 套接字家族类型
    wmkcNetType   sockfdType;     // 套接字网络类型（TCP or UDP）
    wmkcNetSize   addr_info_size; // 网络地址信息结构体的长度
    SOCKADDR     *addr_info;      // 用于保留地址信息的结构体【size: 8】
    wmkcNet_ssl  *ssl_obj;        // SSL/TLS协议结构体
} wmkcNet_obj;

/**
 * @brief 创建一个TCP协议的套接字
 * @authors SN-Grotesque
 * @note 无
 * @param family 代表你要使用的网络家族，比如AF_INET或AF_INET6
 * @param protocol 代表你要使用的网络协议，比如IPPROTO_IP
 * @return 返回一个wmkcNetSocket类型的变量
 */
WMKC_PUBLIC(wmkcNetSocket) wmkcNet_TCP_Socket WMKC_OPEN_API
WMKC_OF((wmkcNetSize family, wmkcNetSize protocol));

/**
 * @brief 创建一个UDP协议的套接字
 * @authors SN-Grotesque
 * @note 无
 * @param family 代表你要使用的网络家族，比如AF_INET或AF_INET6
 * @param protocol 代表你要使用的网络协议，比如IPPROTO_IP
 * @return 返回一个wmkcNetSocket类型的变量
 */
WMKC_PUBLIC(wmkcNetSocket) wmkcNet_UDP_Socket WMKC_OPEN_API
WMKC_OF((wmkcNetSize family, wmkcNetSize protocol));

/**
 * @brief 从字节序列获取IPv4地址的字符串
 * @authors SN-Grotesque
 * @note 无
 * @param addr 这是一个in_addr结构体的数据
 * @return 返回一个wmkcChar指针，为字符串或NULL
 */
WMKC_PUBLIC(wmkcChar *) wmkcNet_GetAddr WMKC_OPEN_API
WMKC_OF((struct in_addr addr));

/**
 * @brief 从字节序列获取IPv6地址的字符串
 * @authors SN-Grotesque
 * @note 无
 * @param pAddr 指针，指向一个字节序列的地址
 * @param pStringBuf 指针，为字符串地址。
 * @return 返回一个const char类型的指针。
 */
WMKC_PUBLIC(wmkcCSTR) wmkcNet_GetAddr6 WMKC_OPEN_API
WMKC_OF((wmkcVoid *pAddr, wmkcChar *pStringBuf));

/**
 * @brief （小端序计算机）将端口号从大端序改为小端序
 * @authors SN-Grotesque
 * @note 无
 * @param port 这是一个端口号，是一个最大为16位的数字。
 * @return 返回一个wmkc_u16类型变量，为端口号。
 */
WMKC_PUBLIC(wmkc_u16) wmkcNet_GetPort WMKC_OPEN_API
WMKC_OF((wmkc_u16 port));

/**
 * @brief 创建新的wmkcNet对象
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcNet对象指针的地址。
 * @param _ssl_method 指针，指向SSL_METHOD。
 *                    为空时代表不使用OpenSSL组件。
 * @param family 代表你要使用的网络家族，比如AF_INET或AF_INET6。
 * @param UDP 为True时代表使用UDP套接字，反之为TCP套接字。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_new WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **obj, const SSL_METHOD *_ssl_method, wmkcNetType family, wmkcBool UDP));

/**
 * @brief 释放wmkcNet对象
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcNet对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_free WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **obj));

/**
 * @brief 初始化wmkcNet库，并解析域名。
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcNet对象的地址。
 *            为NULL时后面的参数全部无效。
 * @param hostname 域名字符串。
 * @param port 目标的网络端口。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_init WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcCSTR hostname, wmkc_u16 port));

/**
 * @brief 解析域名到SOCKADDR指针中
 * @authors SN-Grotesque
 * @note 无
 * @param dst 指针，指向SOCKADDR类型对象的地址。
 * @param hostname 域名字符串。
 * @param family 代表你要使用的网络家族，比如AF_INET或AF_INET6。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_getaddrinfo WMKC_OPEN_API
WMKC_OF((SOCKADDR *dst, wmkcCSTR hostname, wmkcNetType family));

/**
 * @brief 设置发送与接收超时时间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcNet对象的地址。
 * @param _user_TimeOut 这是超时时间，原型为double类型。为0时不设置超时
 *                     （当然直接不调用这个函数最方便）
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_timeout WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetTime _user_TimeOut));

/**
 * @brief 连接函数（连接至目标主机与端口）
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcNet对象的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_connect WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj));

/**
 * @brief 绑定函数（绑定主机与端口）
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcNet对象的地址。
 * @param _user_TimeOut 这是超时时间，用于setsockopt函数设置SO_REUSEADDR时间。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_bind WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetTime _user_TimeOut));

/**
 * @brief 监听函数（监听主机与端口）
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcNet对象的地址。
 * @param _Listen 这是一个整数，表示监听队列中的最多个数。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_listen WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkc_u32 _Listen));

/**
 * @brief 等待连接函数
 * @authors SN-Grotesque
 * @note 此函数有两个wmkcNet对象的原因是为了更好的兼容多线程网络编程。
 * @param dst 指针，指向用于接受连接信息的wmkcNet对象地址。
 * @param src 指针，指向wmkcNet对象的地址，源wmkcNet对象。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_accept WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *dst, wmkcNet_obj *src));

/**
 * @brief 发送函数
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcNet对象的地址。
 * @param _tSize 指针，指向单次传输长度变量的地址。可以为空。
 * @param buf 指针，指向缓冲区地址。
 * @param size 这是一个长度，表示缓冲区内需要发送的内容的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_send WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetSize *_tSize, wmkcNetBuf *buf, wmkcNetSize size));

/**
 * @brief 全部发送函数
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcNet对象的地址。
 * @param buf 指针，指向缓冲区地址。
 * @param size 这是一个长度，表示缓冲区内需要发送的内容的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_sendall WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBuf *buf, wmkcNetSize size));

/**
 * @brief 接收函数
 * @authors SN-Grotesque
 * @note 无
 * @param obj 指针，指向wmkcNet对象的地址。
 * @param _tSize 指针，指向单次传输长度变量的地址。可以为空。
 * @param buf 指针，指向缓冲区地址。
 * @param size 这是一个长度，表示缓冲区内可以接收的内容的长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_recv WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetSize *_tSize, wmkcNetBuf *buf, wmkcNetSize size));

/**
 * @brief 关闭套接字与WSADATA（如果有的话）的函数
 * @authors SN-Grotesque
 * @note 在有些情况下这个函数会返回一个错误代码，那是因为你的套接字没有使用。
 * @param obj 指针，指向wmkcNet对象的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcNet_close WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj));

#endif /* WMKC_NETWORK */
#endif /* WMKC_SUPPORT */
