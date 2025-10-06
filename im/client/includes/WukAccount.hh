#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <common/includes/WukLoginRequest.pb.hh>
#include <net/WukSocket.hh>
#include <utils/number.hh>

namespace wuk::im::client {
    class LIBWUK_API Account {
    private:
        wuk::im::LoginRequest info;
        wuk::net::Socket fd;

    public:
        Account() = default;
        ~Account() = default;

        Account(const Account &other) = default;
        Account(Account &&other) = default;
        Account &operator=(const Account &other) = default;
        Account &operator=(Account &&other) = default;

    public:
        Account(const std::string &server_addr, wuk::u16 server_port);

    public:
        void set_username(const std::string &name);
        void set_password(const std::string &password);

        void send_packet();
    };
}

#endif
