// #include <cpp/includes/network/wmkc_net.hpp>
#include <cpp/includes/network/wmkc_ssl.hpp>

#include <cpp/includes/crypto/snc.hpp>
#include <cpp/includes/wmkc_base64.hpp>

#include <c/includes/wmkc_misc.h>

void net_test(void (*func)(std::string, const uint16_t, const std::string), std::string addr, const uint16_t port, std::string headers)
{
#ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#endif
    func(addr, port, headers);
#ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#endif
}

void https_test(std::string server_hostname, const uint16_t port, const std::string headers)
{
    using namespace std;
    using namespace wmkcNet;

    wmkcSSL_Context ssl = wmkcSSL_Context();
    wmkcSSL_Socket sockfd = ssl.wrap_socket(new Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), server_hostname);

    sockfd.connect(server_hostname, port);
    sockfd.send(headers);

    cout << sockfd.recv(4096) << endl;

    sockfd._fd->close();
}

int main(int argc, char **argv)
{
    net_test(https_test, "www.gov.cn", 443, "GET / HTTP/1.1\r\nHost: www.gov.cn\r\nUser-Agent: android\r\n\r\n");

    return 0;
}
