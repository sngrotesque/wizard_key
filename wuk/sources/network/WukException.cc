#include <network/WukException.hh>

#define CASE_ERR(x) msg = (x); break

void wuk::net::exception(std::string funcName)
{
#   if defined(WUK_PLATFORM_WINOS)
    wS32 err = WSAGetLastError();
#   elif defined(WUK_PLATFORM_LINUX)
    wS32 err = errno;
#   endif
    const char *msg = nullptr;

    switch(err) {
        case WUK_NET_ERR_EINTR: // 执行的调用被操作系统截断。
            CASE_ERR("The call executed was truncated by the operating system.");
        case WUK_NET_ERR_EACCES: // 以不允许的方式访问套接字。
            CASE_ERR("Accessing sockets in an impermissible manner.");
        case WUK_NET_ERR_EFAULT: // 地址指向用户不可访问的地址空间。
            CASE_ERR("The address points to the user's inaccessible address space.");
        case WUK_NET_ERR_EINVAL: // 提供了无效的套接字参数。
            CASE_ERR("An invalid socket parameter was provided.");
        case WUK_NET_ERR_EMFILE: // 无法提供套接字，已达到进程对套接字数量的限制。
            CASE_ERR("Unable to provide sockets, the process's limit on "
                "the number of sockets has been reached.");
        case WUK_NET_ERR_EISCONN: // 已经为连接指定了套接字。
            CASE_ERR("The socket has already been specified for connection.");
        case WUK_NET_ERR_ENOBUFS: // 没有提供缓冲空间
            CASE_ERR("No buffer space provided.");
        case WUK_NET_ERR_EALREADY: // 另一个（TCP快速打开）正在执行，或者套接字处于非阻塞模式。
            CASE_ERR("Another (TCP Fast Open) is executing or the socket is in "
                "non blocking mode.");
        case WUK_NET_ERR_ENOTCONN: // 套接字已连接（未连接）。
            CASE_ERR("The socket is already (not) connected.");
        case WUK_NET_ERR_EMSGSIZE: // 该消息大于传输支持的限制。
            CASE_ERR("The message is greater than the limit supported by "
                "the transmission.");
        case WUK_NET_ERR_ENOTSOCK: // 描述符不是套接字。
            CASE_ERR("The descriptor is not a socket. :)");
        case WUK_NET_ERR_ETIMEDOUT: // 由于超时，连接已断开。
            CASE_ERR("The connection has been disconnected due to timeout.");
        case WUK_NET_ERR_EPROTOTYPE: // 套接字类型不支持此操作。
            CASE_ERR("The socket type does not support this operation.");
        case WUK_NET_ERR_EOPNOTSUPP: // 此套接字不是面向连接的，或者是单向的。
            CASE_ERR("This socket is not connection oriented, or it is unidirectional.");
        case WUK_NET_ERR_EADDRINUSE: // 地址或端口已（未）在使用中。
            CASE_ERR("The address or port is already (not) in use.");
        case WUK_NET_ERR_ECONNRESET: // 连接被重置
            CASE_ERR("Connection is reset");
        case WUK_NET_ERR_ENETUNREACH: // 网络不可用。
            CASE_ERR("The network is not available.");
        case WUK_NET_ERR_EWOULDBLOCK: // 插座没有阻塞，也没有连接。
            CASE_ERR("The socket is non blocking and there is no connection.");
        case WUK_NET_ERR_EINPROGRESS: // 套接字是非阻塞的，不能立即执行。
            CASE_ERR("The socket is non blocking and cannot be executed immediately.");
        case WUK_NET_ERR_ECONNREFUSED:
            CASE_ERR("The connection was forcibly rejected by the remote host.");
        case WUK_NET_ERR_ECONNABORTED:
            CASE_ERR("The socket is unavailable due to timeout or other faults.");
        case WUK_NET_ERR_EAFNOSUPPORT:
            CASE_ERR("The address in the specified network family cannot be used with "
                "this socket.");
        case WUK_NET_ERR_EADDRNOTAVAIL:
            CASE_ERR("The requested address is invalid.");
        case WUK_NET_ERR_EPROTONOSUPPORT:
            CASE_ERR("The specified protocol is not supported.");
#       if defined(WUK_PLATFORM_LINUX)
        case WUK_NET_ERR_EIO:
            CASE_ERR("An I/O error occurred.");
        case WUK_NET_ERR_EPIPE:
            CASE_ERR("The local end has been closed on a connection oriented socket.");
        case WUK_NET_ERR_EPERM:
            CASE_ERR("Firewall rules prohibit connections.");
        case WUK_NET_ERR_EROFS:
            CASE_ERR("The socket index node will reside on a read-only file system.");
        case WUK_NET_ERR_EBADF:
            CASE_ERR("The descriptor is not a socket.");
        case WUK_NET_ERR_ELOOP:
            CASE_ERR("Too many Symbolic link were encountered while resolving addresses.");
        case WUK_NET_ERR_EDQUOT:
        case WUK_NET_ERR_ENOSPC:
            CASE_ERR("The disk quota limit has been reached (insufficient storage space).");
        case WUK_NET_ERR_ENOENT:
            CASE_ERR("The component in the directory prefix of the socket path name does "
                "not exist.");
        case WUK_NET_ERR_ENOMEM:
            CASE_ERR("Insufficient available kernel memory.");
        case WUK_NET_ERR_ENFILE:
            CASE_ERR("Unable to provide sockets, the process's limit on the number of "
                "sockets has been reached.");
        case WUK_NET_ERR_EPROTO:
            CASE_ERR("Wrong protocol.");
        case WUK_NET_ERR_ENOTDIR:
            CASE_ERR("The component with path prefix is not a directory.");
        case WUK_NET_ERR_ENAMETOOLONG:
            CASE_ERR("addr is too long.");
        case WUK_NET_ERR_EDESTADDRREQ:
            CASE_ERR("The socket is not in connection mode and has no peer address set.");
#       elif defined(WUK_PLATFORM_WINOS)
        case WUK_NET_ERR_ENETDOWN:
            CASE_ERR("The network subsystem has malfunctioned.");
        case WUK_NET_ERR_ESHUTDOWN:
            CASE_ERR("The socket has been closed by the shutdown function.");
        case WUK_NET_ERR_EHOSTUNREACH:
            CASE_ERR("Unable to access the remote host at this time.");
        case WUK_NET_ERR_NOTINITIALISED:
            CASE_ERR("The call to the WSAStartup function was not completed.");
        case WUK_NET_ERR_ESOCKTNOSUPPORT:
            CASE_ERR("This address family does not support the specified socket type.");
        case WUK_NET_ERR_EINVALIDPROVIDER:
            CASE_ERR("The service provider returned a version other than 2.2.");
        case WUK_NET_ERR_EINVALIDPROCTABLE:
            CASE_ERR("The service provider returned an invalid or incomplete procedure "
                "table to the WSPStartup.");
        case WUK_NET_ERR_EPROVIDERFAILEDINIT:
            CASE_ERR("The service provider failed to initialize.");
#       endif
        default:
            switch(err) {
                case EAI_AGAIN:    /* A temporary failure in name resolution occurred. */
                case EAI_BADFLAGS: /* An invalid value was provided for the
                                    ai_flags member of the pHints parameter. */
                case EAI_FAIL:     /* A nonrecoverable failure in name resolution occurred. */
                case EAI_FAMILY:   /* The ai_family member of the pHints parameter is
                                    not supported. */
                case EAI_MEMORY:   /* A memory allocation failure occurred. */
                case EAI_NONAME:   /* The name does not resolve for the supplied
                                    parameters or the pNodeName and pServiceName
                                    parameters were not provided. */
                case EAI_SERVICE:  /* The pServiceName parameter is not supported for
                                    the specified ai_socktype member of
                                    the pHints parameter. */
                case EAI_SOCKTYPE: /* The ai_socktype member of the pHints parameter
                                    is not supported. */
#               if defined(WUK_PLATFORM_LINUX)
                case EAI_ADDRFAMILY:
                case EAI_NODATA:
#               endif
                    CASE_ERR("A temporary failure in name resolution occurred.");
#               if defined(WUK_PLATFORM_LINUX)
                case EAI_SYSTEM:
                    CASE_ERR("Other system error; errno is set to indicate the error.");
#               endif
                default:
                    CASE_ERR("unknown error");
            }
    }
    throw wuk::Exception(err, funcName, msg);
}
