#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_NETWORK
#define WMKC_NETWORK
#include <wmkc_error.h>
#include <math.h>

#define WMKC_NET_BLOCKLEN 4096     // wmkcNet的块长度，用作传输

#define WMKC_NET_IPV4_ADDR_SIZE 16 // 用于储存IPv4信息的内存大小
#define WMKC_NET_IPV6_ADDR_SIZE 28 // 用于储存IPv6信息的内存大小

#define WMKC_NET_OPENSSL_API true  // 是否启用OpenSSL的功能

// shutdown函数的错误代码
#   if defined(WMKC_PLATFORM_WINOS)
#define WMKC_NET_SHUTDOWN_ERR_EINVAL   WSAEINVAL
#define WMKC_NET_SHUTDOWN_ERR_ENOTCONN WSAENOTCONN
#define WMKC_NET_SHUTDOWN_ERR_ENOTSOCK WSAENOTSOCK
#   elif defined(WMKC_PLATFORM_LINUX)
#define WMKC_NET_SHUTDOWN_ERR_EINVAL   EINVAL
#define WMKC_NET_SHUTDOWN_ERR_ENOTCONN ENOTCONN
#define WMKC_NET_SHUTDOWN_ERR_ENOTSOCK ENOTSOCK
#   endif

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
typedef struct sockaddr     SOCKADDR; // 套接字地址结构
typedef struct addrinfo     ADDRINFO; // 域名解析结构
typedef struct sockaddr_in  SOCKADDR_IN;  // IPv4网络结构
typedef struct sockaddr_in6 SOCKADDR_IN6; // IPv6网络结构
typedef        wmkcVoid     wmkcNetTimer; // wmkcNet的计时类型
typedef        wmkc_u32     wmkcNetSock; // wmkcNet的socket类型
typedef        wmkcByte     wmkcNetBuf;  // wmkcNet的缓冲区类型
#elif defined(WMKC_PLATFORM_WINOS)
#include <WS2tcpip.h>
#if (!defined(__MINGW32__) && !defined(__MINGW64__))
#   pragma comment(lib, "WS2_32.lib")
#endif
typedef wmkcChar  wmkcNetTimer; // wmkcNet的计时类型
typedef SOCKET    wmkcNetSock; // wmkcNet的socket类型
typedef wmkcChar  wmkcNetBuf;  // wmkcNet的缓冲区类型
#endif /* WMKC_PLATFORM_LINUX */

#include <wmkc_memory.h>

typedef double    wmkcNetTime; // wmkcNet的时间类型
typedef socklen_t wmkcNetSize; // wmkcNet的长度类型
typedef wmkc_u32  wmkcNetType; // wmkcNet的类型类型

typedef struct {
    SOCKADDR *sockAddress;
    wmkcChar *addr;
    wmkc_u16  port;
    wmkcNetSize sockAddressSize;
} wmkcNet_addr;

// wmkcNet对象类型
typedef struct {
    wmkcNetSock sockfd; // 套接字
    wmkcNetType family; // 套接字家族类型
    wmkcNetType type;   // 套接字网络类型（TCP or UDP）
    wmkcNetType proto;  // 套接字的协议类型（IPPROTO_IP, IPPROTO_TCP...）
    wmkcNet_addr *laddr; // 本地网络地址信息
    wmkcNet_addr *raddr; // 目标网络地址信息
} wmkcNet_obj;

#endif /* WMKC_NETWORK */
#endif /* WMKC_SUPPORT */
