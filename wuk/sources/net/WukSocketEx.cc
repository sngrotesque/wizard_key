#include <net/WukSocket.hh>
#include <cmath>

enum class IOType {
    SEND,
    CONNECT,
    ACCEPT,
    RECV
};

timeval create_timeval(wuk::f64 timeout)
{
    timeval tv {};

    wuk::f64 int_part{0};
    wuk::f64 float_part{0};

    float_part = modf(timeout, &int_part);

    tv.tv_sec = static_cast<time_t>(int_part);
    tv.tv_usec = static_cast<time_t>(float_part * 1e6);

    printf("tv_sec:  %ld\n", tv.tv_sec);
    printf("tv_usec: %ld\n", tv.tv_usec);

    return tv;
}

template<typename T, typename call_function, typename... Args>
T sock_call_ex( wuk::net::Socket &fd,
                const std::string &func_name,
                call_function func,
                IOType io_type,
                Args&&... args)
{
    if (fd.get_timeout() == 0) {
#       ifdef ALLOW_UNSET_TIMEOUT
        return func(std::forward<Args>(args)...);
#       else
        throw wuk::Exception(wuk::Error::ERR, func_name,
            "This function is not allowed to be called with a timeout of 0.");
#       endif
    }

    bool was_blocking = fd.get_blocking(); // 保存原始阻塞状态

    try {
        // 设置为非阻塞模式
        fd.set_blocking(false);

        // 执行非阻塞操作
        T result = func(std::forward<Args>(args)...);
        T _res_err = NETERROR;

        if constexpr (std::is_same_v<T, wuk::net::wSocket>) {
            _res_err = static_cast<wuk::net::wSocket>(NETERROR);
        }

        if (result == _res_err) {
            wuk::net::err::SocketType err = wuk::net::err::from_code(wuk::net::err::system::code());
            if (err == wuk::net::err::SocketType::WOULDBLOCK) {
                // 设置select监听
                fd_set fds;
                FD_ZERO(&fds);
                FD_SET(fd.get_fd(), &fds);

                timeval tv = create_timeval(fd.get_timeout());
                wuk::i32 select_ret = 0;
                switch (io_type) {
                    case IOType::SEND:
                    case IOType::CONNECT:
                        select_ret = select(fd.get_fd() + 1, nullptr, &fds, nullptr, &tv);
                        break;
                    case IOType::RECV:
                    case IOType::ACCEPT:
                        select_ret = select(fd.get_fd() + 1, &fds, nullptr, nullptr, &tv);
                        break;
                }

                if (select_ret > 0) {
                    // 除了connect操作以外都需要重新执行以获得一个返回值
                    if (io_type == IOType::CONNECT) {
                        fd.set_blocking(was_blocking);
                        return 0;
                    }
                    result = func(std::forward<Args>(args)...);
                    if (result == _res_err) {
                        wuk::i32 err_code = wuk::net::err::system::code();
                        throw wuk::Exception(err_code, func_name,
                            wuk::net::err::system::message(err_code));
                    }
                } else if (select_ret == 0) {
                    throw wuk::Exception(wuk::Error::ERR, func_name,
                        "socket timeout.");
                } else if (select_ret == NETERROR) {
                    wuk::i32 err_code = wuk::net::err::system::code();
                    throw wuk::Exception(err_code, func_name,
                        wuk::net::err::system::message(err_code));
                }
            }
        }

        // 恢复原始阻塞状态
        fd.set_blocking(was_blocking);
        return result;
    } catch (...) {
        // 确保无论如何都恢复阻塞状态
        fd.set_blocking(was_blocking);
        throw;
    }
}

namespace wuk::net {
    void Socket::connect_ex(const std::string& addr, const wuk::u16& port)
    {
        Addrinfo info(this->m_family, this->m_sock_type, this->m_proto);
        info.resolve(addr, port);

        auto connect_timeout = [&](const sockaddr *addr, socklen_t addrlen)
        {
            return ::connect(this->fd, addr, addrlen);
        };

        wuk::i32 err = sock_call_ex<wuk::i32>(*this, "wuk::net::Socket::connect_ex",
                connect_timeout, IOType::CONNECT, info.get_addr(), info.get_addrlen());

        if (err == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::connect_ex",
                err::system::message(err_code));
        }

        this->m_raddr.set_addr(info.get_addr(), info.get_addrlen());
        this->m_laddr.set_addr(this->getsockname());
    }

    Socket Socket::accept_ex()
    {
        Sockaddr client;

        auto accept_timeout = [&](sockaddr *addr, socklen_t *addrlen)
        {
            return ::accept(this->fd, addr, addrlen);
        };

        wSocket client_sock = sock_call_ex<wSocket>(*this, "wuk::net::Socket::accept_ex",
                accept_timeout, IOType::ACCEPT, client.set_addr(), client.set_addrlen());

        if (client_sock == static_cast<wSocket>(NETERROR)) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::accept_ex",
                err::system::message(err_code));
        }

        Socket new_sock(this->m_family, this->m_sock_type, this->m_proto, client_sock);
        new_sock.set_raddr(client);

        return new_sock;
    }

    wuk::ilong Socket::send_ex(const std::string &buffer, wuk::i32 flag)
    {
        auto send_timeout = [&](const std::string &buffer)
        {
            return ::send(this->fd, buffer.c_str(), buffer.length(), flag);
        };

        wuk::ilong sent = sock_call_ex<wuk::ilong>(*this, "wuk::net::Socket::send_ex",
                send_timeout, IOType::SEND, buffer);

        if (sent == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::send_ex",
                err::system::message(err_code));
        }

        return sent;
    }

    std::string Socket::recv_ex(const socklen_t &length, wuk::i32 flag)
    {
        auto recv_timeout = [&](std::string &buffer)
        {
            return ::recv(this->fd, buffer.data(), length, flag);
        };

        std::string buffer(length, '\0');
        wuk::ilong received = sock_call_ex<wuk::ilong>(*this, "wuk::net::Socket::recv_ex",
                recv_timeout, IOType::RECV, buffer);

        if (received == NETERROR) {
            wuk::i32 err_code = err::system::code();
            throw wuk::Exception(err_code, "wuk::net::Socket::recv_ex",
                err::system::message(err_code));
        }

        return buffer;
    }
}
