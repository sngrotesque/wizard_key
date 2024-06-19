#include <base64_rfc4648.hpp>
#include <misc.hpp>

#include <base64_rfc4648.cpp>
#include <misc.cpp>

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    wmkc::Base64 base64;

    cout << base64.encode("SN----") << endl;

    return 0;
}
