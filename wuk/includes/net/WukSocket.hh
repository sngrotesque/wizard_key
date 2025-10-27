#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <net/WukNetwork.hh>
#include <net/WukError.hh>
#include <WukBuffer.hh>

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

        Addrinfo(const Addrinfo &other) = delete;
        Addrinfo(Addrinfo &&other) noexcept;
        Addrinfo &operator=(const Addrinfo &other) = delete;
        Addrinfo &operator=(Addrinfo &&other) noexcept;

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
        Sockaddr(const sockaddr_storage *addr, socklen_t addrlen);
        Sockaddr(const sockaddr *addr, socklen_t addrlen);
        ~Sockaddr();

        Sockaddr(const Sockaddr &other) = default;
        Sockaddr(Sockaddr &&other) = default;
        Sockaddr &operator=(const Sockaddr &other) = default;
        Sockaddr &operator=(Sockaddr &&other) = default;

    public:
        sockaddr *set_addr() noexcept;
        socklen_t *set_addrlen() noexcept;

        void set_addr(const sockaddr *addr, socklen_t addrlen);
        void set_addr(const Sockaddr &addr);

        const sockaddr *get_addr() const noexcept;
        socklen_t get_addrlen() const noexcept;

        std::string get_address() const;
        wuk::u16 get_port() const;
    };

// Socket BEGIN
    class LIBWUK_API Socket {
    private:
        wSocket  m_fd        = INV_SOCK;

        wuk::i32 m_family    = NETERROR;
        wuk::i32 m_sock_type = NETERROR;
        wuk::i32 m_proto     = NETERROR;

        Sockaddr m_raddr; // 当前套接字的对端地址信息
        Sockaddr m_laddr; // 当前套接字本地绑定的地址信息

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

        // True代表当前套接字有效，否则无效；内部直接返回is_valid方法。
        explicit operator bool() const noexcept;

    public:
        template <typename T>
        void setsockopt(wuk::i32 level, wuk::i32 opt_name, T value);
        template <typename T>
        T getsockopt(wuk::i32 level, wuk::i32 opt_name);

        const Sockaddr getsockname();

        void set_blocking(bool blocked);
        bool get_blocking() const noexcept;

        void set_timeout(wuk::f64 t);
        wuk::f64 get_timeout() const noexcept;

    public:
        void connect(const std::string &addr, wuk::u16 port);
        void bind(const std::string &addr, wuk::u16 port);
        void listen(wuk::i32 backlog) const;
        Socket accept();

        wuk::ilong send(const wuk::Buffer &buffer, wuk::i32 flag = 0);
        void sendall(const wuk::Buffer &buffer, wuk::i32 flag = 0);
        wuk::ilong sendto(const wuk::Buffer &buffer, const Sockaddr &addr, wuk::i32 flag = 0);
        wuk::Buffer recv(wuk::i32 length, wuk::i32 flag = 0);
        wuk::Buffer recvall(wuk::i32 length, wuk::i32 flag = 0);
        wuk::Buffer recvfrom(wuk::i32 length, Sockaddr &addr, wuk::i32 flag = 0);

        void shutdown(wuk::i32 how) const;
        void close();

    public:
        void set_remote(const Sockaddr &addr);
        void set_local(const Sockaddr &addr);

    public:
        const Sockaddr &get_remote() const noexcept;
        const Sockaddr &get_local() const noexcept;

        wSocket fd() const noexcept;

        bool is_valid() const noexcept;
        void mark_invalid() noexcept;
    };
}

#include <template/WukSocket.hh>

#endif
