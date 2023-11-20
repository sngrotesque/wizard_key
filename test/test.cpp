#include <wmkc_time.hpp>
#include <wmkc_random.hpp>
#include <wmkc_misc.h>

using namespace std;

int main(int argc, char **argv)
{
    wmkcRandom random;
    string random_data = random.urandom(32);

    wmkcByte *p = (wmkcByte *)random_data.c_str();

    wmkcMisc_PRINT(p, 32, 32, 1, 0);

    return 0;
}
