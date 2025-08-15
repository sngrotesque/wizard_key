#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <net/WukNetwork.hh>
#include <net/WukError.hh>

#ifdef WUK_STD_CPP_20
#   include <compare>
#endif

namespace wuk::net {
#   ifdef WUK_PLATFORM_WINOS
    using wSocket = SOCKET;
#   else
    using wSocket = wI32;
#   endif

// 类类型声明
    class LIBWUK_API WukAddrinfo;
    class LIBWUK_API WukSockaddr;
    class LIBWUK_API WukSocket;

// WukAddrinfo BEGIN
    class LIBWUK_API WukAddrinfo {
    private:
        addrinfo hints {0};
        addrinfo *res = nullptr;

    public:
        WukAddrinfo(wI32 family = AF_INET, wI32 sock_type = SOCK_STREAM, wI32 proto = IPPROTO_TCP);
        ~WukAddrinfo();

    public:
        WukAddrinfo &resolve(const std::string &addr, const wU16 &port);

    public:
        const sockaddr *get_addr() const;
        socklen_t get_addrlen() const;
        WukSockaddr get_sockaddr() const;
    };

// WukSockaddr BEGIN
    class LIBWUK_API WukSockaddr {
    private:
        sockaddr_storage addr {0};
        socklen_t addrlen = sizeof(addr);

    public:
        WukSockaddr() = default;
        WukSockaddr(const sockaddr_storage *addr, const socklen_t &addrlen);
        WukSockaddr(const sockaddr *addr, const socklen_t &addrlen);
        ~WukSockaddr();

    public:
        sockaddr *set_addr() noexcept;
        socklen_t *set_addrlen() noexcept;

        void set_addr(const sockaddr *addr, const socklen_t &addrlen);
        void set_addr(const WukSockaddr &addr);

        const sockaddr *get_addr() const noexcept;
        socklen_t get_addrlen() const noexcept;

        const std::string get_address_string() const;
        wU16 get_port() const;
    };

// WukSocket BEGIN
    class LIBWUK_API WukSocket {
    private:
        wSocket fd = static_cast<wSocket>(NETERROR);

        wI32 m_family    = AF_INET;
        wI32 m_sock_type = SOCK_STREAM;
        wI32 m_proto     = IPPROTO_TCP;

        WukSockaddr m_raddr;
        WukSockaddr m_laddr;

        double m_timeout = 0;

        bool is_close = false;
        bool is_blocking = true;

    public:
        WukSocket() = default;
        WukSocket(wI32 family, wI32 sock_type, wI32 proto);
        WukSocket(wI32 family, wI32 sock_type, wI32 proto, wSocket cur_fd);
        ~WukSocket();

        WukSocket(WukSocket &&other) noexcept;
        WukSocket &operator=(WukSocket &&other) noexcept;

    public:
#       ifdef WUK_STD_CPP_20
        std::strong_ordering operator<=>(const WukSocket &other) const;
#       else
        bool operator<(const WukSocket &other) const;
        bool operator<=(const WukSocket &other) const;
        bool operator>(const WukSocket &other) const;
        bool operator>=(const WukSocket &other) const;
        bool operator==(const WukSocket &other) const;
        bool operator!=(const WukSocket &other) const;
#       endif

    public:
        template <typename T>
        inline void setsockopt(wI32 level, wI32 opt_name, const T &value)
        {
            const char *opt_ptr = reinterpret_cast<const char *>(&value);
            socklen_t opt_len = static_cast<socklen_t>(sizeof(T));
            int err = ::setsockopt(this->fd, level, opt_name, opt_ptr, opt_len);
            if (err == NETERROR) {
                int err_code = wuk::net::SystemError::code();
                throw wuk::Exception(err_code, "wuk::net::WukSocket::setsockopt",
                    wuk::net::SystemError::message(err_code).c_str());
            }
        }

        template <typename T>
        inline T getsockopt(wI32 level, wI32 opt_name)
        {
            T value {};
            socklen_t opt_len = static_cast<socklen_t>(sizeof(T));
            int err = ::getsockopt(this->fd, level, opt_name,
                                reinterpret_cast<char *>(&value), &opt_len);
            if (err == NETERROR) {
                int err_code = wuk::net::SystemError::code();
                throw wuk::Exception(err_code, "wuk::net::WukSocket::getsockopt",
                    wuk::net::SystemError::message(err_code).c_str());
            }
            return value;
        }

        const WukSockaddr getsockname();

        void set_blocking(bool blocked);
        bool get_blocking() const noexcept;

        void set_timeout(double t) noexcept;
        double get_timeout() const noexcept;

    public:
        // 阻塞套接字
        void connect(const std::string &addr, const wU16 &port);
        void bind(const std::string &addr, const wU16 &port);
        void listen(const socklen_t &backlog);
        WukSocket accept() const;
        wSSize send(const std::string &buffer, wI32 flag = 0);
        void sendall(const std::string &buffer, wI32 flag = 0);
        wSSize sendto(const std::string &buffer, const WukSockaddr &addr, wI32 flag = 0);
        std::string recv(const socklen_t &length, wI32 flag = 0);
        std::string recvfrom(const socklen_t &length, WukSockaddr &addr, wI32 flag = 0);

        // 非阻塞套接字
        void connect_ex(const std::string &addr, const wU16 &port);
        WukSocket accept_ex();
        wSSize send_ex(const std::string &buffer, wI32 flag = 0);
        std::string recv_ex(const socklen_t &length, wI32 flag = 0);

        void shutdown(const wI32 &how);
        void close();

    public:
        void set_raddr(const WukSockaddr &addr);
        void set_laddr(const WukSockaddr &addr);

        const WukSockaddr &get_raddr() const noexcept;
        const WukSockaddr &get_laddr() const noexcept;
    
    public:
        wSocket get_fd() const noexcept;
        bool is_valid() const noexcept;
        void mark_invalid() noexcept;
    };
}

#endif
