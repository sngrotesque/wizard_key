#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>

#if defined(WUK_PLATFORM_WINOS)
#   include <WS2tcpip.h>
#else
#   include <errno.h>
#   include <netdb.h>
#endif

#ifndef NETERROR
#   ifdef WUK_PLATFORM_WINOS
#       define NETERROR SOCKET_ERROR
#   else
#       define NETERROR EOF
#   endif
#endif

namespace wuk::net::err {
    enum class SocketType {
        // 共有
        ACCES,           // 权限不足（EACCES / WSAEACCES）
        BADF,            // 无效文件描述符（EBADF / WSAEBADF）
        INVAL,           // 参数无效（EINVAL / WSAEINVAL）
        FAULT,           // 地址指针错误（EFAULT / WSAEFAULT）
        INTR,            // 系统调用被中断（EINTR / WSAEINTR）
        NOTSOCK,         // 非 socket（ENOTSOCK / WSAENOTSOCK）
        NOTCONN,         // 未连接（ENOTCONN / WSAENOTCONN）
        ISCONN,          // 已连接（EISCONN / WSAEISCONN）
        INPROGRESS,      // 正在进行连接（EINPROGRESS / WSAEINPROGRESS）
        ALREADY,         // 已在进行连接（EALREADY / WSAEALREADY）
        SHUTDOWN,        // 套接字已关闭（ESHUTDOWN / WSAESHUTDOWN）
        ADDRINUSE,       // 地址已被占用（EADDRINUSE / WSAEADDRINUSE）
        ADDRNOTAVAIL,    // 地址不可用（EADDRNOTAVAIL / WSAEADDRNOTAVAIL）
        DESTADDRREQ,     // 需要目标地址（EDESTADDRREQ / WSAEDESTADDRREQ）
        AFNOSUPPORT,     // 地址族不支持（EAFNOSUPPORT / WSAEAFNOSUPPORT）
        PROTONOSUPPORT,  // 协议不支持（EPROTONOSUPPORT / WSAEPROTONOSUPPORT）
        PROTOTYPE,       // 协议类型错误（EPROTOTYPE / WSAEPROTOTYPE）
        NOPROTOOPT,      // 不支持的 socket 选项（ENOPROTOOPT / WSAENOPROTOOPT）
        OPNOTSUPP,       // 操作不支持（EOPNOTSUPP / WSAEOPNOTSUPP）
        PFNOSUPPORT,     // 协议族不支持（EPFNOSUPPORT / WSAEPFNOSUPPORT）
        NETDOWN,         // 网络不可用（ENETDOWN / WSAENETDOWN）
        NETUNREACH,      // 网络不可达（ENETUNREACH / WSAENETUNREACH）
        NETRESET,        // 网络连接重置（ENETRESET / WSAENETRESET）
        HOSTDOWN,        // 主机不可用（EHOSTDOWN / WSAEHOSTDOWN）
        HOSTUNREACH,     // 主机不可达（EHOSTUNREACH / WSAEHOSTUNREACH）
        CONNABORTED,     // 连接被中止（ECONNABORTED / WSAECONNABORTED）
        CONNRESET,       // 连接被重置（ECONNRESET / WSAECONNRESET）
        CONNREFUSED,     // 连接被拒绝（ECONNREFUSED / WSAECONNREFUSED）
        TIMEDOUT,        // 连接超时（ETIMEDOUT / WSAETIMEDOUT）
        TOOMANYREFS,     // 引用过多（ETOOMANYREFS / WSAETOOMANYREFS）
        NOBUFS,          // 缓冲区不足（ENOBUFS / WSAENOBUFS）
        MSGSIZE,         // 消息过大（EMSGSIZE / WSAEMSGSIZE）
        MFILE,           // 文件描述符耗尽（EMFILE / WSAEMFILE）
        WOULDBLOCK,      // 非阻塞操作会阻塞（EWOULDBLOCK / WSAEWOULDBLOCK）

        // 平台独有
        HOSTNOTFOUND,    // DNS 查询失败（WSAHOST_NOT_FOUND，仅 Windows）
        TRYAGAIN,        // DNS 暂时失败（WSATRY_AGAIN，仅 Windows）
        NORECOVERY,      // DNS 不可恢复错误（WSANO_RECOVERY，仅 Windows）
        NODATA,          // DNS 查询无数据（WSANO_DATA，仅 Windows）
        TYPENOTFOUND,    // 类型查找失败（WSATYPE_NOT_FOUND，仅 Windows）
        SERVICENOTFOUND, // 找不到服务（WSASERVICE_NOT_FOUND，仅 Windows）
        REFUSED,         // 数据库查询被拒绝（WSAEREFUSED，仅 Windows）
        CANCELLED,       // 查询被取消（WSAECANCELLED，仅 Windows）
        NOMORE,          // 无更多结果（WSAENOMORE，仅 Windows）
        DISCON,          // 正常断开连接（WSAEDISCON，仅 Windows）
        SOCKTNOSUPPORT,  // 不支持的 socket 类型（WSAESOCKTNOSUPPORT，仅 Windows）
        NOTINITIALISED,  // 未调用 WSAStartup（WSANOTINITIALISED，仅 Windows）
        SYSNOTREADY,     // 网络子系统未准备好（WSASYSNOTREADY，仅 Windows）

        NFILE,           // 系统级文件表耗尽（ENFILE，仅 Linux）
        NOSTR,           // 非 STREAMS 文件（ENOSTR，仅 Linux）
        NOSR,            // STREAMS 资源不足（ENOSR，仅 Linux）
        PIPE,            // 管道错误（EPIPE，仅 Linux）
        IO,              // I/O 错误（EIO，仅 Linux）
        NAMETOOLONG,     // 名称过长（ENAMETOOLONG，仅 Linux）
        NOTTY,           // 非终端设备（ENOTTY，仅 Linux）
        NOTEMPTY,        // 删除非空目录（ENOTEMPTY，仅 Linux）

        // 未知错误
        UNKNOWN          // 未知错误（默认 fallback）
    };

    inline SocketType from_code(int err) noexcept
    {
        switch (err) {
#           if defined(WUK_PLATFORM_WINOS)
            // Public
            case WSAEACCES:            return SocketType::ACCES;
            case WSAEADDRINUSE:        return SocketType::ADDRINUSE;
            case WSAEADDRNOTAVAIL:     return SocketType::ADDRNOTAVAIL;
            case WSAEAFNOSUPPORT:      return SocketType::AFNOSUPPORT;
            case WSAEWOULDBLOCK:       return SocketType::WOULDBLOCK;
            case WSAEALREADY:          return SocketType::ALREADY;
            case WSAEBADF:             return SocketType::BADF;
            case WSAECONNABORTED:      return SocketType::CONNABORTED;
            case WSAECONNREFUSED:      return SocketType::CONNREFUSED;
            case WSAECONNRESET:        return SocketType::CONNRESET;
            case WSAEDESTADDRREQ:      return SocketType::DESTADDRREQ;
            case WSAEFAULT:            return SocketType::FAULT;
            case WSAEHOSTDOWN:         return SocketType::HOSTDOWN;
            case WSAEHOSTUNREACH:      return SocketType::HOSTUNREACH;
            case WSAEINPROGRESS:       return SocketType::INPROGRESS;
            case WSAEINTR:             return SocketType::INTR;
            case WSAEINVAL:            return SocketType::INVAL;
            case WSAEMFILE:            return SocketType::MFILE;
            case WSAEMSGSIZE:          return SocketType::MSGSIZE;
            case WSAENETDOWN:          return SocketType::NETDOWN;
            case WSAENETRESET:         return SocketType::NETRESET;
            case WSAENETUNREACH:       return SocketType::NETUNREACH;
            case WSAENOBUFS:           return SocketType::NOBUFS;
            case WSAENOPROTOOPT:       return SocketType::NOPROTOOPT;
            case WSAENOTCONN:          return SocketType::NOTCONN;
            case WSAENOTSOCK:          return SocketType::NOTSOCK;
            case WSAEOPNOTSUPP:        return SocketType::OPNOTSUPP;
            case WSAEPFNOSUPPORT:      return SocketType::PFNOSUPPORT;
            case WSAEPROTONOSUPPORT:   return SocketType::PROTONOSUPPORT;
            case WSAEPROTOTYPE:        return SocketType::PROTOTYPE;
            case WSAESHUTDOWN:         return SocketType::SHUTDOWN;
            case WSAETIMEDOUT:         return SocketType::TIMEDOUT;
            case WSAETOOMANYREFS:      return SocketType::TOOMANYREFS;
            case WSAEISCONN:           return SocketType::ISCONN;
            // Private
            case WSATYPE_NOT_FOUND:    return SocketType::TYPENOTFOUND;
            case WSAHOST_NOT_FOUND:    return SocketType::HOSTNOTFOUND;
            case WSATRY_AGAIN:         return SocketType::TRYAGAIN;
            case WSANO_RECOVERY:       return SocketType::NORECOVERY;
            case WSANO_DATA:           return SocketType::NODATA;
            case WSASYSNOTREADY:       return SocketType::SYSNOTREADY;
            case WSANOTINITIALISED:    return SocketType::NOTINITIALISED;
            case WSAENOMORE:           return SocketType::NOMORE;
            case WSAECANCELLED:        return SocketType::CANCELLED;
            case WSASERVICE_NOT_FOUND: return SocketType::SERVICENOTFOUND;
            case WSAEREFUSED:          return SocketType::REFUSED;
            case WSAESOCKTNOSUPPORT:   return SocketType::SOCKTNOSUPPORT;
            case WSAEDISCON:           return SocketType::DISCON;
#           else
            // Public
            case EACCES:               return SocketType::ACCES;
            case EADDRINUSE:           return SocketType::ADDRINUSE;
            case EADDRNOTAVAIL:        return SocketType::ADDRNOTAVAIL;
            case EAFNOSUPPORT:         return SocketType::AFNOSUPPORT;
            // 由于在现代Linux中EAGAIN始终等于EWOULDBLOCK，所以不区分具体是哪个宏了。
            case EWOULDBLOCK:          return SocketType::WOULDBLOCK;
            case EALREADY:             return SocketType::ALREADY;
            case EBADF:                return SocketType::BADF;
            case ECONNABORTED:         return SocketType::CONNABORTED;
            case ECONNREFUSED:         return SocketType::CONNREFUSED;
            case ECONNRESET:           return SocketType::CONNRESET;
            case EDESTADDRREQ:         return SocketType::DESTADDRREQ;
            case EFAULT:               return SocketType::FAULT;
            case EHOSTDOWN:            return SocketType::HOSTDOWN;
            case EHOSTUNREACH:         return SocketType::HOSTUNREACH;
            case EINPROGRESS:          return SocketType::INPROGRESS;
            case EINTR:                return SocketType::INTR;
            case EINVAL:               return SocketType::INVAL;
            case EIO:                  return SocketType::IO;
            case EISCONN:              return SocketType::ISCONN;
            case EMFILE:               return SocketType::MFILE;
            case EMSGSIZE:             return SocketType::MSGSIZE;
            case ENETDOWN:             return SocketType::NETDOWN;
            case ENETRESET:            return SocketType::NETRESET;
            case ENETUNREACH:          return SocketType::NETUNREACH;
            case ENOBUFS:              return SocketType::NOBUFS;
            case ENOPROTOOPT:          return SocketType::NOPROTOOPT;
            case ENOTCONN:             return SocketType::NOTCONN;
            case ENOTSOCK:             return SocketType::NOTSOCK;
            case EOPNOTSUPP:           return SocketType::OPNOTSUPP;
            case EPFNOSUPPORT:         return SocketType::PFNOSUPPORT;
            case EPROTONOSUPPORT:      return SocketType::PROTONOSUPPORT;
            case EPROTOTYPE:           return SocketType::PROTOTYPE;
            case ESHUTDOWN:            return SocketType::SHUTDOWN;
            case ETIMEDOUT:            return SocketType::TIMEDOUT;
            case ETOOMANYREFS:         return SocketType::TOOMANYREFS;
            // Private
            case ENFILE:               return SocketType::NFILE;
            case ENOSR:                return SocketType::NOSR;
            case ENOSTR:               return SocketType::NOSTR;
            case ENOTTY:               return SocketType::NOTTY;
            case EPIPE:                return SocketType::PIPE;
            case ENOTEMPTY:            return SocketType::NOTEMPTY;
            case ENAMETOOLONG:         return SocketType::NAMETOOLONG;
#           endif
            default:                   return SocketType::UNKNOWN;
        }
    }
}

namespace wuk::net::err::system {
    // 获取当前错误码
    inline int code() noexcept
    {
#       if defined(WUK_PLATFORM_WINOS)
        return WSAGetLastError();
#       else
        return errno;
#       endif
    }

    // 获取指定错误码的描述
    inline std::string message(int code) noexcept
    {
#       if defined(WUK_PLATFORM_WINOS)
        char *msg_buf = nullptr;

        DWORD len = FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                                    FORMAT_MESSAGE_FROM_SYSTEM     |
                                    FORMAT_MESSAGE_IGNORE_INSERTS,
                                    nullptr,
                                    code,
                                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                    (LPSTR)&msg_buf,
                                    0,
                                    nullptr);

        std::string result = (len && msg_buf) ? std::string(msg_buf) : "Unknown error";
        if(msg_buf) {
            LocalFree(msg_buf);
        }
        return result;
#       else
        return std::string(strerror(code));
#       endif
    }

    // 获取当前错误码的描述
    inline std::string last_message() noexcept
    {
        return message(code());
    }
}

#endif

