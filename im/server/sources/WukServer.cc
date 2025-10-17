#include <server/includes/WukServer.hh>
#include <algorithm>
#include <cmath>

constexpr wuk::i32 MAX_CLIENTS = FD_SETSIZE;

static timeval create_timeval(wuk::f64 t)
{
    timeval tv {};
    wuk::f64 int_part {};
    wuk::f64 float_part = std::modf(t, &int_part);

    tv.tv_sec  = static_cast<time_t>(int_part);
    tv.tv_usec = static_cast<time_t>(float_part * 1e6);

    return tv;
}

namespace wuk::im::server {
    Server::Server(wuk::u16 port, wuk::i32 backlog)
        : m_addr("0.0.0.0")
        , m_port(port)
        , m_backlog(backlog)
    {

    }

    void Server::init()
    {
        wuk::net::Socket server_fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        server_fd.setsockopt<wuk::i32>(SOL_SOCKET, SO_REUSEADDR, true);
        server_fd.setsockopt<wuk::i32>(IPPROTO_TCP, TCP_NODELAY, true);
        server_fd.bind(this->m_addr, this->m_port);
        server_fd.listen(this->m_backlog);
        this->m_fd = std::move(server_fd);

        this->m_clients.reserve(MAX_CLIENTS);
    }

    void Server::run(wuk::f64 timeout)
    {
        FD_SET read_fds;
        FD_SET write_fds;

        this->m_fd.set_timeout(timeout);

        while (true) {
            FD_ZERO(&read_fds);
            FD_ZERO(&write_fds);

            FD_SET(this->m_fd.get_fd(), &read_fds);

            wuk::net::wSocket max_fds = this->m_fd.get_fd();
            for (const Session &client : this->m_clients) {
                if (!client.valid()) {
                    continue;
                }
                wuk::net::wSocket fd = client.fd();
                FD_SET(fd, &read_fds);
                if (fd > max_fds) {
                    max_fds = fd;
                }
            }

            timeval tv = create_timeval(timeout);
            wuk::i32 ready = select(max_fds + 1, &read_fds, &write_fds, nullptr, &tv);

            if (ready == 0) {
                throw wuk::Exception(wuk::Error::TIMEO, "wuk::im::server::Server::run",
                    "socket timeout.");
            } else if (ready == NETERROR) {
                throw wuk::Exception(wuk::net::err::system::code(),
                    "wuk::im::server::Server::run",
                    wuk::net::err::system::last_message());
            }

            if (FD_ISSET(this->m_fd.get_fd(), &read_fds)) {
                wuk::net::Socket client = this->m_fd.accept_ex();
                client.set_blocking(false);

                std::vector<Session>::iterator \
                it = std::find_if(
                    this->m_clients.begin(),
                    this->m_clients.end(),
                    [](const Session &client)
                    {
                        return !client.valid();
                    }
                );

                if (it != this->m_clients.end()) {
                    *it = std::move(client);
                } else if (this->m_clients.size() < MAX_CLIENTS) {
                    this->m_clients.emplace_back(std::move(client));
                } else {
                    throw wuk::Exception(wuk::Error::ERR, "wuk::im::server::Server::run",
                        "The client queue has reached its maximum size, "
                        "no new connections should be accepted.");
                }

#               ifdef WUK_DEBUG
                fmt::print("客户端连接：{}:{}。\n",
                    (*it).remote_addr(),
                    (*it).remote_port()
                );
#               endif
            }

            // for (Session &client : this->m_clients) {
            //     if (!FD_ISSET(client.fd(), &read_fds)) {
            //         continue;
            //     }
            //     // 也就是在这处理Session收发？
            // }

            std::vector<Session>::iterator \
            pos = std::remove_if(
                this->m_clients.begin(),
                this->m_clients.end(),
                [](const Session &client)
                {
                    return !client.valid();
                }
            );
            this->m_clients.erase(pos, this->m_clients.end());
        }
    }
}
