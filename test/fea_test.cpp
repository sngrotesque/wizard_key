#include <iostream>
#include <fstream>

#include <crypto/fea.hpp>
#include <base64.hpp>
#include <misc.hpp>
#include <padding.hpp>

#include <crypto/fea_cbc.cpp>
#include <crypto/fea_ecb.cpp>
#include <crypto/fea_ctr.cpp>
#include <crypto/fea_cfb.cpp>
#include <crypto/fea.cpp>
#include <base64.cpp>
#include <random.cpp>
#include <time.cpp>
#include <padding.cpp>

using namespace std;

const wByte *key = (wByte *)"[CeN&qmUNso53Q4|E$H<RW7;Y%?f.*~J";
const wByte *iv  = (wByte *)"_wnMsGmwfV=Q13]l";

wByte *xcrypt(wByte *buffer, wSize length, wmkc::crypto::xcryptMode xcrypt_mode, wBool encryption)
{
    wmkc::crypto::fea fea(key, iv);
    wByte *result = nullptr;

    if(encryption) {
        result = wmkc::pkcs7_pad(buffer, length, WMKC_FEA_BL);
        fea.encrypt(result, length, xcrypt_mode);
    } else {
        fea.decrypt(buffer, length, xcrypt_mode);
        result = wmkc::pkcs7_unpad(buffer, length);
    }

    return result;
}

wVoid file_xcrypt(string srcPath, string dstPath, wmkc::crypto::xcryptMode xcrypt_mode, wBool encryption)
{
    fstream src_fp(srcPath.c_str(), ios::in  | ios::binary | ios::ate);
    fstream dst_fp(dstPath.c_str(), ios::out | ios::binary);

    if(!src_fp.is_open() || !dst_fp.is_open()) {
        cout << "Failed to file open." << endl;
        return;
    }

    wByte *buffer_in  = nullptr;
    wByte *buffer_out = nullptr;
    wSize length;

    length    = src_fp.tellg();
    buffer_in = new wByte[length];

    src_fp.seekg(ios::beg);
    src_fp.read((wChar *)buffer_in, length);
    buffer_out = xcrypt(buffer_in, length, xcrypt_mode, encryption);
    dst_fp.write((wChar *)buffer_out, length);

    delete[] buffer_out;
    delete[] buffer_in;
    src_fp.close();
    dst_fp.close();
}

wVoid fea_speed_test(wU32 length = (1<<24))
{
    wmkc::crypto::fea fea(key,iv);
    wmkc::Time timer;
    double start, stop;

    wByte *buffer = new wByte[length];

    start = timer.time();
    fea.encrypt(buffer, length, wmkc::crypto::xcryptMode::CBC);
    stop = timer.time();

    printf("time used: %.4lf\n", stop-start);

    delete[] buffer;
}

wS32 main(wS32 argc, wChar **argv)
{
    fea_speed_test();

    return 0;
}
