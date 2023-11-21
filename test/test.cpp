#include <network/wmkc_net.hpp>

#include <wmkc_base64.hpp>
#include <wmkc_random.hpp>
#include <wmkc_misc.hpp>
#include <wmkc_time.hpp>

using namespace std;

int main(int argc, char **argv)
{
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#endif

    wmkcTime time;
    wmkcBase64 base64;
    wmkcRandom random;
    wmkcNet::Socket sockfd(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    printf("Current time: %lf\n", time.time());
    cout << "random number[0x00-0xff]: " << random.randint(0x00, 0xff) << endl;
    sockfd.connect("www.baidu.com", 80);
    sockfd.sendall("GET / HTTP/1.1\r\nHost: www.baidu.com\r\nAccept: */*\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:101.0) Gecko/20100101 Firefox/101.0\r\n\r\n");
    cout << base64.encode(sockfd.recv(4096)) << endl;
    sockfd.close();

#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#endif
    return 0;
}
