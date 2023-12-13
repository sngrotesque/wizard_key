#include <network/wmkc_net.hpp>
#include <network/wmkc_ssl.hpp>
#include <network/wmkc_dns.hpp>

#include <crypto/snc.hpp>

#include <wmkc_binascii.hpp>
#include <wmkc_base64.hpp>

#include <wmkc_random.hpp>
#include <wmkc_struct.hpp>

#include <wmkc_hash.hpp>
#include <wmkc_misc.hpp>
#include <wmkc_time.hpp>

#include <map>

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

    const char *sendbuf = ("\x04\xd2\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00\x08\x70\x61\x73\x73"
        "\x70\x6f\x72\x74\x08\x62\x69\x6c\x69\x62\x69\x6c\x69\x03\x63\x6f\x6d\x00\x00\x01\x00\x01");
    const int sendbuf_size = 39;

    wmkcNet::Socket fd(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    fd.sendto(std::string(sendbuf, sendbuf_size), IPEndPoint("dns.alidns.com", 53));
    std::string res = fd.recvfrom(1024);
    fd.close();

    wmkcMisc::PRINT((wmkcByte *)res.c_str(), res.size(), 32, 1, 0);

#   ifdef WMKC_PLATFORM_WINOS
    WSACleanup();
#   endif
}

void vector_test(vector<int> args)
{
    for(const auto &i : args) {
        cout << "args[i]: " << i << endl;
    }
}

void map_test(map<string, string> args)
{
    for(const auto &i : args) {
        cout << i.first << ":" << i.second << endl;
    }
}

int main()
{
    // vector_test({1,2,3,4,5,6,7,8,9});
    map_test({{"name","sngrotesque"}, {"cookie","aisjd1358789"}});

    return 0;
}
