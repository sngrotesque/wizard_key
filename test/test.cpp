#include <network/wmkc_net.hpp>
#include <network/wmkc_ssl.hpp>

#include <crypto/snc.hpp>

#include <wmkc_binascii.hpp>
#include <wmkc_base64.hpp>
#include <wmkc_random.hpp>
#include <wmkc_misc.hpp>
#include <wmkc_time.hpp>

using namespace std;
using namespace wmkcNet;

void timer(void (*func)())
{
    wmkcTime time;

    double start = time.time();
    func();
    double stop = time.time();

    cout << "time used: " << (stop-start) << endl;
}

int main()
{
    
    return 0;
}
