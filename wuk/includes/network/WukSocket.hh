#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_NET
#define WUK_CPP_NET
#include <network/WukException.hh>
#include <cmath>

#if defined(WUK_PLATFORM_LINUX)
#   include <netdb.h>
#   include <unistd.h>
#   include <sys/time.h>
#   include <arpa/inet.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <netinet/ip.h>
#   include <netinet/tcp.h>
#   define WUK_NET_ERROR EOF             // 定义错误代码
typedef struct sockaddr     SOCKADDR;     // 套接字地址结构
typedef struct addrinfo     ADDRINFO;     // 域名解析结构
typedef struct sockaddr_in  SOCKADDR_IN;  // IPv4网络结构
typedef struct sockaddr_in6 SOCKADDR_IN6; // IPv6网络结构
typedef wS32   wSocket;                   // wukNet的socket类型
#elif defined(WUK_PLATFORM_WINOS)
#   include <WS2tcpip.h>
#   if defined(_MSC_VER)
#       pragma comment(lib, "WS2_32")
#   endif
#   define WUK_NET_ERROR SOCKET_ERROR    // 定义错误代码
typedef SOCKET         wSocket;           // wukNet的socket类型
#endif /* WUK_PLATFORM_LINUX */

#include <WukMemory.hh>

namespace wuk {
    namespace net {
        class LIBWUK_API IPEndPoint {
        public:
            std::string addr;
            wU16 port;
            IPEndPoint(std::string addr, wU16 port): addr(addr), port(port) {}
            IPEndPoint():addr(),port() {};
        };

        class LIBWUK_API SocketOption {
        public:
            const void *val;
            socklen_t val_len;
            SocketOption(const void *val, socklen_t val_len)
            : val(val), val_len(val_len) {};
        };
    }
}

namespace wuk {
    namespace net {
        ADDRINFO    *get_addr_info(wS32 family, wS32 type, wS32 proto, std::string addr,
                                std::string serviceName);
        IPEndPoint  get_network_info(wSocket sockfd, wS32 family);
        IPEndPoint  get_network_info(wS32 family, SOCKADDR *pAddr);
        std::string network_addr_to_string_addr(wS32 family, const void *pAddr);
        wU16        network_port_to_number_port(const wU16 port);

        class LIBWUK_API Socket {
        public:
            double timeout; // 超时时间
            wSocket fd; // 套接字文件描述符
            wS32 family; // 套接字网络家族
            wS32 type;   // 套接字类型
            wS32 proto;  // 套接字协议
            wS32 transmissionLength; // 单次传输长度

            IPEndPoint lAddr; // 套接字绑定的本地IP端点
            IPEndPoint rAddr; // 套接字绑定的远程IP端点

            Socket(wS32 _family, wS32 _type, wS32 _proto = 0, wSocket _fd = EOF);
            ~Socket();

            void setsockopt(int level, int optName, SocketOption &opt);
            void getsockopt(int level, int optName, SocketOption &opt);

            void settimeout(double _time);
            void connect(const std::string addr, const wU16 port);
            void bind(const std::string addr, const wU16 port);
            void listen(const wS32 backlog);
            Socket accept();
            void send(const std::string content, const wS32 flag = 0);
            void sendall(const std::string content, const wS32 flag = 0);
            void sendto(const std::string content, IPEndPoint target,
                        const wS32 flag = 0);
            std::string recv(const wS32 len, const wS32 flag = 0);
            std::string recvfrom(const wS32 len, SOCKADDR *from = nullptr,
                        socklen_t *fromlen = nullptr, const wS32 flag = 0);
            void shutdown(const wS32 how);
            void close();
        };
    };
}

#endif /* WUK_CPP_NET */
#endif /* WUK_SUPPORT */
