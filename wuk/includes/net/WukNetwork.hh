#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT

#if defined(WUK_PLATFORM_LINUX)
#   include <netdb.h>
#   include <unistd.h>
#   include <sys/time.h>
#   include <arpa/inet.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <netinet/ip.h>
#   include <netinet/tcp.h>
#elif defined(WUK_PLATFORM_WINOS)
#   include <WS2tcpip.h>
#   ifdef WUK_COMPILER_MSVC
#       pragma comment(lib, "WS2_32")
#   endif
#endif

#endif