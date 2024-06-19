#include <struct.hpp>
#include <misc.hpp>

#include <crypto/fea.hpp>

#include <iostream>
#include <cmath>

#include <zlib.h>

// Source path: Begin
#include <struct.cpp>

#include <crypto/fea.cpp>
#include <crypto/fea_ctr.cpp>
#include <crypto/fea_ecb.cpp>
#include <crypto/fea_cbc.cpp>
#include <crypto/fea_cfb.cpp>
// Source path: End

using namespace std;

string struct_pack_test(wU32 seq, string content)
{
    wmkc::Struct Struct;
    std::string results;

    try {
        results.append(Struct.pack("!II", seq, content.size()));
        results.append(content);
        results.append(Struct.pack("!I", crc32(0, (wByte *)results.c_str(), results.size())));
    } catch(std::exception &e) {
        cout << e.what() << endl;
    }

    return results;
}

void fea_test()
{
    std::string res = struct_pack_test(0x00000001, "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nAccept: */*\r\nConnection: keep-alive\r\nUser-Agent: Android\r\n\r\n");
    wmkc::crypto::FEA fea((wByte *)"abcdef0123456789abcdef0123456789", (wByte *)"abcdef0123456789");
    wSize length = res.size();
    wByte *buffer = new (std::nothrow) wByte[length];

    memcpy(buffer, res.c_str(), length);

    fea.encrypt(buffer, length, wmkc::crypto::xcryptMode::CTR);
    wmkc::misc::PRINT_HEX(buffer, length, 32, 1, 0);

    delete[] buffer;
}

int main(int argc, char **argv)
{
    fea_test();
    // struct_pack_test(0xff234567, "Hello, World.\n");

    return 0;
}
