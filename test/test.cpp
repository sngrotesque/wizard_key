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
    wmkcStruct Struct;

    Struct.pack(">IIIII", {1,2,3,4,5});


    return 0;
}
