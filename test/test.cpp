#include <network/wmkc_net.hpp>
#include <network/wmkc_ssl.hpp>
#include <network/wmkc_dns.hpp>

#include <crypto/snc.hpp>

#include <wmkc_binascii.hpp>
#include <wmkc_base64.hpp>
#include <wmkc_random.hpp>
#include <wmkc_hash.hpp>
#include <wmkc_misc.hpp>
#include <wmkc_time.hpp>

using namespace std;
using namespace wmkcNet;
using namespace wmkcCrypto;

void timer(void (*func)())
{
    wmkcTime time;

    double start = time.time();
    func();
    double stop = time.time();

    cout << "time used: " << (stop-start) << endl;
}

void snc_test()
{
    wmkcSNC *snc = wmkcNull;
    wmkcRandom random;
    wmkcTime time;
    wmkcByte key[96];
    wmkcByte iv[32];

    wmkc_u32 length = 256 * (1024 * 1024);
    wmkcByte *content = new wmkcByte[length];

    double start_time, stop_time;

    random.urandom(key, sizeof(key));
    random.urandom(iv, sizeof(iv));

    snc = new wmkcSNC(key, iv, SNC_keyMode::SNC_768);

    cout << "Start timing." << endl;
    start_time = time.time();
    snc->encrypt(content, length);
    stop_time = time.time();
    cout << "End timing." << endl;

    printf("time used: %.4lf\n", stop_time-start_time);

    delete content;
    delete snc;
}

void net_test()
{
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    char send_buffer[40] = {"\x04\xd2\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00\x08\x70\x61\x73\x73\x70\x6f\x72\x74\x08\x62\x69\x6c\x69\x62\x69\x6c\x69\x03\x63\x6f\x6d\x00\x00\x01\x00\x01"};
    int send_buffer_size = 39;
    char recv_buffer[512];
    int err, recv_length;
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ADDRINFO *result = getAddrInfo(AF_INET, SOCK_DGRAM, IPPROTO_UDP, "223.5.5.5", "53");
    err = sendto(sockfd, send_buffer, send_buffer_size, 0, result->ai_addr, result->ai_addrlen);
    if(err == SOCKET_ERROR) {
        wmkcNet::Socket_exception("net_test");
    }
    recv_length = recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, result->ai_addr, (int *)&result->ai_addrlen);
    if(recv_length == SOCKET_ERROR) {
        wmkcNet::Socket_exception("net_test");
    }
    wmkcMisc::PRINT((wmkcByte *)recv_buffer, recv_length, 16, 1, 0);
    closesocket(sockfd);

#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
}

int main()
{
    net_test();
    return 0;
}
