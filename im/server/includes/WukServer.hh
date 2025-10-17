// 服务端入口（套接字监听，客户端连接管理等）
#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <server/includes/WukSession.hh>
#include <server/includes/WukAccountService.hh>
#include <server/includes/WukMessageService.hh>
#include <server/includes/WukDispatcher.hh>
#include <vector>

namespace wuk::im::server {
    class LIBWUK_API Server {
    private:
        wuk::net::Socket     m_fd;
        std::vector<Session> m_clients;

    private:
        std::string m_addr;
        wuk::u16    m_port    = 0;
        wuk::i32    m_backlog = 0;

    public:
        Server() = default;
        Server(wuk::u16 port, wuk::i32 backlog = 1024);

        Server(const Server &other) = delete;
        Server(Server &&other) = default;
        Server &operator=(const Server &other) = delete;
        Server &operator=(Server &&other) = default;

    public:
        void init();
        void run(wuk::f64 timeout = 300);
    };
}

#endif
