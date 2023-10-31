#include <wmkc_conf.hpp>
#include <wmkc_error.h>
#include <math.h>

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
#if defined(_MSC_VER)
#   pragma comment(lib, "WS2_32")
#endif
typedef SOCKET    wmkcNetSockT; // wmkcNet的socket类型
typedef wmkcChar  wmkcNetBufT;  // wmkcNet的缓冲区类型
#endif /* WMKC_PLATFORM_LINUX */
#include <wmkc_memory.h>

class wmkcNet_error : public std::exception {
    private:
        wmkc_s32 errCode;
        string errFuncName;
        string errMessage;
    public:
        wmkcNet_error(wmkc_s32 code, const string& funcName, const string& message)
        : errCode(code), errMessage(message), errFuncName(funcName) {}
        const char* what() const noexcept override {
            return format("{0}[{1}]: {2}", errFuncName, errCode, errMessage).c_str();
        }
};

class wmkcNet {
    private:
        wmkcNetSockT sockfd;
        wmkc_s32 family;
        wmkc_s32 type;
        wmkc_s32 proto;
        wmkc_s32 err;
        double timeout;
        ADDRINFO hints;

        string localSocketAddr;
        string remoteSocketAddr;

        void wmkcNet_exception(string funcName);
        string getAddr(wmkcVoid *pAddr);
        wmkc_u16 getPort(wmkc_u16 port);
    public:
        wmkc_s32 size; // 单次传输长度
        wmkcNet(wmkc_s32 _family, wmkc_s32 _type, wmkc_s32 _proto, wmkcNetSockT _fd = EOF)
        : family(_family), type(_type), proto(_proto), hints()
        {
            if(_fd == EOF) {
                this->sockfd = socket(this->family, this->type, this->proto);
                if(this->sockfd == EOF) {
                    this->wmkcNet_exception("wmkcNet::wmkcNet");
                }
            } else {
                this->sockfd = _fd;
            }
        }
        ~wmkcNet();
        ADDRINFO *getAddrInfo(string addr, string serviceName);
        void settimeout(double _time);
        void connect(string addr, wmkc_u16 port);
        void bind(string addr, wmkc_u16 port);
        void listen(wmkc_s32 backlog);
        wmkcNet *accept();
        void send(wmkcNetBufT *buf, wmkc_s32 len, wmkc_s32 flag = 0);
        void sendall(wmkcNetBufT *buf, wmkc_s32 len, wmkc_s32 flag = 0);
        void send(string content, wmkc_s32 flag = 0);
        void sendall(string content, wmkc_s32 flag = 0);
        void recv(wmkcNetBufT *buf, wmkc_s32 len, wmkc_s32 flag = 0);
        string recv(wmkc_s32 len, wmkc_s32 flag = 0);
        void shutdown(wmkc_s32 how);
        void close();
};



