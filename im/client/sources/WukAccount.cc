#include <client/includes/WukAccount.hh>

namespace wuk::im::client {
    Account::Account(const std::string &server_addr, wuk::u16 server_port)
        : fd(AF_INET, SOCK_STREAM, IPPROTO_TCP)
    {
        this->fd.set_timeout(5);
        this->fd.connect_ex(server_addr, server_port);
    }

    void Account::set_username(const std::string &name)
    {
        if (name.empty()) {
            throw wuk::Exception(wuk::Error::ERR,
                "wuk::im::client::Account::set_username",
                "name is empty.");
        }

        this->info.set_m_name(name);
    }

    void Account::set_password(const std::string &password)
    {
        if (password.empty()) {
            throw wuk::Exception(wuk::Error::ERR,
                "wuk::im::client::Account::set_password",
                "password is empty.");
        }

        this->info.set_m_password(password);
    }

    void Account::send_packet()
    {
        std::string packet_length(4, '\0');
        std::string packet = this->info.SerializeAsString();

        wuk::utils::pack_bytes(
            reinterpret_cast<wuk::byte *>(packet_length.data()),
            packet_length.capacity(),
            static_cast<wuk::u32>(packet.size())
        );

        // 先发送4字节的包的长度给服务端
        this->fd.send_ex(packet_length);
        // 再发送实际内容
        this->fd.send_ex(packet);
    }
}
