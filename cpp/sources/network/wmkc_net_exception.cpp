#include <network/wmkc_net_exception.hpp>

void wmkcNet::Socket_exception(std::string funcName)
{
#   if defined(WMKC_PLATFORM_WINOS)
    wmkc_s32 err = WSAGetLastError();
#   elif defined(WMKC_PLATFORM_LINUX)
    wmkc_s32 err = errno;
#   endif
    const char *msg = wmkcNull;

    switch(err) {
        case WMKC_NET_ERR_EINTR:
            msg = "The call executed was truncated by the operating system."; break;
        case WMKC_NET_ERR_EACCES:
            msg = "Accessing sockets in an impermissible manner."; break;
        case WMKC_NET_ERR_EFAULT:
            msg = "The address points to the user's inaccessible address space."; break;
        case WMKC_NET_ERR_EINVAL:
            msg = "An invalid socket parameter was provided."; break;
        case WMKC_NET_ERR_EMFILE:
            msg = "Unable to provide sockets, the process's limit on the number of sockets has been reached."; break;
        case WMKC_NET_ERR_EISCONN:
            msg = "The socket has already been specified for connection."; break;
        case WMKC_NET_ERR_ENOBUFS:
            msg = "No buffer space provided."; break;
        case WMKC_NET_ERR_EALREADY:
            msg = "Another (TCP Fast Open) is executing or the socket is in non blocking mode."; break;
        case WMKC_NET_ERR_ENOTCONN:
            msg = "The socket is already (not) connected."; break;
        case WMKC_NET_ERR_EMSGSIZE:
            msg = "The message is greater than the limit supported by the transmission."; break;
        case WMKC_NET_ERR_ENOTSOCK:
            msg = "The descriptor is not a socket. :)"; break;
        case WMKC_NET_ERR_ETIMEDOUT:
            msg = "The connection has been disconnected due to timeout."; break;
        case WMKC_NET_ERR_EPROTOTYPE:
            msg = "The socket type does not support this operation."; break;
        case WMKC_NET_ERR_EOPNOTSUPP:
            msg = "This socket is not connection oriented, or it is unidirectional."; break;
        case WMKC_NET_ERR_EADDRINUSE:
            msg = "The address or port is already (not) in use."; break;
        case WMKC_NET_ERR_ECONNRESET:
            msg = "Connection is reset"; break;
        case WMKC_NET_ERR_ENETUNREACH:
            msg = "The network is not available."; break;
        case WMKC_NET_ERR_EWOULDBLOCK:
            msg = "The socket is non blocking and there is no connection."; break;
        case WMKC_NET_ERR_EINPROGRESS:
            msg = "The socket is non blocking and cannot be executed immediately."; break;
        case WMKC_NET_ERR_ECONNREFUSED:
            msg = "The connection was forcibly rejected by the remote host."; break;
        case WMKC_NET_ERR_ECONNABORTED:
            msg = "The socket is unavailable due to timeout or other faults."; break;
        case WMKC_NET_ERR_EAFNOSUPPORT:
            msg = "The address in the specified network family cannot be used with this socket."; break;
        case WMKC_NET_ERR_EADDRNOTAVAIL:
            msg = "The requested address is invalid."; break;
        case WMKC_NET_ERR_EPROTONOSUPPORT:
            msg = "The specified protocol is not supported."; break;
#       if defined(WMKC_PLATFORM_LINUX)
        case WMKC_NET_ERR_EIO:
            msg = "An I/O error occurred."; break;
        case WMKC_NET_ERR_EPIPE:
            msg = "The local end has been closed on a connection oriented socket."; break;
        case WMKC_NET_ERR_EPERM:
            msg = "Firewall rules prohibit connections."; break;
        case WMKC_NET_ERR_EROFS:
            msg = "The socket index node will reside on a read-only file system."; break;
        case WMKC_NET_ERR_EBADF:
            msg = "The descriptor is not a socket."; break;
        case WMKC_NET_ERR_ELOOP:
            msg = "Too many Symbolic link were encountered while resolving addresses."; break;
        case WMKC_NET_ERR_EDQUOT:
        case WMKC_NET_ERR_ENOSPC:
            msg = "The disk quota limit has been reached (insufficient storage space)."; break;
        case WMKC_NET_ERR_ENOENT:
            msg = "The component in the directory prefix of the socket path name does not exist."; break;
        case WMKC_NET_ERR_ENOMEM:
            msg = "Insufficient available kernel memory."; break;
        case WMKC_NET_ERR_ENFILE:
            msg = "Unable to provide sockets, the process's limit on the number of sockets has been reached."; break;
        case WMKC_NET_ERR_EPROTO:
            msg = "Wrong protocol."; break;
        case WMKC_NET_ERR_ENOTDIR:
            msg = "The component with path prefix is not a directory."; break;
        case WMKC_NET_ERR_ENAMETOOLONG:
            msg = "addr is too long."; break;
        case WMKC_NET_ERR_EDESTADDRREQ:
            msg = "The socket is not in connection mode and has no peer address set."; break;
#       elif defined(WMKC_PLATFORM_WINOS)
        case WMKC_NET_ERR_ENETDOWN:
            msg = "The network subsystem has malfunctioned."; break;
        case WMKC_NET_ERR_ESHUTDOWN:
            msg = "The socket has been closed by the shutdown function."; break;
        case WMKC_NET_ERR_EHOSTUNREACH:
            msg = "Unable to access the remote host at this time."; break;
        case WMKC_NET_ERR_NOTINITIALISED:
            msg = "The call to the WSAStartup function was not completed."; break;
        case WMKC_NET_ERR_ESOCKTNOSUPPORT:
            msg = "This address family does not support the specified socket type."; break;
        case WMKC_NET_ERR_EINVALIDPROVIDER:
            msg = "The service provider returned a version other than 2.2."; break;
        case WMKC_NET_ERR_EINVALIDPROCTABLE:
            msg = "The service provider returned an invalid or incomplete procedure table to the WSPStartup."; break;
        case WMKC_NET_ERR_EPROVIDERFAILEDINIT:
            msg = "The service provider failed to initialize."; break;
#       endif
        default:
            switch(err) {
                case EAI_AGAIN:
                    msg = "A temporary failure in name resolution occurred."; break;
                case EAI_BADFLAGS:
                    msg = "An invalid value was provided for the ai_flags member of the pHints parameter."; break;
                case EAI_FAIL:
                    msg = "A nonrecoverable failure in name resolution occurred."; break;
                case EAI_FAMILY:
                    msg = "The ai_family member of the pHints parameter is not supported."; break;
                case EAI_MEMORY:
                    msg = "A memory allocation failure occurred."; break;
                case EAI_NONAME:
                    msg = ("The name does not resolve for the supplied parameters or the pNodeName and "
                        "pServiceName parameters were not provided."); break;
                case EAI_SERVICE:
                    msg = ("The pServiceName parameter is not supported for the specified ai_socktype member of "
                        "the pHints parameter."); break;
                case EAI_SOCKTYPE:
                    msg = "The ai_socktype member of the pHints parameter is not supported."; break;
                default:
                    msg = "wmkcNet::Socket_exception: Unexpected error."; break;
            }
    }
    throw std::runtime_error(std::string(funcName) + "[" + std::to_string(err) + "]: " + msg);
}