#include <network/WukDns.hh>

wuk::net::IPEndPoint wuk::net::Dns::get_host_address(std::string server_name)
{

}

wuk::net::IPEndPoint wuk::net::Dns::get_addr_info(std::string server_name, std::string service_name)
{
    ADDRINFO hints{};
    ADDRINFO *result{nullptr};

    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    wI32 err = getaddrinfo(server_name.c_str(), service_name.c_str(), &hints, &result);
    if (err) {
        throw wuk::net::Exception(err, "wuk::net::Dns::get_addr_info");
    }

    
}

wuk::net::IPEndPoint wuk::net::Dns::get_name_info(ADDRINFO content)
{

}


