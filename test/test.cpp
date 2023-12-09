#include <network/wmkc_net.hpp>
#include <network/wmkc_ssl.hpp>

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

void net_test()
{
#   ifdef WMKC_PLATFORM_WINOS
    WSADATA ws;
    WSAStartup(MAKEWORD(2,2), &ws);
#   endif

    std::string target_host = "www.pixiv.net";
    wmkc_u16 target_port = 443;
    std::string sendbuf = (
        "GET / HTTP/1.1\r\n"
        "Host: " + target_host + "\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0\r\n\r\n");
    wmkcChar recvbuf[4096] = {0};

    wmkcSSL_Context *ssl = new wmkcSSL_Context(TLS_method());
    wmkcSSL_Socket ssl_socket = ssl->wrap_socket(Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), target_host);

    ssl_socket.fd.connect(target_host, target_port);
    SSL_connect(ssl_socket.ssl);

    SSL_write(ssl_socket.ssl, sendbuf.c_str(), sendbuf.size());
    SSL_read(ssl_socket.ssl, recvbuf, sizeof(recvbuf));

    cout << recvbuf << endl;

    ssl_socket.fd.close();

    delete ssl;
#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
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

int main()
{
    



    return 0;
}
