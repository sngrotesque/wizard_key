#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <net/WukNetwork.hh>
#include <net/WukError.hh>

#ifdef WUK_STD_CPP_20
#   include <compare>
#endif

namespace wuk::net {
#   ifdef WUK_PLATFORM_WINOS
    using wSocket = SOCKET;
#   else
    using wSocket = wuk::i32;
#   endif

// 类类型声明
    class LIBWUK_API Addrinfo;
    class LIBWUK_API Sockaddr;
    class LIBWUK_API Socket;

// Addrinfo BEGIN
    class LIBWUK_API Addrinfo {
    private:
        addrinfo m_hints {};
        addrinfo *m_res = nullptr;

    public:
        Addrinfo(wuk::i32 family    = AF_INET,
                 wuk::i32 sock_type = SOCK_STREAM,
                 wuk::i32 proto     = IPPROTO_TCP) noexcept;
        ~Addrinfo();

    public:
        Addrinfo &resolve(const std::string &addr, const wuk::u16 &port);

    public:
        const sockaddr *get_addr() const;
        socklen_t get_addrlen() const;
        Sockaddr get_sockaddr() const;
    };

// Sockaddr BEGIN
    class LIBWUK_API Sockaddr {
    private:
        sockaddr_storage m_addr {};
        socklen_t m_addrlen = sizeof(m_addr);

    public:
        Sockaddr() = default;
        Sockaddr(const sockaddr_storage *addr, const socklen_t &addrlen);
        Sockaddr(const sockaddr *addr, const socklen_t &addrlen);
        ~Sockaddr();

    public:
        sockaddr *set_addr() noexcept;
        socklen_t *set_addrlen() noexcept;

        void set_addr(const sockaddr *addr, const socklen_t &addrlen);
        void set_addr(const Sockaddr &addr);

        const sockaddr *get_addr() const noexcept;
        socklen_t get_addrlen() const noexcept;

        const std::string get_address() const;
        wuk::u16 get_port() const;
    };

// Socket BEGIN
    class LIBWUK_API Socket {
    private:
        wSocket m_fd = static_cast<wSocket>(NETERROR);

        wuk::i32 m_family    = AF_INET;
        wuk::i32 m_sock_type = SOCK_STREAM;
        wuk::i32 m_proto     = IPPROTO_TCP;

        Sockaddr m_raddr;
        Sockaddr m_laddr;

        wuk::f64 m_timeout = 0;

        bool m_is_close = false;
        bool m_is_blocking = true;

    public:
        Socket() = default;
        Socket(wuk::i32 family, wuk::i32 sock_type, wuk::i32 proto);
        Socket(wuk::i32 family, wuk::i32 sock_type, wuk::i32 proto, wSocket other_fd);
        ~Socket();

        Socket(const Socket &other) = delete;
        Socket(Socket &&other) noexcept;
        Socket &operator=(const Socket &other) = delete;
        Socket &operator=(Socket &&other) noexcept;

    public:
#       ifdef WUK_STD_CPP_20
        std::strong_ordering operator<=>(const Socket &other) const noexcept;
#       else
        bool operator<(const Socket &other) const noexcept;
        bool operator<=(const Socket &other) const noexcept;
        bool operator>(const Socket &other) const noexcept;
        bool operator>=(const Socket &other) const noexcept;
        bool operator==(const Socket &other) const noexcept;
        bool operator!=(const Socket &other) const noexcept;
#       endif

    public:
        template <typename T>
        inline void setsockopt(wuk::i32 level, wuk::i32 opt_name, const T &value)
        {
            const char *opt_ptr = reinterpret_cast<const char *>(&value);
            socklen_t opt_len = static_cast<socklen_t>(sizeof(T));
            int err = ::setsockopt(this->m_fd, level, opt_name, opt_ptr, opt_len);
            if (err == NETERROR) {
                int err_code = wuk::net::err::system::code();
                throw wuk::Exception(err_code, "wuk::net::Socket::setsockopt",
                    wuk::net::err::system::message(err_code).c_str());
            }
        }

        template <typename T>
        inline T getsockopt(wuk::i32 level, wuk::i32 opt_name)
        {
            T value {};
            socklen_t opt_len = static_cast<socklen_t>(sizeof(T));
            int err = ::getsockopt(this->m_fd, level, opt_name,
                                reinterpret_cast<char *>(&value), &opt_len);
            if (err == NETERROR) {
                int err_code = wuk::net::err::system::code();
                throw wuk::Exception(err_code, "wuk::net::Socket::getsockopt",
                    wuk::net::err::system::message(err_code).c_str());
            }
            return value;
        }

        const Sockaddr getsockname();

        void set_blocking(bool blocked);
        bool get_blocking() const noexcept;

        void set_timeout(wuk::f64 t) noexcept;
        wuk::f64 get_timeout() const noexcept;

    public:
        // 阻塞套接字
        void connect(const std::string &addr, wuk::u16 port);
        void bind(const std::string &addr, wuk::u16 port);
        void listen(const socklen_t &backlog) const;
        Socket accept() const;
        wuk::ilong send(const std::string &buffer, wuk::i32 flag = 0) const;
        void sendall(const std::string &buffer, wuk::i32 flag = 0) const;
        wuk::ilong sendto(const std::string &buffer, const Sockaddr &addr, wuk::i32 flag = 0) const;
        std::string recv(const socklen_t &length, wuk::i32 flag = 0) const;
        std::string recvfrom(const socklen_t &length, Sockaddr &addr, wuk::i32 flag = 0) const;

        // 非阻塞套接字
        void connect_ex(const std::string &addr, wuk::u16 port);
        Socket accept_ex();
        wuk::ilong send_ex(const std::string &buffer, wuk::i32 flag = 0);
        std::string recv_ex(const socklen_t &length, wuk::i32 flag = 0);

        void shutdown(wuk::i32 how) const;
        void close();

    public:
        void set_raddr(const Sockaddr &addr);
        void set_laddr(const Sockaddr &addr);

        const Sockaddr &get_raddr() const noexcept;
        const Sockaddr &get_laddr() const noexcept;
    
    public:
        wSocket get_fd() const noexcept;
        bool is_valid() const noexcept;
        void mark_invalid() noexcept;
    };
}

#endif
