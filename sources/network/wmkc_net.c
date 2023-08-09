#include <network/wmkc_net.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_errorHandler WMKC_OPEN_API
WMKC_OF((wmkcCSTR funcName))
{
#   if defined(WMKC_PLATFORM_WINOS)
    wmkc_s32 errCodeBySystem = WSAGetLastError();
#   elif defined(WMKC_PLATFORM_LINUX)
    wmkc_s32 errCodeBySystem = errno;
#   endif
    wmkcErr_obj error;

    switch(errCodeBySystem) {
        case WMKC_NET_ERR_EINTR:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The call executed was truncated by the operating system.");
        case WMKC_NET_ERR_EACCES:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "Accessing sockets in an impermissible manner.");
        case WMKC_NET_ERR_EFAULT:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The address points to the user's inaccessible address space.");
        case WMKC_NET_ERR_EINVAL:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "An invalid socket parameter was provided.");
        case WMKC_NET_ERR_EMFILE:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "Unable to provide sockets, the process's limit on the number of sockets has been reached.");
        case WMKC_NET_ERR_EISCONN:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The socket has already been specified for connection.");
        case WMKC_NET_ERR_ENOBUFS:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "No buffer space provided.");
        case WMKC_NET_ERR_EALREADY:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "Another (TCP Fast Open) is executing or the socket is in non blocking mode.");
        case WMKC_NET_ERR_ENOTCONN:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The socket is already (not) connected.");
        case WMKC_NET_ERR_EMSGSIZE:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The message is greater than the limit supported by the transmission.");
        case WMKC_NET_ERR_ENOTSOCK:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The descriptor is not a socket. :)");
        case WMKC_NET_ERR_ETIMEDOUT:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The connection has been disconnected due to timeout.");
        case WMKC_NET_ERR_EPROTOTYPE:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The socket type does not support this operation.");
        case WMKC_NET_ERR_EOPNOTSUPP:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "This socket is not connection oriented, or it is unidirectional.");
        case WMKC_NET_ERR_EADDRINUSE:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The address or port is already (not) in use.");
        case WMKC_NET_ERR_ECONNRESET:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "Connection is reset");
        case WMKC_NET_ERR_ENETUNREACH:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The network is not available.");
        case WMKC_NET_ERR_EWOULDBLOCK:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The socket is non blocking and there is no connection.");
        case WMKC_NET_ERR_EINPROGRESS:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The socket is non blocking and cannot be executed immediately.");
        case WMKC_NET_ERR_ECONNREFUSED:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The connection was forcibly rejected by the remote host.");
        case WMKC_NET_ERR_ECONNABORTED:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The socket is unavailable due to timeout or other faults.");
        case WMKC_NET_ERR_EAFNOSUPPORT:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The address in the specified network family cannot be used with this socket.");
        case WMKC_NET_ERR_EADDRNOTAVAIL:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The requested address is invalid.");
        case WMKC_NET_ERR_EPROTONOSUPPORT:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The specified protocol is not supported.");
#       if defined(WMKC_PLATFORM_LINUX)
        case WMKC_NET_ERR_EIO:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "An I/O error occurred.");
        case WMKC_NET_ERR_EPIPE:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The local end has been closed on a connection oriented socket.");
        case WMKC_NET_ERR_EPERM:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "Firewall rules prohibit connections.");
        case WMKC_NET_ERR_EROFS:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The socket index node will reside on a read-only file system.");
        case WMKC_NET_ERR_EBADF:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The descriptor is not a socket.");
        case WMKC_NET_ERR_ELOOP:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "Too many Symbolic link were encountered while resolving addresses.");
        case WMKC_NET_ERR_EDQUOT:
        case WMKC_NET_ERR_ENOSPC:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The disk quota limit has been reached (insufficient storage space).");
        case WMKC_NET_ERR_ENOENT:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The component in the directory prefix of the socket path name does not exist.");
        case WMKC_NET_ERR_ENOMEM:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "Insufficient available kernel memory.");
        case WMKC_NET_ERR_ENFILE:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "Unable to provide sockets, the process's limit on the number of sockets has been reached.");
        case WMKC_NET_ERR_EPROTO:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "Wrong protocol.");
        case WMKC_NET_ERR_ENOTDIR:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The component with path prefix is not a directory.");
        case WMKC_NET_ERR_ENAMETOOLONG:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "addr is too long.");
        case WMKC_NET_ERR_EDESTADDRREQ:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The socket is not in connection mode and has no peer address set.");
#       elif defined(WMKC_PLATFORM_WINOS)
        case WMKC_NET_ERR_ENETDOWN:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The network subsystem has malfunctioned.");
        case WMKC_NET_ERR_ESHUTDOWN:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The socket has been closed by the shutdown function.");
        case WMKC_NET_ERR_EHOSTUNREACH:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "Unable to access the remote host at this time.");
        case WMKC_NET_ERR_NOTINITIALISED:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The call to the WSAStartup function was not completed.");
        case WMKC_NET_ERR_ESOCKTNOSUPPORT:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "This address family does not support the specified socket type.");
        case WMKC_NET_ERR_EINVALIDPROVIDER:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The service provider returned a version other than 2.2.");
        case WMKC_NET_ERR_EINVALIDPROCTABLE:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The service provider returned an invalid or incomplete procedure table to the WSPStartup.");
        case WMKC_NET_ERR_EPROVIDERFAILEDINIT:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "The service provider failed to initialize.");
#       endif

        default:
            wmkcErr_func_return(error, errCodeBySystem, funcName,
                "Unexpected error.");
    }
}

WMKC_PUBLIC(wmkcVoid) wmkcNet_sockaddr2ipaddr WMKC_OPEN_API
WMKC_OF((wmkc_s32 family, wmkcVoid *content, wmkcNet_addr *addr))
{
    wmkcMem_zero(addr->addr, INET6_ADDRSTRLEN);
    if(family == AF_INET) {
        wmkcNet_GetAddr(family, &((SOCKADDR_IN *)content)->sin_addr, addr->addr);
        addr->port = wmkcNet_GetPort(((SOCKADDR_IN *)content)->sin_port);
    } else if(family == AF_INET6) {
        wmkcNet_GetAddr(family, &((SOCKADDR_IN6 *)content)->sin6_addr, addr->addr);
        addr->port = wmkcNet_GetPort(((SOCKADDR_IN6 *)content)->sin6_port);
    }
}

WMKC_PUBLIC(wmkcCSTR) wmkcNet_GetAddr WMKC_OPEN_API
WMKC_OF((wmkc_s32 family, wmkcVoid *pAddr, wmkcChar *pStringBuf))
{
    return inet_ntop(family, pAddr, pStringBuf, INET6_ADDRSTRLEN);
}

WMKC_PUBLIC(wmkc_u16) wmkcNet_GetPort WMKC_OPEN_API
WMKC_OF((wmkc_u16 port))
{
    return ntohs(port);
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_new WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_new: obj is NULL.");
    }

    if(!wmkcMem_new(wmkcNet_obj *, (*obj), sizeof(wmkcNet_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcNet_new: Failed to allocate memory for (*obj).");
    }

    if(!wmkcMem_new(wmkcNet_addr *, (*obj)->laddr, sizeof(wmkcNet_addr))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcNet_new: Failed to allocate memory for (*obj)->laddr.");
    }

    if(!wmkcMem_new(wmkcNet_addr *, (*obj)->raddr, sizeof(wmkcNet_addr))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcNet_new: Failed to allocate memory for (*obj)->raddr.");
    }

    wmkcMem_zero((*obj)->laddr->addr, sizeof((*obj)->laddr->addr));
    (*obj)->laddr->sockAddress = wmkcNull;
    (*obj)->laddr->port = 0;

    wmkcMem_zero((*obj)->raddr->addr, sizeof((*obj)->raddr->addr));
    (*obj)->raddr->sockAddress = wmkcNull;
    (*obj)->raddr->port = 0;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_free WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_free: obj is NULL.");
    }

    if((*obj)->laddr) {
        wmkcMem_zero((*obj)->laddr->addr, sizeof((*obj)->laddr->addr));
        if((*obj)->laddr->sockAddress)
            wmkcMem_free((*obj)->laddr->sockAddress);
        wmkcMem_free((*obj)->laddr);
    }
    if((*obj)->raddr) {
        wmkcMem_zero((*obj)->raddr->addr, sizeof((*obj)->raddr->addr));
        if((*obj)->raddr->sockAddress)
            wmkcMem_free((*obj)->raddr->sockAddress);
        wmkcMem_free((*obj)->raddr);
    }
    wmkcMem_free((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_socket WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkc_s32 family, wmkc_s32 type, socklen_t proto))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_socket: obj is NULL.");
    }

    if((obj->sockfd = socket(family, type, proto)) == wmkcErr_Err32) {
        return wmkcNet_errorHandler("wmkcNet_socket");
    }

    obj->family = family; // 套接字家族
    obj->type = type;     // 套接字类型
    obj->proto = proto;   // 套接字协议

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_getaddrinfo WMKC_OPEN_API
WMKC_OF((wmkcNet_addr *addr, wmkcCSTR hostname, wmkc_u16 port,
    wmkc_s32 family, wmkc_s32 type, wmkc_s32 proto))
{
    wmkcErr_obj error;
    if(!addr || !hostname || !port) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_getaddrinfo: "
            "obj or hostname or port is NULL.");
    }

    // 根据指定的套接字信息来解析域名
    ADDRINFO *result = wmkcNull;
    ADDRINFO hints = {.ai_family = family, .ai_socktype = type, .ai_protocol = proto};

    // 将端口号转为字符串，用于指定getaddrinfo函数的服务名。
    // 如果不指定为端口的话，ADDRINFO结构中的sin_port很大概率会错误。
    wmkcChar servname[6] = {0};
    snprintf(servname, sizeof(servname), "%u", port);

    // 执行getaddrinfo函数并判断是否出错。
    switch(getaddrinfo(hostname, servname, &hints, &result)) {
        case EAI_AGAIN:
            wmkcErr_func_return(error, EAI_AGAIN, "wmkcNet_getaddrinfo",
                "A temporary failure in name resolution occurred.");
        case EAI_BADFLAGS:
            wmkcErr_func_return(error, EAI_AGAIN, "wmkcNet_getaddrinfo",
                "An invalid value was provided for the ai_flags member of the pHints parameter.");
        case EAI_FAIL:
            wmkcErr_func_return(error, EAI_AGAIN, "wmkcNet_getaddrinfo",
                "A nonrecoverable failure in name resolution occurred.");
        case EAI_FAMILY:
            wmkcErr_func_return(error, EAI_AGAIN, "wmkcNet_getaddrinfo",
                "The ai_family member of the pHints parameter is not supported.");
        case EAI_MEMORY:
            wmkcErr_func_return(error, EAI_AGAIN, "wmkcNet_getaddrinfo",
                "A memory allocation failure occurred.");
        case EAI_NONAME:
            wmkcErr_func_return(error, EAI_AGAIN, "wmkcNet_getaddrinfo",
                "The name does not resolve for the supplied parameters or the pNodeName and "
                "pServiceName parameters were not provided.");
        case EAI_SERVICE:
            wmkcErr_func_return(error, EAI_AGAIN, "wmkcNet_getaddrinfo",
                "The pServiceName parameter is not supported for the specified ai_socktype member of "
                "the pHints parameter.");
        case EAI_SOCKTYPE:
            wmkcErr_func_return(error, EAI_AGAIN, "wmkcNet_getaddrinfo",
                "The ai_socktype member of the pHints parameter is not supported.");
    }

    // 根据网络家族将网络地址结构转为字符串地址，设置端口号。
    wmkcNet_sockaddr2ipaddr(family, result->ai_addr, addr);

    // 如果指针不为空，那么先释放（防止内存泄露）再申请新的内存
    // 当然也可能出现另一种情况，那就是这个指针不指向空也不指向正确的地址，
    // 那就需要使用者自行处理了。
    if(addr->sockAddress)
        wmkcMem_free(addr->sockAddress);
    if(!wmkcMem_new(SOCKADDR *, addr->sockAddress, WMKC_NET_IPV6_ADDR_SIZE)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcNet_getaddrinfo: "
            "Failed to allocate memory for addr->sockAddress.");
    }

    // 先将网络地址结构的内存区域清零以防出现意外
    wmkcMem_zero(addr->sockAddress, WMKC_NET_IPV6_ADDR_SIZE);
    memcpy(addr->sockAddress, result->ai_addr, (addr->sockAddressSize = result->ai_addrlen));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_settimeout WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, double _val))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_settimeout: obj is NULL.");
    }

#   if defined(WMKC_PLATFORM_WINOS)
    DWORD _timeout = (DWORD)(_val * 1000);
    wmkcChar *optval = (wmkcChar *)&_timeout;
#   elif defined(WMKC_PLATFORM_LINUX)
    double intpart = 0;
    double fracpart = modf(_val, &intpart);
    struct timeval _timeout = {.tv_sec=(long)intpart, .tv_usec=(long)(fracpart * 1000000)};
    wmkcVoid *optval = (wmkcVoid *)&_timeout;
#   endif

    if(setsockopt(obj->sockfd, SOL_SOCKET, SO_SNDTIMEO, optval, sizeof(_timeout))) {
        return wmkcNet_errorHandler("wmkcNet_settimeout");
    }
    if(setsockopt(obj->sockfd, SOL_SOCKET, SO_RCVTIMEO, optval, sizeof(_timeout))) {
        return wmkcNet_errorHandler("wmkcNet_settimeout");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_bind WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcCSTR addr, wmkc_u16 port))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_bind: obj is NULL.");
    }

    // 将传入的地址解析（毕竟可能使用localhost作为地址）
    if((error = wmkcNet_getaddrinfo(obj->laddr, addr, port, obj->family, obj->type, obj->proto)).code)
        return error;

    if(bind(obj->sockfd, obj->laddr->sockAddress, obj->laddr->sockAddressSize)) {
        return wmkcNet_errorHandler("wmkcNet_bind");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_connect WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcCSTR addr, wmkc_u16 port))
{
    wmkcErr_obj error;
    if(!obj || !obj->raddr || !addr || !port) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_getaddrinfo: "
            "obj or obj->raddr or addr or port is NULL.");
    }

    // 将传入的地址解析（毕竟可能使用localhost作为地址）
    if((error = wmkcNet_getaddrinfo(obj->raddr, addr, port, obj->family, obj->type, obj->proto)).code)
        return error;

    if(connect(obj->sockfd, obj->raddr->sockAddress, obj->raddr->sockAddressSize)) {
        return wmkcNet_errorHandler("wmkcNet_connect");
    }

    if(obj->laddr->sockAddress)
        wmkcMem_free(obj->laddr->sockAddress);
    obj->laddr->sockAddressSize = WMKC_NET_IPV6_ADDR_SIZE;
    if(!wmkcMem_new(SOCKADDR *, obj->laddr->sockAddress, obj->laddr->sockAddressSize)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcNet_connect: "
            "Failed to allocate memory for obj->laddr->sockAddress.");
    }

    wmkcMem_zero(obj->laddr->sockAddress, obj->laddr->sockAddressSize);
    if(getsockname(obj->sockfd, obj->laddr->sockAddress, &obj->laddr->sockAddressSize)) {
        return wmkcNet_errorHandler("wmkcNet_connect");
    }
    wmkcNet_sockaddr2ipaddr(obj->family, obj->laddr->sockAddress, obj->laddr);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_listen WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkc_s32 backlog))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_listen: obj is NULL.");
    }

    if(listen(obj->sockfd, backlog)) {
        return wmkcNet_errorHandler("wmkcNet_listen");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_accept WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **dst, wmkcNet_obj *src))
{
    wmkcErr_obj error;
    if(!dst || !src) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcNet_accept",
            "dst or src is NULL.");
    }

    // 创建一个新的套接字用于接受连接
    if((error = wmkcNet_new(dst)).code) return error;

    (*dst)->laddr->sockAddressSize = WMKC_NET_IPV6_ADDR_SIZE;
    if(!wmkcMem_new(SOCKADDR *, (*dst)->laddr->sockAddress, (*dst)->laddr->sockAddressSize)) {
        wmkcErr_func_return(error, wmkcErr_ErrMemory, "wmkcNet_accept",
            "Failed to allocate memory for (*dst)->laddr->sockAddress.");
    }
    (*dst)->raddr->sockAddressSize = WMKC_NET_IPV6_ADDR_SIZE;
    if(!wmkcMem_new(SOCKADDR *, (*dst)->raddr->sockAddress, (*dst)->raddr->sockAddressSize)) {
        wmkcErr_func_return(error, wmkcErr_ErrMemory, "wmkcNet_accept",
            "Failed to allocate memory for (*dst)->raddr->sockAddress.");
    }

    (*dst)->sockfd = accept(src->sockfd, (*dst)->raddr->sockAddress, &((*dst)->raddr->sockAddressSize));
    if((*dst)->sockfd == wmkcErr_Err32) {
        return wmkcNet_errorHandler("wmkcNet_accept");
    }
    (*dst)->family = src->family;
    (*dst)->type = src->type;
    (*dst)->proto = src->proto;

    wmkcMem_zero((*dst)->laddr->sockAddress, (*dst)->laddr->sockAddressSize);
    if(getsockname((*dst)->sockfd, (*dst)->laddr->sockAddress, &(*dst)->laddr->sockAddressSize)) {
        return wmkcNet_errorHandler("wmkcNet_connect");
    }
    wmkcNet_sockaddr2ipaddr((*dst)->family, (*dst)->raddr->sockAddress, (*dst)->raddr);
    wmkcNet_sockaddr2ipaddr((*dst)->family, (*dst)->laddr->sockAddress, (*dst)->laddr);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_send WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT *content, socklen_t size, wmkc_s32 _flag))
{
    wmkcErr_obj error;
    if(!obj || !content || !size) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcNet_send",
            "dst or content or size is NULL.");
    }

    if((obj->tSize = send(obj->sockfd, content, size, _flag)) == wmkcErr_Err32) {
        return wmkcNet_errorHandler("wmkcNet_send");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_sendall WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT *content, wmkcSize size, wmkc_s32 _flag))
{
    wmkcErr_obj error;
    if(!obj || !content || !size) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcNet_sendall",
            "dst or content or size is NULL.");
    }
    wmkc_u32 retry_count; // 重试次数

    while(size) {
        /* 重传功能
        * 默认情况下，重传次数为5。
        * 但由于TCP协议自带重传功能，并且在不同操作系统中不一样，在Linux中
        * 为15次，在Windows中，基本上为3次或更多。
        * 那么实际的重传次数，可能是75 (15 x 5)或15 (3 x 5)次。
        * 如果5次都失败，那么会直接返回错误信息。
        * 
        * 成功传输的话，会根据每次传输的大小来分割数据包，直到传输完毕。
        */
        retry_count = 5;
        while(retry_count) {
            error = wmkcNet_send(obj, content, size, _flag);
            if(obj->tSize == wmkcErr_Err32) {
                retry_count--;
            } else {
                break;
            }
        }
        if(!retry_count) {
            return error;
        }

        size -= obj->tSize;
        content += obj->tSize;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_recv WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBufT *content, socklen_t size, wmkc_s32 _flag))
{
    wmkcErr_obj error;
    if(!obj || !content || !size) {
        wmkcErr_func_return(error, wmkcErr_ErrNULL, "wmkcNet_recv",
            "dst or content or size is NULL.");
    }

    if((obj->tSize = recv(obj->sockfd, content, size, _flag)) == wmkcErr_Err32) {
        return wmkcNet_errorHandler("wmkcNet_recv");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_shutdown WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkc_u32 how))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_shutdown: obj is NULL.");
    }

    if(shutdown(obj->sockfd, how) == wmkcErr_Err32) {
        return wmkcNet_errorHandler("wmkcNet_shutdown");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_close WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_close: obj is NULL.");
    }

#   if defined(WMKC_PLATFORM_WINOS)
    if(closesocket(obj->sockfd)) {
#   elif defined(WMKC_PLATFORM_LINUX)
    if(close(obj->sockfd)) {
#   endif
        return wmkcNet_errorHandler("wmkcNet_close");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
