#include <net/WukSocket.hh>
#include <cmath>

enum class IOType {
    SEND,
    CONNECT,
    ACCEPT,
    RECV
};

timeval create_timeval(double timeout)
{
    timeval tv {0};

    double int_part{0};
    double float_part{0};

    float_part = modf(timeout, &int_part);

    tv.tv_sec = static_cast<time_t>(int_part);
    tv.tv_usec = static_cast<time_t>(float_part * 1e6);

    printf("tv_sec:  %ld\n", tv.tv_sec);
    printf("tv_usec: %ld\n", tv.tv_usec);

    return tv;
}

template<typename T, typename call_function, typename... Args>
T sock_call_ex(  wuk::net::WukSocket &fd,
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
            wuk::net::SocketError err = wuk::net::from_code(wuk::net::SystemError::code());
            if (err == wuk::net::SocketError::WOULDBLOCK) {
                // 设置select监听
                fd_set fds;
                FD_ZERO(&fds);
                FD_SET(fd.get_fd(), &fds);

                timeval tv = create_timeval(fd.get_timeout());
                wI32 select_ret = 0;
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
                        wI32 err_code = wuk::net::SystemError::code();
                        throw wuk::Exception(err_code, func_name,
                            wuk::net::SystemError::message(err_code));
                    }
                } else if (select_ret == 0) {
                    throw wuk::Exception(wuk::Error::ERR, func_name,
                        "socket timeout.");
                } else if (select_ret == NETERROR) {
                    wI32 err_code = wuk::net::SystemError::code();
                    throw wuk::Exception(err_code, func_name,
                        wuk::net::SystemError::message(err_code));
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

void wuk::net::WukSocket::connect_ex(const std::string& addr, const wU16& port)
{
    WukAddrinfo info(this->m_family, this->m_sock_type, this->m_proto);
    info.resolve(addr, port);

    auto connect_timeout = [&](const sockaddr *addr, socklen_t addrlen)
    {
        return ::connect(this->fd, addr, addrlen);
    };

    wI32 err = sock_call_ex<wI32>(*this, "wuk::net::WukSocket::connect_ex",
            connect_timeout, IOType::CONNECT, info.get_addr(), info.get_addrlen());

    if (err == NETERROR) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::connect_ex",
            wuk::net::SystemError::message(err_code));
    }

    this->m_raddr.set_addr(info.get_addr(), info.get_addrlen());
    this->m_laddr.set_addr(this->getsockname());
}

wuk::net::WukSocket wuk::net::WukSocket::accept_ex()
{
    WukSockaddr client;

    auto accept_timeout = [&](sockaddr *addr, socklen_t *addrlen)
    {
        return ::accept(this->fd, addr, addrlen);
    };

    wSocket client_sock = sock_call_ex<wSocket>(*this, "wuk::net::WukSocket::accept_ex",
            accept_timeout, IOType::ACCEPT, client.set_addr(), client.set_addrlen());

    if (client_sock == static_cast<wSocket>(NETERROR)) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::accept_ex",
            wuk::net::SystemError::message(err_code));
    }

    WukSocket new_sock(this->m_family, this->m_sock_type, this->m_proto, client_sock);
    new_sock.set_raddr(client);

    return new_sock;
}

wSSize wuk::net::WukSocket::send_ex(const std::string &buffer, wI32 flag)
{
    auto send_timeout = [&](const std::string &buffer)
    {
        return ::send(this->fd, buffer.c_str(), buffer.length(), flag);
    };

    wSSize sent = sock_call_ex<wSSize>(*this, "wuk::net::WukSocket::send_ex",
            send_timeout, IOType::SEND, buffer);

    if (sent == NETERROR) {
        wI32 err_code = wuk::net::SystemError::code();
        throw wuk::Exception(err_code, "wuk::net::WukSocket::accept_ex",
            wuk::net::SystemError::message(err_code));
    }

    return sent;
}

// std::string wuk::net::WukSocket::recv_ex(const socklen_t &length, wI32 flag)
// {

// }
