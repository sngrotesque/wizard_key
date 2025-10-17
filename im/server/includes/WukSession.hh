// 会话/连接抽象
#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <net/WukSocket.hh>
#include <server/includes/WukDispatcher.hh>

namespace wuk::im::server {
    class LIBWUK_API Session {
    private:
        wuk::net::Socket m_client;

    public:
        Session() = default;

        Session(const Session &other) = delete;
        Session(Session &&other) = default;
        Session &operator=(const Session &other) = delete;
        Session &operator=(Session &&other) = default;

        Session(const wuk::net::Socket &other) = delete;
        Session(wuk::net::Socket &&other);
        Session &operator=(const wuk::net::Socket &other) = delete;
        Session &operator=(wuk::net::Socket &&other);

    public:
        bool valid() const noexcept;
        wuk::net::wSocket fd() const noexcept;

        std::string remote_addr() const noexcept;
        wuk::u16    remote_port() const noexcept;

    public:
        void on_receive();
        void on_send(const wuk::Buffer &buffer);

        void disconnect();
    };
}

#endif
