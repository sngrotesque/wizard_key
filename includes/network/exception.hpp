#include <config/wuk.hpp>

#if WUK_SUPPORT
#ifndef WUK_NET_EXCEPTION
#define WUK_NET_EXCEPTION
#include <config/exception.hpp>

#if defined(WUK_PLATFORM_LINUX)
#include <errno.h>
#include <netdb.h>
#elif defined(WUK_PLATFORM_WINOS)
#include <WS2tcpip.h>
#endif

/*********************************************************************
# 套接字具体错误代码

### General error

------

##### WUK_NET_ERR_EINTR - [accept, send, recv, shutdown, close, connect]
    执行的调用被操作系统截断。

##### WUK_NET_ERR_EACCES - [bind, send, socket, connect]
    以不被允许的方式访问套接字。

##### WUK_NET_ERR_EFAULT - [bind, accept, send, recv, connect]
    地址指向了用户的不可访问的地址空间。

##### WUK_NET_ERR_EINVAL - [bind, shutdown, listen, accept, sned, recv, socket, connect]
    提供了无效的参数或套接字已绑定某地址或在接受之前未调用侦听函数或套接字是监听套接字。

##### WUK_NET_ERR_EMFILE - [accept, listen, socket]
    无法提供更多套接字描述符，已达到每个进程对套接字数量的限制。

##### WUK_NET_ERR_EISCONN - [listen, send, connect]
    套接字已被指定连接。

##### WUK_NET_ERR_ENOBUFS - [bind, accept, send, listen, socket, connect]
    未提供任何缓冲区空间。

##### WUK_NET_ERR_EALREADY - [send, connect]
    另一个快速打开操作（TCP Fast Open）正在执行。指定的套接字上正在进行非阻塞模式connect调用。

##### WUK_NET_ERR_ENOTCONN - [listen, shutdown, send, recv]
    套接字已经连接了 or 套接字未连接

##### WUK_NET_ERR_EMSGSIZE - [send, recv]
    套接字面向消息，消息大于基础传输支持的最大大小

##### WUK_NET_ERR_ENOTSOCK - [bind, listen, shutdown, accept, send, recv, closesocket, connect]
    描述符不是套接字

##### WUK_NET_ERR_ETIMEDOUT - [send, recv, connect]
    由于超时导致连接已断开。

##### WUK_NET_ERR_EPROTOTYPE - [socket, accept, connect]
    指定的协议是此套接字的错误类型，套接字类型不支持请求的通信协议。

##### WUK_NET_ERR_EOPNOTSUPP - [send, recv, listen, accept]
    引用的套接字不是支持面向连接（监听）的服务的类型，或者套接字是单向的，只支持发送或接收操作。

##### WUK_NET_ERR_EADDRINUSE - [bind, listen, accept, connect]
    本地地址或端口已被使用。套接字没有绑定到地址。

##### WUK_NET_ERR_ECONNRESET - [shutdown, accept, send, recv]
    连接被重置。套接字不可再用，应关闭套接字（此错误仅适用于面向连接的套接字）。

##### WUK_NET_ERR_ENETUNREACH - [connect]
    此时不可以从此主机访问该网络。网络不可用。

##### WUK_NET_ERR_EWOULDBLOCK - [accept, send, recv, closesocket, connect]
    套接字标记为非阻塞模式，没有要接受的连接，请求的操作会进行阻止。套接字标记为非阻塞模式，无法立即完成连接。

##### WUK_NET_ERR_EINPROGRESS - [bind, listen, shutdown, accept, send, recv, closesocket, socket, connect]
    套接字是非阻塞的，调用操作无法立即完成。

##### WUK_NET_ERR_ECONNREFUSED - [recv, connect]
    连接被远程主机强制性拒绝。

##### WUK_NET_ERR_ECONNABORTED - [shutdown, send, recv, accept]
    面向连接的套接字由于超时或其他故障线路已终止，套接字不可再用。

##### WUK_NET_ERR_EAFNOSUPPORT - [socket, connect]
    指定网络族中的地址无法与此套接字一起使用。

##### WUK_NET_ERR_EADDRNOTAVAIL - [bind, connect]
    请求的地址在上下文无效或请求的地址不是本地地址。远程地址不是有效的地址。

##### WUK_NET_ERR_EPROTONOSUPPORT - [socket]
    不支持指定的协议

------

### Linux error

------

##### WUK_NET_ERR_EIO - [close]
    出现I/O错误

##### WUK_NET_ERR_EPIPE - [send]
    本地端已在面向连接的套接字上关闭，在这种情况下，除非设置了MSG_NOSIGNAL，否则进程也将接收SIGPIPE。

##### WUK_NET_ERR_EPERM - [accept, connect]
    防火墙规则禁止连接

##### WUK_NET_ERR_EROFS - [bind]
    套接字索引节点将驻留在只读文件系统上

##### WUK_NET_ERR_EBADF - [bind, accept, close, shutdown, send, recv, listen, connect]
    参数sockfd不是有效的文件描述符

##### WUK_NET_ERR_ELOOP - [bind]
    解析地址时遇到太多符号链接

##### WUK_NET_ERR_EDQUOT
##### WUK_NET_ERR_ENOSPC - [close]
    存储空间不足或已达到磁盘配额限制。

##### WUK_NET_ERR_ENOENT - [bind]
    套接字路径名的目录前缀中的组件不存在

##### WUK_NET_ERR_ENOMEM - [socket, accept, send, recv]
    可用的内核内存不足

##### WUK_NET_ERR_ENFILE - [socket, accept]
    已达到系统范围内打开文件总数的限制

##### WUK_NET_ERR_EPROTO - [accept]
    错误的协议

##### WUK_NET_ERR_EAGAIN - [accept, send, recv, connect]
    套接字被标记为非阻塞，并且不存在任何可接受的连接。
    sockfd引用的套接字以前没有绑定到地址，在尝试将其绑定到临时端口时，确定临时端口范围内的所有端口号当前都在使用中。

##### WUK_NET_ERR_ENOTDIR - [bind]
    路径前缀的组件不是目录

##### WUK_NET_ERR_ENAMETOOLONG - [bind]
    地址太长

##### WUK_NET_ERR_EDESTADDRREQ - [send]
    套接字不是连接模式，并且没有设置对等地址。

------

### Windows error

------

##### WUK_NET_ERR_ENETDOWN - [bind, listen, shutdown, accept, send, recv, closesocket, socket, connect]
    网络子系统发生故障

##### WUK_NET_ERR_ESHUTDOWN - [send, recv]
    套接字已被shutdown函数关闭

##### WUK_NET_ERR_EHOSTUNREACH - [send, connect]
    此时无法从此主机访问远程主机

##### WUK_NET_ERR_NOTINITIALISED - [bind, listen, shutdown, accept, send, recv, closesocket, socket, connect]
    未完成对WSAStartup函数的调用

##### WUK_NET_ERR_ESOCKTNOSUPPORT - [socket]
    此地址族不支持指定的套接字类型

##### WUK_NET_ERR_EINVALIDPROVIDER - [socket]
    服务提供商返回了2.2以外的版本

##### WUK_NET_ERR_EINVALIDPROCTABLE - [socket]
    服务提供程序向WSPStartup返回了无效或不完整的过程表

##### WUK_NET_ERR_EPROVIDERFAILEDINIT - [socket]
    服务提供程序未能初始化。如果分层服务提供程序（LSP）或命名空间提供程序安装不正确或提供程序无法正常运行则会返回此错误。
*********************************************************************/

#if defined(WUK_PLATFORM_WINOS)
/* Public *************************************************** */
#define WUK_NET_ERR_EINTR               WSAEINTR
#define WUK_NET_ERR_EACCES              WSAEACCES
#define WUK_NET_ERR_EFAULT              WSAEFAULT
#define WUK_NET_ERR_EINVAL              WSAEINVAL
#define WUK_NET_ERR_EMFILE              WSAEMFILE
#define WUK_NET_ERR_EISCONN             WSAEISCONN
#define WUK_NET_ERR_ENOBUFS             WSAENOBUFS
#define WUK_NET_ERR_ENOTCONN            WSAENOTCONN
#define WUK_NET_ERR_EALREADY            WSAEALREADY
#define WUK_NET_ERR_EMSGSIZE            WSAEMSGSIZE
#define WUK_NET_ERR_ENOTSOCK            WSAENOTSOCK
#define WUK_NET_ERR_ETIMEDOUT           WSAETIMEDOUT
#define WUK_NET_ERR_EPROTOTYPE          WSAEPROTOTYPE
#define WUK_NET_ERR_EOPNOTSUPP          WSAEOPNOTSUPP
#define WUK_NET_ERR_EADDRINUSE          WSAEADDRINUSE
#define WUK_NET_ERR_ECONNRESET          WSAECONNRESET
#define WUK_NET_ERR_EINPROGRESS         WSAEINPROGRESS
#define WUK_NET_ERR_ENETUNREACH         WSAENETUNREACH
#define WUK_NET_ERR_EWOULDBLOCK         WSAEWOULDBLOCK
#define WUK_NET_ERR_ECONNREFUSED        WSAECONNREFUSED
#define WUK_NET_ERR_ECONNABORTED        WSAECONNABORTED
#define WUK_NET_ERR_EAFNOSUPPORT        WSAEAFNOSUPPORT
#define WUK_NET_ERR_EADDRNOTAVAIL       WSAEADDRNOTAVAIL
#define WUK_NET_ERR_EPROTONOSUPPORT     WSAEPROTONOSUPPORT
/* Pirivate ************************************************** */
#define WUK_NET_ERR_ENETDOWN            WSAENETDOWN
#define WUK_NET_ERR_ENETRESET           WSAENETRESET
#define WUK_NET_ERR_ESHUTDOWN           WSAESHUTDOWN
#define WUK_NET_ERR_EHOSTUNREACH        WSAEHOSTUNREACH
#define WUK_NET_ERR_NOTINITIALISED      WSANOTINITIALISED
#define WUK_NET_ERR_ESOCKTNOSUPPORT     WSAESOCKTNOSUPPORT
#define WUK_NET_ERR_EINVALIDPROVIDER    WSAEINVALIDPROVIDER
#define WUK_NET_ERR_EINVALIDPROCTABLE   WSAEINVALIDPROCTABLE
#define WUK_NET_ERR_EPROVIDERFAILEDINIT WSAEPROVIDERFAILEDINIT
#   elif defined(WUK_PLATFORM_LINUX)
/* Public *************************************************** */
#define WUK_NET_ERR_EINTR               EINTR
#define WUK_NET_ERR_EACCES              EACCES
#define WUK_NET_ERR_EFAULT              EFAULT
#define WUK_NET_ERR_EINVAL              EINVAL
#define WUK_NET_ERR_EMFILE              EMFILE
#define WUK_NET_ERR_EISCONN             EISCONN
#define WUK_NET_ERR_ENOBUFS             ENOBUFS
#define WUK_NET_ERR_ENOTCONN            ENOTCONN
#define WUK_NET_ERR_EALREADY            EALREADY
#define WUK_NET_ERR_EMSGSIZE            EMSGSIZE
#define WUK_NET_ERR_ENOTSOCK            ENOTSOCK
#define WUK_NET_ERR_ETIMEDOUT           ETIMEDOUT
#define WUK_NET_ERR_EPROTOTYPE          EPROTOTYPE
#define WUK_NET_ERR_EOPNOTSUPP          EOPNOTSUPP
#define WUK_NET_ERR_EADDRINUSE          EADDRINUSE
#define WUK_NET_ERR_ECONNRESET          ECONNRESET
#define WUK_NET_ERR_EINPROGRESS         EINPROGRESS
#define WUK_NET_ERR_ENETUNREACH         ENETUNREACH
#define WUK_NET_ERR_EWOULDBLOCK         EWOULDBLOCK
#define WUK_NET_ERR_ECONNREFUSED        ECONNREFUSED
#define WUK_NET_ERR_ECONNABORTED        ECONNABORTED
#define WUK_NET_ERR_EAFNOSUPPORT        EAFNOSUPPORT
#define WUK_NET_ERR_EADDRNOTAVAIL       EADDRNOTAVAIL
#define WUK_NET_ERR_EPROTONOSUPPORT     EPROTONOSUPPORT
/* Pirivate ************************************************** */
#define WUK_NET_ERR_EIO                 EIO
#define WUK_NET_ERR_EPERM               EPERM
#define WUK_NET_ERR_EROFS               EROFS
#define WUK_NET_ERR_EBADF               EBADF
#define WUK_NET_ERR_ELOOP               ELOOP
#define WUK_NET_ERR_EPIPE               EPIPE
#define WUK_NET_ERR_ENOSPC              ENOSPC
#define WUK_NET_ERR_EDQUOT              EDQUOT
#define WUK_NET_ERR_EPROTO              EPROTO
#define WUK_NET_ERR_EAGAIN              EAGAIN // == WUK_NET_ERR_EWOULDBLOCK
#define WUK_NET_ERR_ENOENT              ENOENT
#define WUK_NET_ERR_ENFILE              ENFILE
#define WUK_NET_ERR_ENOMEM              ENOMEM
#define WUK_NET_ERR_ENOTDIR             ENOTDIR
#define WUK_NET_ERR_ENAMETOOLONG        ENAMETOOLONG
#define WUK_NET_ERR_EDESTADDRREQ        EDESTADDRREQ
#define WUK_NET_ERR_ECONNREFUSED        ECONNREFUSED
#endif

namespace wuk {
    namespace net {
        LIBWUK_API void exception(std::string funcName);
    }
}

#endif
#endif
