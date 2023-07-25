#include <network/wmkc_net.h>

WMKC_PUBLIC(wmkcNetSocket) wmkcNet_TCP_Socket WMKC_OPEN_API
WMKC_OF((wmkcNetSize family, wmkcNetSize protocol))
{
    return socket(family, SOCK_STREAM, protocol);
}

WMKC_PUBLIC(wmkcNetSocket) wmkcNet_UDP_Socket WMKC_OPEN_API
WMKC_OF((wmkcNetSize family, wmkcNetSize protocol))
{
    return socket(family, SOCK_DGRAM, protocol);
}

WMKC_PUBLIC(wmkcChar *) wmkcNet_GetAddr WMKC_OPEN_API
WMKC_OF((struct in_addr addr))
{
    return inet_ntoa(addr);
}

WMKC_PUBLIC(wmkcCSTR) wmkcNet_GetAddr6 WMKC_OPEN_API
WMKC_OF((wmkcVoid *pAddr, wmkcChar *pStringBuf))
{
    return inet_ntop(AF_INET6, pAddr, pStringBuf, INET6_ADDRSTRLEN);
}

WMKC_PUBLIC(wmkc_u16) wmkcNet_GetPort WMKC_OPEN_API
WMKC_OF((wmkc_u16 port))
{
    return ntohs(port);
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_new WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **obj, const SSL_METHOD *_ssl_method, wmkcNetType family, wmkcBool UDP))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_new: obj is NULL.");
    }

    if(!wmkcMem_new(wmkcNet_obj *, (*obj), sizeof(wmkcNet_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcNet_new: Failed to allocate memory for (*obj).");
    }

    if(_ssl_method) {
        if(!wmkcMem_new(wmkcNet_ssl *, (*obj)->ssl_obj, sizeof(wmkcNet_ssl))) {
            wmkcErr_return(error, wmkcErr_ErrMemory,
                "wmkcNet_new: Failed to allocate memory for (*obj)->ssl_obj.");
        }
        if(!((*obj)->ssl_obj->ssl_ctx = SSL_CTX_new(_ssl_method))) {
            wmkcErr_return(error, wmkcErr_Err32, "wmkcNet_new: "
            "The return value after calling the SSL_CTX_new function is NULL.");
        }
        if(!((*obj)->ssl_obj->ssl = SSL_new((*obj)->ssl_obj->ssl_ctx))) {
            wmkcErr_return(error, wmkcErr_Err32, "wmkcNet_new: "
            "The return value after calling the SSL_new function is NULL.");
        }
    } else {
        (*obj)->ssl_obj = wmkcNull;
    }

    if(family != AF_INET && family != AF_INET6) {
        wmkcErr_return(error, wmkcErr_NetFamily,
            "wmkcNet_new: The type of socket must be AF_INET or AF_INET6.");
    }
    (*obj)->sockfdFamily = family;
    if(family == AF_INET) {
        (*obj)->addr_info_size = WMKC_NET_IPV4_ADDR_SIZE;
    } else {
        (*obj)->addr_info_size = WMKC_NET_IPV6_ADDR_SIZE;
    }
    if(!UDP) {
        (*obj)->sockfd = wmkcNet_TCP_Socket(family, IPPROTO_IP);
    } else {
        (*obj)->sockfd = wmkcNet_UDP_Socket(family, IPPROTO_IP);
    }
    if((*obj)->sockfd == wmkcErr_Err32) {
        wmkcErr_return(error, wmkcErr_NetSocket, "wmkcNet_new: Unable to create socket.");
    }
    (*obj)->sockfdType = WMKC_NET_TCP_TYPE + UDP;

    if(!wmkcMem_new(SOCKADDR *, (*obj)->addr_info, (*obj)->addr_info_size)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcNet_new: Failed to allocate memory for (*obj)->addr_info.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_free WMKC_OPEN_API
WMKC_OF((wmkcNet_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_new: obj is NULL.");
    }

    if((*obj)->addr_info) {
        wmkcMem_free((*obj)->addr_info);
    }
    if((*obj)->ssl_obj) {
        SSL_CTX_free((*obj)->ssl_obj->ssl_ctx);
        SSL_free((*obj)->ssl_obj->ssl);
        wmkcMem_free((*obj)->ssl_obj);
    }
    wmkcMem_free((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_getaddrinfo WMKC_OPEN_API
WMKC_OF((SOCKADDR *dst, wmkcCSTR hostname, wmkcNetType family))
{
    wmkcErr_obj error;
    if(!dst || !hostname) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcNet_getaddrinfo: dst or hostname is NULL.");
    }
    ADDRINFO *result = wmkcNull;
    ADDRINFO hints = {.ai_family = family};

    if(getaddrinfo(hostname, "echo", &hints, &result)) {
        wmkcErr_return(error, wmkcErr_NetDomainResolv,
            "wmkcNet_getaddrinfo: Domain Name System failed.");
    }
    memcpy(dst, result->ai_addr, result->ai_addrlen);

    freeaddrinfo(result); // 释放掉已结束使用的解析数据
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_init WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcCSTR hostname, wmkc_u16 port))
{
    wmkcErr_obj error;

    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_init: obj is NULL.");
    }
    if(obj->ssl_obj) {
        SSL_set_fd(obj->ssl_obj->ssl, obj->sockfd);
    }

    if(hostname && *hostname) {
        error = wmkcNet_getaddrinfo(obj->addr_info, hostname, obj->sockfdFamily);
        if(error.code) return error;

        if(obj->sockfdFamily == AF_INET) {
            SOCKADDR_IN *ipv4 = (SOCKADDR_IN *)obj->addr_info;
            ipv4->sin_port = htons(port);
        } else {
            SOCKADDR_IN6 *ipv6 = (SOCKADDR_IN6 *)obj->addr_info;
            ipv6->sin6_port = htons(port);
        }
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_timeout WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetTime _user_TimeOut))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_timeout: obj is NULL.");
    }

    if(_user_TimeOut) {
        wmkcNetSize optlen = sizeof(struct timeval);
#           if defined(WMKC_PLATFORM_WINOS)
        DWORD _timeout = (DWORD)(_user_TimeOut * 1000);
#           elif defined(WMKC_PLATFORM_LINUX)
        struct timeval _timeout = {
            .tv_sec = floor(_user_TimeOut),
            .tv_usec = (long)(modf(_user_TimeOut, &_user_TimeOut) * 1000)
        };
#           endif

        wmkcNetTimer *optval = (wmkcNetTimer *)&_timeout;
        if(setsockopt(obj->sockfd, SOL_SOCKET, SO_SNDTIMEO, optval, optlen) ||
            setsockopt(obj->sockfd, SOL_SOCKET, SO_RCVTIMEO, optval, optlen)) {
            wmkcErr_return(error, wmkcErr_NetSetSockOpt,
                "wmkcNet_timeout: Error in setsockopt function.");
        }
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_connect WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_connect: obj is NULL.");
    }
    if(obj->sockfdType == WMKC_NET_UDP_TYPE) {
        wmkcErr_return(error, wmkcErr_NetSockfdType,
            "wmkcNet_connect: Wrong socket type, currently not expected to be UDP.");
    }

    // 连接至接收端，失败就返回错误代码
    if(connect(obj->sockfd, obj->addr_info, obj->addr_info_size) == wmkcErr_Err32) {
        wmkcErr_return(error, wmkcErr_NetConnect,
            "wmkcNet_connect: Socket connection failed.");
    }
    if(obj->ssl_obj) {
        SSL_connect(obj->ssl_obj->ssl);
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_bind WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetTime _user_TimeOut))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_bind: obj is NULL.");
    }

    wmkcNetTimer *optval = wmkcNull;
    struct timeval _timeout;

    if(_user_TimeOut) {
        _timeout.tv_sec = floor(_user_TimeOut);
        _timeout.tv_usec = (long)(modf(_user_TimeOut, &_user_TimeOut) * 1000);
        optval = (wmkcNetTimer *)&_timeout;

        if(setsockopt(obj->sockfd, SOL_SOCKET, SO_REUSEADDR, optval, sizeof(_timeout))) {
            wmkcErr_return(error, wmkcErr_NetSetSockOpt,
                "wmkcNet_bind: Error in setsockopt function in wmkcNet_timeout function.");
        }
    }

    // 如果绑定失败就返回错误代码
    if(bind(obj->sockfd, obj->addr_info, obj->addr_info_size) == wmkcErr_Err32) {
        wmkcErr_return(error, wmkcErr_NetBind, "wmkcNet_bind: Socket binding failed.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_listen WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkc_u32 _Listen))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_listen: obj is NULL.");
    }

    // 监听连接，失败或者Socket类型不正确就返回错误代码
    if(obj->sockfdType == WMKC_NET_TCP_TYPE) {
        if(!_Listen) {
            _Listen = WMKC_NET_DEFAULT_LISTEN;
        }
        if(listen(obj->sockfd, _Listen) == wmkcErr_Err32) {
            wmkcErr_return(error, wmkcErr_NetListen,
                "wmkcNet_listen: Socket listening failed.");
        }
    } else {
        wmkcErr_return(error, wmkcErr_NetSockfdType,
            "wmkcNet_listen: Wrong socket type, currently not expected to be UDP.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_accept WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *dst, wmkcNet_obj *src))
{
    wmkcErr_obj error;
    if(!dst || !src) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_accept: dst or src is NULL.");
    }
    if(!dst->addr_info || !src->addr_info) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcNet_accept: dst->addr_info or src->addr_info is NULL.");
    }

    // 接收连接，失败就返回错误代码
    if(src->sockfdType == WMKC_NET_TCP_TYPE) {
        dst->sockfd = accept(src->sockfd, dst->addr_info, &dst->addr_info_size);
        if(dst->sockfd == wmkcErr_Err32) {
            wmkcErr_return(error, wmkcErr_NetAccept,
                "wmkcNet_accept: The socket failed to accept the connection.");
        }
    } else {
        wmkcErr_return(error, wmkcErr_NetSockfdType,
            "wmkcNet_accept: Wrong socket type, currently not expected to be UDP.");
    }
    if(src->ssl_obj) {
        SSL_accept(src->ssl_obj->ssl);
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_send WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetSize *_tSize, wmkcNetBuf *buf, wmkcNetSize size))
{
    wmkcErr_obj error;
    if(!obj || !buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_send: obj or buf or size is NULL.");
    }
    if(!_tSize) {
        wmkcNetSize size;
        _tSize = &size;
    }

    if(obj->sockfdType == WMKC_NET_TCP_TYPE) {
        if(obj->ssl_obj) {
            *_tSize = SSL_write(obj->ssl_obj->ssl, buf, size);
        } else {
            *_tSize = send(obj->sockfd, buf, size, 0);
        }
    } else if(obj->sockfdType == WMKC_NET_UDP_TYPE) {
        *_tSize = sendto(obj->sockfd, buf, size, 0, obj->addr_info, obj->addr_info_size);
    } else {
        wmkcErr_return(error, wmkcErr_NetSockfdType,
                "wmkcNet_send: Wrong socket type, expected TCP or UDP.");
    }

    if(*_tSize == wmkcErr_Err32 || *_tSize == 0) {
        wmkcErr_return(error, wmkcErr_NetSend,
            "wmkcNet_send: The socket failed to send data.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_sendall WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetBuf *buf, wmkcNetSize size))
{
    wmkcErr_obj error;
    if(!obj || !buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcNet_sendall: obj or buf or size is NULL.");
    }
    wmkcNetSize index, quotient, leftover, _tSize;

    // 计算需要发送WMKC_NET_BLOCKLEN大小的数据包的次数
    // 计算是否还有剩下的数据包
    quotient = size / WMKC_NET_BLOCKLEN;
    leftover = size % WMKC_NET_BLOCKLEN;

    for(index = 0; index < quotient; ++index) {
        error = wmkcNet_send(obj, &_tSize, buf, WMKC_NET_BLOCKLEN);
        if(error.code) return error;
        buf += WMKC_NET_BLOCKLEN;
    }

    if(leftover) {
        error = wmkcNet_send(obj, &_tSize, buf, leftover);
        if(error.code) return error;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_recv WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcNetSize *_tSize, wmkcNetBuf *buf, wmkcNetSize size))
{
    wmkcErr_obj error;
    if(!obj || !buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_send: obj or buf or size is NULL.");
    }
    if(!_tSize) {
        wmkcNetSize size;
        _tSize = &size;
    }

    if(obj->sockfdType == WMKC_NET_TCP_TYPE) {
        if(obj->ssl_obj) {
            *_tSize = SSL_read(obj->ssl_obj->ssl, buf, size);
        } else {
            *_tSize = recv(obj->sockfd, buf, size, 0);
        }
    } else if(obj->sockfdType == WMKC_NET_UDP_TYPE) {
        *_tSize = recvfrom(obj->sockfd, buf, size, 0, obj->addr_info, &obj->addr_info_size);
    } else {
        wmkcErr_return(error, wmkcErr_NetSockfdType,
                "wmkcNet_send: Wrong socket type, expected TCP or UDP.");
    }

    if(*_tSize == wmkcErr_Err32 || *_tSize == 0) {
        wmkcErr_return(error, wmkcErr_NetSend,
            "wmkcNet_send: The socket failed to send data.");
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

    // 判断系统类型，根据对应系统使用不同的关闭函数将Socket与WSADATA进行关闭
    // 如果错误，就返回错误代码
#if defined(WMKC_PLATFORM_LINUX)
    if((shutdown(obj->sockfd, 2) | close(obj->sockfd)) == wmkcErr_Err32) {
        wmkcErr_return(error, wmkcErr_NetClose, "wmkcNet_close: Socket close failed.");
    }
#elif defined(WMKC_PLATFORM_WINOS)
    if(closesocket(obj->sockfd) == wmkcErr_Err32) {
        wmkcErr_return(error, wmkcErr_NetClose, "wmkcNet_close: Socket close failed.");
    }
#endif

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
