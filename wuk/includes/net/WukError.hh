#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
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

namespace wuk::net {
    enum class SocketError {
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

    inline SocketError from_code(int err)
    {
        switch (err) {
#           if defined(WUK_PLATFORM_WINOS)
            // Public
            case WSAEACCES: return SocketError::ACCES;
            case WSAEADDRINUSE: return SocketError::ADDRINUSE;
            case WSAEADDRNOTAVAIL: return SocketError::ADDRNOTAVAIL;
            case WSAEAFNOSUPPORT: return SocketError::AFNOSUPPORT;
            case WSAEWOULDBLOCK: return SocketError::WOULDBLOCK;
            case WSAEALREADY: return SocketError::ALREADY;
            case WSAEBADF: return SocketError::BADF;
            case WSAECONNABORTED: return SocketError::CONNABORTED;
            case WSAECONNREFUSED: return SocketError::CONNREFUSED;
            case WSAECONNRESET: return SocketError::CONNRESET;
            case WSAEDESTADDRREQ: return SocketError::DESTADDRREQ;
            case WSAEFAULT: return SocketError::FAULT;
            case WSAEHOSTDOWN: return SocketError::HOSTDOWN;
            case WSAEHOSTUNREACH: return SocketError::HOSTUNREACH;
            case WSAEINPROGRESS: return SocketError::INPROGRESS;
            case WSAEINTR: return SocketError::INTR;
            case WSAEINVAL: return SocketError::INVAL;
            case WSAEMFILE: return SocketError::MFILE;
            case WSAEMSGSIZE: return SocketError::MSGSIZE;
            case WSAENETDOWN: return SocketError::NETDOWN;
            case WSAENETRESET: return SocketError::NETRESET;
            case WSAENETUNREACH: return SocketError::NETUNREACH;
            case WSAENOBUFS: return SocketError::NOBUFS;
            case WSAENOPROTOOPT: return SocketError::NOPROTOOPT;
            case WSAENOTCONN: return SocketError::NOTCONN;
            case WSAENOTSOCK: return SocketError::NOTSOCK;
            case WSAEOPNOTSUPP: return SocketError::OPNOTSUPP;
            case WSAEPFNOSUPPORT: return SocketError::PFNOSUPPORT;
            case WSAEPROTONOSUPPORT: return SocketError::PROTONOSUPPORT;
            case WSAEPROTOTYPE: return SocketError::PROTOTYPE;
            case WSAESHUTDOWN: return SocketError::SHUTDOWN;
            case WSAETIMEDOUT: return SocketError::TIMEDOUT;
            case WSAETOOMANYREFS: return SocketError::TOOMANYREFS;
            case WSAEISCONN: return SocketError::ISCONN;
            // Private
            case WSATYPE_NOT_FOUND: return SocketError::TYPENOTFOUND;
            case WSAHOST_NOT_FOUND: return SocketError::HOSTNOTFOUND;
            case WSATRY_AGAIN: return SocketError::TRYAGAIN;
            case WSANO_RECOVERY: return SocketError::NORECOVERY;
            case WSANO_DATA: return SocketError::NODATA;
            case WSASYSNOTREADY: return SocketError::SYSNOTREADY;
            case WSANOTINITIALISED: return SocketError::NOTINITIALISED;
            case WSAENOMORE: return SocketError::NOMORE;
            case WSAECANCELLED: return SocketError::CANCELLED;
            case WSASERVICE_NOT_FOUND: return SocketError::SERVICENOTFOUND;
            case WSAEREFUSED: return SocketError::REFUSED;
            case WSAESOCKTNOSUPPORT: return SocketError::SOCKTNOSUPPORT;
            case WSAEDISCON: return SocketError::DISCON;
#           else
            // Public
            case EACCES: return SocketError::ACCES;
            case EADDRINUSE: return SocketError::ADDRINUSE;
            case EADDRNOTAVAIL: return SocketError::ADDRNOTAVAIL;
            case EAFNOSUPPORT: return SocketError::AFNOSUPPORT;
            case EAGAIN: return SocketError::WOULDBLOCK;
            case EALREADY: return SocketError::ALREADY;
            case EBADF: return SocketError::BADF;
            case ECONNABORTED: return SocketError::CONNABORTED;
            case ECONNREFUSED: return SocketError::CONNREFUSED;
            case ECONNRESET: return SocketError::CONNRESET;
            case EDESTADDRREQ: return SocketError::DESTADDRREQ;
            case EFAULT: return SocketError::FAULT;
            case EHOSTDOWN: return SocketError::HOSTDOWN;
            case EHOSTUNREACH: return SocketError::HOSTUNREACH;
            case EINPROGRESS: return SocketError::INPROGRESS;
            case EINTR: return SocketError::INTR;
            case EINVAL: return SocketError::INVAL;
            case EIO: return SocketError::IO;
            case EISCONN: return SocketError::ISCONN;
            case EMFILE: return SocketError::MFILE;
            case EMSGSIZE: return SocketError::MSGSIZE;
            case ENETDOWN: return SocketError::NETDOWN;
            case ENETRESET: return SocketError::NETRESET;
            case ENETUNREACH: return SocketError::NETUNREACH;
            case ENOBUFS: return SocketError::NOBUFS;
            case ENOPROTOOPT: return SocketError::NOPROTOOPT;
            case ENOTCONN: return SocketError::NOTCONN;
            case ENOTSOCK: return SocketError::NOTSOCK;
            case EOPNOTSUPP: return SocketError::OPNOTSUPP;
            case EPFNOSUPPORT: return SocketError::PFNOSUPPORT;
            case EPROTONOSUPPORT: return SocketError::PROTONOSUPPORT;
            case EPROTOTYPE: return SocketError::PROTOTYPE;
            case ESHUTDOWN: return SocketError::SHUTDOWN;
            case ETIMEDOUT: return SocketError::TIMEDOUT;
            case ETOOMANYREFS: return SocketError::TOOMANYREFS;
            case EWOULDBLOCK: return SocketError::WOULDBLOCK;
            // Private
            case ENFILE: return SocketError::NFILE;
            case ENOSR: return SocketError::NOSR;
            case ENOSTR: return SocketError::NOSTR;
            case ENOTTY: return SocketError::NOTTY;
            case EPIPE: return SocketError::PIPE;
            case ENOTEMPTY: return SocketError::NOTEMPTY;
            case ENAMETOOLONG: return SocketError::NAMETOOLONG;
#           endif
            default: return SocketError::UNKNOWN;
        }
    }

    class SystemError {
    public:
        // 获取当前错误码
        static inline int code()
        {
#           if defined(WUK_PLATFORM_WINOS)
            return WSAGetLastError();
#           else
            return errno;
#           endif
        }

        // 获取指定错误码的描述
        static inline std::string message(int code)
        {
#           if defined(WUK_PLATFORM_WINOS)
            char *msg_buf = nullptr;
            DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;

            DWORD len = FormatMessageA(flags,
                nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPSTR)&msg_buf, 0, nullptr);

            std::string result = (len && msg_buf) ? std::string(msg_buf) : "Unknown error";
            if(msg_buf) {
                LocalFree(msg_buf);
            }
            return result;
#           else
            return std::string(strerror(code));
#           endif
        }

        // 获取当前错误码的描述
        static inline std::string last_message()
        {
            return message(code());
        }
    };
}

#endif

