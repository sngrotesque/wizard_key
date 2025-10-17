#include <server/includes/WukSession.hh>

namespace wuk::im::server {
    Session::Session(wuk::net::Socket &&other)
        : m_client(std::move(other))
    {

    }

    Session &Session::operator=(wuk::net::Socket &&other)
    {
        if (this->m_client == other) {
            return *this;
        }
        this->m_client = std::move(other);
        return *this;
    }

    bool Session::valid() const noexcept
    {
        return this->m_client.is_valid();
    }

    wuk::net::wSocket Session::fd() const noexcept
    {
        return this->m_client.get_fd();
    }

    std::string Session::remote_addr() const noexcept
    {
        return this->m_client.get_raddr().get_address();
    }

    wuk::u16 Session::remote_port() const noexcept
    {
        return this->m_client.get_raddr().get_port();
    }

    




    void Session::disconnect()
    {
        this->m_client.shutdown(2);
        this->m_client.close();
    }
}
