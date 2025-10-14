#include <server/includes/WukServer.hh>

namespace wuk::im::server {
    Server::Server(const std::string &listen_addr, wuk::u16 listen_port)
        : listen_addr(listen_addr), listen_port(listen_port)
    {

    }

    void Server::init(double timeout)
    {
        wuk::net::Socket server_fd(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        server_fd.setsockopt<wuk::i32>(SOL_SOCKET, SO_REUSEADDR, true);
        server_fd.set_timeout(timeout);
        server_fd.bind(this->listen_addr, this->listen_port);
        server_fd.listen(~0U);

        this->listen_fd = std::move(server_fd);
    }

    void Server::handler()
    {
        
    }
}
