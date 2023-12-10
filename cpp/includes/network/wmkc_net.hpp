#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_NET
#define WMKC_CPP_NET
#include <network/wmkc_net_exception.hpp>
#include <wmkc_exception.hpp>
#include <math.h>

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
#define WMKC_NET_ERROR      EOF // 定义错误代码
#elif defined(WMKC_PLATFORM_WINOS)
#include <WS2tcpip.h>
#if defined(_MSC_VER)
#   pragma comment(lib, "WS2_32")
#endif
typedef SOCKET         wmkcNetSockT; // wmkcNet的socket类型
typedef wmkcChar       wmkcNetBufT;  // wmkcNet的缓冲区类型
#define WMKC_NET_ERROR SOCKET_ERROR // 定义错误代码
#endif /* WMKC_PLATFORM_LINUX */

#include <wmkc_memory.hpp>

namespace wmkcNet {
    // wmkcNet的IP端点
    class IPEndPoint {
        public:
            std::string addr;
            wmkc_u16 port;
            IPEndPoint(std::string addr, wmkc_u16 port);
    };

    ADDRINFO *getAddrInfo(wmkc_s32 family, wmkc_s32 type, wmkc_s32 proto, std::string addr, std::string serviceName);

    std::string networkAddr2stringAddr(wmkc_s32 family, const wmkcVoid *pAddr);
    uint16_t networkPort2numberPort(const wmkc_u16 port);
    IPEndPoint getNetworkInfo(wmkcNetSockT sockfd, wmkc_s32 family);
    IPEndPoint getNetworkInfo(wmkc_s32 family, wmkcVoid *pAddr);

    class Socket {
        private:
            wmkc_s32 err;
        public:
            double timeout; // 超时时间
            wmkcNetSockT fd; // 套接字文件描述符
            wmkc_s32 family; // 套接字网络家族
            wmkc_s32 type;   // 套接字类型
            wmkc_s32 proto;  // 套接字协议
            wmkc_s32 transmissionLength; // 单次传输长度

            wmkcNet::IPEndPoint lAddr; // 套接字绑定的本地IP端点
            wmkcNet::IPEndPoint rAddr; // 套接字绑定的远程IP端点

            Socket(wmkc_s32 _family, wmkc_s32 _type, wmkc_s32 _proto, wmkcNetSockT _fd = EOF);
            ~Socket();

            void settimeout(double _time);
            void connect(const std::string addr, const wmkc_u16 port);
            void bind(const std::string addr, const wmkc_u16 port);
            void listen(const wmkc_s32 backlog);
            wmkcNet::Socket accept();
            void send(const std::string content, const wmkc_s32 flag = 0);
            void sendall(const std::string content, const wmkc_s32 flag = 0);
            void sendto(const std::string content, const wmkc_s32 flag = 0);
            std::string recv(const wmkc_s32 len, const wmkc_s32 flag = 0);
            void shutdown(const wmkc_s32 how);
            void close();
    };
};

#endif /* WMKC_CPP_NET */
#endif /* WMKC_SUPPORT */
