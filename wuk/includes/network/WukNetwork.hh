#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <network/WukException.hh>
#include <WukMemory.hh>

#if defined(WUK_PLATFORM_LINUX)
#   include <netdb.h>
#   include <unistd.h>
#   include <sys/time.h>
#   include <arpa/inet.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <netinet/ip.h>
#   include <netinet/tcp.h>
#   define WUK_NET_ERROR EOF              // 定义错误代码
#elif defined(WUK_PLATFORM_WINOS)
#   include <WS2tcpip.h>
#   if defined(_MSC_VER)
#       pragma comment(lib, "WS2_32")
#   endif
#   define WUK_NET_ERROR SOCKET_ERROR     // 定义错误代码
#endif

#if defined(WUK_PLATFORM_LINUX)
typedef struct sockaddr         SOCKADDR;
typedef struct addrinfo         ADDRINFO;
typedef struct sockaddr_in      SOCKADDR_IN;
typedef struct sockaddr_in6     SOCKADDR_IN6;
typedef struct sockaddr_storage SOCKADDR_STORAGE;
typedef wS32   wSocket;                   // wukNet的socket类型
#elif defined(WUK_PLATFORM_WINOS)
typedef SOCKET wSocket;                   // wukNet的socket类型
#endif

#endif
