#include <network/wmkc_net_new.h>

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
WMKC_OF((wmkcNet_addr *addr, wmkcCSTR hostname, wmkcCSTR servname,
    wmkcNetType family, wmkcNetType type, wmkcNetType proto))
{
    wmkcErr_obj error;
    if(!addr || !hostname || !servname) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcNet_getaddrinfo: "
            "obj or hostname or servname is NULL.");
    }

    ADDRINFO *result = wmkcNull;
    ADDRINFO hints = {.ai_family = family, .ai_socktype = type, .ai_protocol = proto};

    if(getaddrinfo(hostname, servname, &hints, &result)) {
        wmkcErr_return(error, wmkcErr_NetDomainResolv,
            "wmkcNet_getaddrinfo: Domain Name System failed.");
    }

    if(family == AF_INET) {
        SOCKADDR_IN *ip = (SOCKADDR_IN *)result->ai_addr;
        addr->addr = wmkcNet_GetAddr(ip->sin_addr);
    } else if(family == AF_INET6) {
        SOCKADDR_IN6 *ip = (SOCKADDR_IN6 *)result->ai_addr;
        wmkcChar ipv6_string[INET6_ADDRSTRLEN] = {0};
        addr->addr = wmkcNet_GetAddr6(&ip->sin6_addr, ipv6_string);
    }
    addr->port = 0;

    addr->sockAddressSize = result->ai_addrlen;
    memcpy(addr->sockAddress, result->ai_addr, addr->sockAddressSize);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcNet_connect WMKC_OPEN_API
WMKC_OF((wmkcNet_obj *obj, wmkcCSTR addr, wmkc_u16 port))
{

}


