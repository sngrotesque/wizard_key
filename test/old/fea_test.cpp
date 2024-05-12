#include <crypto/crypto.hpp>
#include <crypto/fea.hpp>

#include <wmkc_time.hpp>
#include <wmkc_misc.hpp>

using namespace std;
using namespace wmkcCrypto;

void fea_sbox_test()
{
    wmkcFEA fea;
    wmkcTime time;

    wU32 length = 268435456;
    wByte *array = new wByte[length];
    double start_time, stop_time;

    cout << "data length: " << ((double)length / (1024 * 1024)) << "mb." << endl;

    cout << "start timer..." << endl;
    start_time = time.time();
    for(wU32 i = 0; i < length; ++i) {
        *(array + i) = fea.subBytes(*(array + i));
    }
    stop_time = time.time();
    cout << "stop timer." << endl;

    printf("time used: %.4lf\n", (stop_time-start_time));

    delete array;
}

int main()
{
    fea_sbox_test();
    return 0;
}
