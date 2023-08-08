#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_NETWORK
#define WMKC_NETWORK
#include <wmkc_error.h>
#include <math.h>

#define WMKC_NET_IPV4_ADDR_SIZE 16 // 用于储存IPv4信息的内存大小
#define WMKC_NET_IPV6_ADDR_SIZE 28 // 用于储存IPv6信息的内存大小

// 关于这些宏的说明：includes/network/wmkc_net_macro.md
#if defined(WMKC_PLATFORM_WINOS)
/* Public *************************************************** */
#define WMKC_NET_ERR_EINTR               WSAEINTR
#define WMKC_NET_ERR_EACCES              WSAEACCES
#define WMKC_NET_ERR_EFAULT              WSAEFAULT
#define WMKC_NET_ERR_EINVAL              WSAEINVAL
#define WMKC_NET_ERR_EMFILE              WSAEMFILE
#define WMKC_NET_ERR_EISCONN             WSAEISCONN
#define WMKC_NET_ERR_ENOBUFS             WSAENOBUFS
#define WMKC_NET_ERR_ENOTCONN            WSAENOTCONN
#define WMKC_NET_ERR_EALREADY            WSAEALREADY
#define WMKC_NET_ERR_EMSGSIZE            WSAEMSGSIZE
#define WMKC_NET_ERR_ENOTSOCK            WSAENOTSOCK
#define WMKC_NET_ERR_ETIMEDOUT           WSAETIMEDOUT
#define WMKC_NET_ERR_EPROTOTYPE          WSAEPROTOTYPE
#define WMKC_NET_ERR_EOPNOTSUPP          WSAEOPNOTSUPP
#define WMKC_NET_ERR_EADDRINUSE          WSAEADDRINUSE
#define WMKC_NET_ERR_ECONNRESET          WSAECONNRESET
#define WMKC_NET_ERR_EINPROGRESS         WSAEINPROGRESS
#define WMKC_NET_ERR_ENETUNREACH         WSAENETUNREACH
#define WMKC_NET_ERR_EWOULDBLOCK         WSAEWOULDBLOCK
#define WMKC_NET_ERR_ECONNREFUSED        WSAECONNREFUSED
#define WMKC_NET_ERR_ECONNABORTED        WSAECONNABORTED
#define WMKC_NET_ERR_EAFNOSUPPORT        WSAEAFNOSUPPORT
#define WMKC_NET_ERR_EADDRNOTAVAIL       WSAEADDRNOTAVAIL
#define WMKC_NET_ERR_EPROTONOSUPPORT     WSAEPROTONOSUPPORT
/* Pirivate ************************************************** */
#define WMKC_NET_ERR_ENETDOWN            WSAENETDOWN
#define WMKC_NET_ERR_ENETRESET           WSAENETRESET
#define WMKC_NET_ERR_ESHUTDOWN           WSAESHUTDOWN
#define WMKC_NET_ERR_EHOSTUNREACH        WSAEHOSTUNREACH
#define WMKC_NET_ERR_NOTINITIALISED      WSANOTINITIALISED
#define WMKC_NET_ERR_ESOCKTNOSUPPORT     WSAESOCKTNOSUPPORT
#define WMKC_NET_ERR_EINVALIDPROVIDER    WSAEINVALIDPROVIDER
#define WMKC_NET_ERR_EINVALIDPROCTABLE   WSAEINVALIDPROCTABLE
#define WMKC_NET_ERR_EPROVIDERFAILEDINIT WSAEPROVIDERFAILEDINIT
#   elif defined(WMKC_PLATFORM_LINUX)
/* Public *************************************************** */
#define WMKC_NET_ERR_EINTR               EINTR
#define WMKC_NET_ERR_EACCES              EACCES
#define WMKC_NET_ERR_EFAULT              EFAULT
#define WMKC_NET_ERR_EINVAL              EINVAL
#define WMKC_NET_ERR_EMFILE              EMFILE
#define WMKC_NET_ERR_EISCONN             EISCONN
#define WMKC_NET_ERR_ENOBUFS             ENOBUFS
#define WMKC_NET_ERR_ENOTCONN            ENOTCONN
#define WMKC_NET_ERR_EALREADY            EALREADY
#define WMKC_NET_ERR_EMSGSIZE            EMSGSIZE
#define WMKC_NET_ERR_ENOTSOCK            ENOTSOCK
#define WMKC_NET_ERR_ETIMEDOUT           ETIMEDOUT
#define WMKC_NET_ERR_EPROTOTYPE          EPROTOTYPE
#define WMKC_NET_ERR_EOPNOTSUPP          EOPNOTSUPP
#define WMKC_NET_ERR_EADDRINUSE          EADDRINUSE
#define WMKC_NET_ERR_ECONNRESET          ECONNRESET
#define WMKC_NET_ERR_EINPROGRESS         EINPROGRESS
#define WMKC_NET_ERR_ENETUNREACH         ENETUNREACH
#define WMKC_NET_ERR_EWOULDBLOCK         EWOULDBLOCK
#define WMKC_NET_ERR_ECONNREFUSED        ECONNREFUSED
#define WMKC_NET_ERR_ECONNABORTED        ECONNABORTED
#define WMKC_NET_ERR_EAFNOSUPPORT        EAFNOSUPPORT
#define WMKC_NET_ERR_EADDRNOTAVAIL       EADDRNOTAVAIL
#define WMKC_NET_ERR_EPROTONOSUPPORT     EPROTONOSUPPORT
/* Pirivate ************************************************** */
#define WMKC_NET_ERR_EIO                 EIO
#define WMKC_NET_ERR_EPERM               EPERM
#define WMKC_NET_ERR_EROFS               EROFS
#define WMKC_NET_ERR_EBADF               EBADF
#define WMKC_NET_ERR_ELOOP               ELOOP
#define WMKC_NET_ERR_EPIPE               EPIPE
#define WMKC_NET_ERR_ENOSPC              ENOSPC
#define WMKC_NET_ERR_EDQUOT              EDQUOT
#define WMKC_NET_ERR_EPROTO              EPROTO
#define WMKC_NET_ERR_EAGAIN              EAGAIN // == WMKC_NET_ERR_EWOULDBLOCK
#define WMKC_NET_ERR_ENOENT              ENOENT
#define WMKC_NET_ERR_ENFILE              ENFILE
#define WMKC_NET_ERR_ENOMEM              ENOMEM
#define WMKC_NET_ERR_ENOTDIR             ENOTDIR
#define WMKC_NET_ERR_ENAMETOOLONG        ENAMETOOLONG
#define WMKC_NET_ERR_EDESTADDRREQ        EDESTADDRREQ
#define WMKC_NET_ERR_ECONNREFUSED        ECONNREFUSED
#endif

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
typedef        wmkc_s32     wmkcNetSockT; // wmkcNet的socket类型
typedef        wmkcByte     wmkcNetBufT;  // wmkcNet的缓冲区类型
#elif defined(WMKC_PLATFORM_WINOS)
#include <WS2tcpip.h>
#if (!defined(__MINGW32__) && !defined(__MINGW64__))
#   pragma comment(lib, "WS2_32.lib")
#endif
typedef SOCKET    wmkcNetSockT; // wmkcNet的socket类型
typedef wmkcChar  wmkcNetBufT;  // wmkcNet的缓冲区类型
#endif /* WMKC_PLATFORM_LINUX */

#include <wmkc_memory.h>

typedef struct {
    SOCKADDR *sockAddress;
    wmkcChar  addr[INET6_ADDRSTRLEN];
    wmkc_u16  port;
    socklen_t sockAddressSize;
} wmkcNet_addr;

// wmkcNet对象类型
typedef struct {
    wmkcNetSockT sockfd; // 套接字
    wmkc_s32 family; // 套接字家族类型（AF_INET, AF_INET6...）
    wmkc_s32 type;   // 套接字网络类型（SOCK_STREAM, SOCK_DGRAM...）
    wmkc_s32 proto;  // 套接字的协议类型（IPPROTO_IP, IPPROTO_TCP...）
    wmkc_s32 tSize;  // 单次传输长度（-1或0为错误）
    wmkcNet_addr *laddr; // 本地网络地址信息
    wmkcNet_addr *raddr; // 目标网络地址信息
    double timeout; // 套接字的超时时间，用于设置bind, connect, acceot, send, recv...
} wmkcNet_obj;

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_errorHandler WMKC_OPEN_API
WMKC_OF((wmkcCSTR funcName));

WMKC_PUBLIC(wmkcCSTR) wmkcNet_GetAddr WMKC_OPEN_API
WMKC_OF((wmkc_s32 family, wmkcVoid *pAddr, wmkcChar *pStringBuf));

WMKC_PUBLIC(wmkc_u16) wmkcNet_GetPort WMKC_OPEN_API
WMKC_OF((wmkc_u16 port));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_new WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **obj));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_free WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **obj));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_socket WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkc_s32 family, wmkc_s32 type, socklen_t proto));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_getaddrinfo WMKC_OPEN_API
WMKC_OF((wmkcNet_addr *addr, wmkcCSTR hostname, wmkc_u16 port,
    wmkc_s32 family, wmkc_s32 type, wmkc_s32 proto));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_settimeout WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, double _value));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_bind WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcCSTR addr, wmkc_u16 port));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_connect WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcCSTR addr, wmkc_u16 port));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_listen WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkc_u32 backlog));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_accept WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **dst, wmkcNet_obj *src));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_send WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT *content, socklen_t size, wmkc_s32 _flag));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_sendall WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT *content, wmkcSize size, wmkc_s32 _flag));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_recv WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT *content, socklen_t size, wmkc_s32 _flag));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_shutdown WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkc_u32 how));

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_close WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj));

#endif /* WMKC_NETWORK */
#endif /* WMKC_SUPPORT */
