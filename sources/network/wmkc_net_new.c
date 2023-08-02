#include <network/wmkc_net_new.h>

WMKC_PUBLIC(wmkcCSTR) wmkcNet_GetAddr WMKC_OPEN_API
WMKC_OF((wmkcNetType family, wmkcVoid *pAddr, wmkcChar *pStringBuf))
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

    (*obj)->laddr->addr = wmkcNull;
    (*obj)->laddr->sockAddress = wmkcNull;

    (*obj)->raddr->addr = wmkcNull;
    (*obj)->raddr->sockAddress = wmkcNull;

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
        if((*obj)->laddr->addr)
            wmkcMem_free((*obj)->laddr->addr);
        if((*obj)->laddr->sockAddress)
            wmkcMem_free((*obj)->laddr->sockAddress);

        wmkcMem_free((*obj)->laddr);
    }

    if((*obj)->raddr) {
        if((*obj)->raddr->addr)
            wmkcMem_free((*obj)->raddr->addr);
        if((*obj)->raddr->sockAddress)
            wmkcMem_free((*obj)->raddr->sockAddress);

        wmkcMem_free((*obj)->raddr);
    }

    wmkcMem_free((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_socket WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetType family, wmkcNetType type, wmkcNetSize proto))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_socket: obj is NULL.");
    }

    obj->sockfd = socket(family, type, proto);
    obj->type = type;
    obj->family = family;
    obj->proto = proto;

    if(obj->sockfd == wmkcErr_Err32) {
        wmkcErr_return(error, wmkcErr_NetSocket, "wmkcNet_socket: Unable to create socket.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_getaddrinfo WMKC_OPEN_API
WMKC_OF((wmkcNet_addr *addr, wmkcCSTR hostname, wmkc_u16 port,
    wmkcNetType family, wmkcNetType type, wmkcNetType proto))
{
    wmkcErr_obj error;
    if(!addr || !hostname || !port) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_getaddrinfo: "
            "obj or hostname or port is NULL.");
    }

    ADDRINFO *result = wmkcNull;
    ADDRINFO hints = {.ai_family = family, .ai_socktype = type, .ai_protocol = proto};

    wmkcChar servname[6] = {0};
    sprintf(servname, "%u", port);
    if(getaddrinfo(hostname, servname, &hints, &result)) {
        wmkcErr_return(error, wmkcErr_NetDomainResolv,
            "wmkcNet_getaddrinfo: Domain Name System failed.");
    }

    if(addr->addr) {
        wmkcMem_free(addr->addr);
    }
    if(!wmkcMem_new(wmkcChar *, addr->addr, INET6_ADDRSTRLEN)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcNet_getaddrinfo: Failed to allocate memory for addr->addr.");
    }
    wmkcMem_zero(addr->addr, INET6_ADDRSTRLEN);

    if(family == AF_INET) {
        SOCKADDR_IN *ip = (SOCKADDR_IN *)result->ai_addr;
        wmkcNet_GetAddr(family, &ip->sin_addr, addr->addr);
    } else if(family == AF_INET6) {
        SOCKADDR_IN6 *ip = (SOCKADDR_IN6 *)result->ai_addr;
        wmkcNet_GetAddr(family, &ip->sin6_addr, addr->addr);
    }
    addr->port = port;

    if(addr->sockAddress) {
        wmkcMem_free(addr->sockAddress);
    }
    if(!wmkcMem_new(SOCKADDR *, addr->sockAddress, WMKC_NET_IPV6_ADDR_SIZE)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcNet_getaddrinfo: "
            "Failed to allocate memory for addr->sockAddress.");
    }
    wmkcMem_zero(addr->sockAddress, WMKC_NET_IPV6_ADDR_SIZE);

    addr->sockAddressSize = result->ai_addrlen;
    memcpy(addr->sockAddress, result->ai_addr, addr->sockAddressSize);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_settimeout WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, double _value))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_settimeout: obj is NULL.");
    }

    if(_value) {
#       if defined(WMKC_PLATFORM_WINOS)
        DWORD _timeout = (DWORD)(_value * 1000);
#       elif defined(WMKC_PLATFORM_LINUX)
        double intpart = 0;
        double fracpart = modf(_value, &intpart);
        struct timeval _timeout = {.tv_sec=(long)intpart, .tv_usec=(long)(fracpart * 1000000)};
#       endif

        wmkcNetTimer *optval = (wmkcNetTimer *)&_timeout;
        if(setsockopt(obj->sockfd, SOL_SOCKET, SO_SNDTIMEO, optval, sizeof(_timeout))) {
            wmkcErr_return(error, wmkcErr_Err32, "wmkcNet_settimeout: "
                "Error setting send timeout using the setsockopt function.");
        }
        if(setsockopt(obj->sockfd, SOL_SOCKET, SO_RCVTIMEO, optval, sizeof(_timeout))) {
            wmkcErr_return(error, wmkcErr_Err32, "wmkcNet_settimeout: "
                "Error setting receive timeout using the setsockopt function.");
        }
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

    error = wmkcNet_getaddrinfo(obj->laddr, addr, port, obj->family, obj->type, obj->proto);
    if(error.code) return error;

    if(bind(obj->sockfd, obj->laddr->sockAddress, obj->laddr->sockAddressSize)) {
        wmkcErr_return(error, wmkcErr_NetBind, "wmkcNet_bind: "
            "Binding host address to port failed.");
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

    if(obj->type == SOCK_DGRAM || obj->type == SOCK_RAW) {
        wmkcErr_return(error, wmkcErr_NetConnect, "wmkcNet_connect: "
            "Please do not use sockets that are oriented towards no connections.");
    }

    error = wmkcNet_getaddrinfo(obj->raddr, addr, port, obj->family, obj->type, obj->proto);
    if(error.code) return error;

    if(connect(obj->sockfd, obj->raddr->sockAddress, obj->raddr->sockAddressSize)) {
        wmkcErr_return(error, wmkcErr_NetConnect, "wmkcNet_connect: "
            "The socket connection to the target host failed.");
    }

    if(obj->laddr->sockAddress) {
        wmkcMem_free(obj->laddr->sockAddress);
    }
    obj->laddr->sockAddressSize = WMKC_NET_IPV6_ADDR_SIZE;
    if(!wmkcMem_new(SOCKADDR *, obj->laddr->sockAddress, obj->laddr->sockAddressSize)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcNet_connect: "
            "Failed to allocate memory for obj->laddr->sockAddress.");
    }

    if(getsockname(obj->sockfd, obj->laddr->sockAddress, &obj->laddr->sockAddressSize)) {
        wmkcErr_return(error, wmkcErr_NetConnect, "wmkcNet_connect: "
            "The getsockname function returned an error code.");
    }

    if(obj->laddr->addr) {
        wmkcMem_free(obj->laddr->addr);
    }
    if(!wmkcMem_new(wmkcChar *, obj->laddr->addr, INET6_ADDRSTRLEN)) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "wmkcNet_connect: "
            "Failed to allocate memory for obj->laddr->addr.");
    }
    if(obj->family == AF_INET) {
        SOCKADDR_IN *info = (SOCKADDR_IN *)obj->laddr->sockAddress;
        wmkcNet_GetAddr(AF_INET, &info->sin_addr, obj->laddr->addr);
        obj->laddr->port = ntohs(info->sin_port);
    } else if(obj->family == AF_INET6) {
        SOCKADDR_IN6 *info = (SOCKADDR_IN6 *)obj->laddr->sockAddress;
        wmkcNet_GetAddr(AF_INET6, &info->sin6_addr, obj->laddr->addr);
        obj->laddr->port = ntohs(info->sin6_port);
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_listen WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj))
{

}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_accept WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj))
{

}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_send WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj))
{

}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_sendall WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj))
{

}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_recv WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj))
{

}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_shutdown WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkc_u32 how))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_shutdown: obj is NULL.");
    }
    wmkc_s32 wmkcNet_shutdown_err_code;

    if(shutdown(obj->sockfd, how) == wmkcErr_Err32) {
#       if defined(WMKC_PLATFORM_WINOS)
        wmkcNet_shutdown_err_code = WSAGetLastError();
#       elif defined(WMKC_PLATFORM_LINUX)
        wmkcNet_shutdown_err_code = errno;
#       endif
        switch (wmkcNet_shutdown_err_code) {
            case WMKC_NET_SHUTDOWN_ERR_EINVAL:
                wmkcErr_return(error, wmkcErr_NetShutdown, "wmkcNet_shutdown: "
                    "The parameter is invalid or inconsistent with the socket type.");
            case WMKC_NET_SHUTDOWN_ERR_ENOTCONN:
                wmkcErr_return(error, wmkcErr_NetShutdown, "wmkcNet_shutdown: "
                    "The socket is not connected. This error only applies to connection oriented sockets.");
            case WMKC_NET_SHUTDOWN_ERR_ENOTSOCK:
                wmkcErr_return(error, wmkcErr_NetShutdown, "wmkcNet_shutdown: "
                    "The socket argument does not refer to a socket.");
            default:
                wmkcErr_return(error, wmkcErr_NetShutdown, "wmkcNet_shutdown: "
                    "Other errors occurred after calling the shutdown function.");
        }
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

    wmkc_s32 close_err_code = 0;
#   if defined(WMKC_PLATFORM_WINOS)
    close_err_code = closesocket(obj->sockfd);
#   elif defined(WMKC_PLATFORM_LINUX)
    close_err_code = close(obj->sockfd);
#   endif
    if(close_err_code) {
        wmkcErr_return(error, wmkcErr_NetClose, "wmkcNet_close: "
            "An error occurred while closing the socket.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
