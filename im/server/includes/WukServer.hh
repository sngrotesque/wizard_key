// 服务端入口（套接字监听，客户端连接管理等）
#pragma once

#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <net/WukSocket.hh>

#include <server/includes/WukSession.hh>
#include <server/includes/WukAccountService.hh>
#include <server/includes/WukMessageService.hh>
#include <server/includes/WukDispatcher.hh>

namespace wuk::im::server {
    class LIBWUK_API Server {
    private:
        wuk::net::Socket listen_fd;
        const std::string listen_addr;
        wuk::u16 listen_port;

    public:
        Server() = default;
        Server(const std::string &listen_addr, wuk::u16 listen_port);

    public:
        void init(double timeout = 300);
        void handler();
    };
}

#endif
