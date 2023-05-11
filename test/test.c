// #include <network/snNet.h>
// #include <network/snTransfer.h>
// #include <snHash.h>
// #include <errno.h>

// #include <network/snTransfer.c>
// #include <network/snNet.c>
// #include <snBinascii.c>
// #include <snObject.c>
// #include <snHash.c>
// #include <snFile.c>

#include <snPadding.h>
#include <crypto/snc.h>
#include <snMisc.h>
#include <snTime.h>

#include <snPadding.c>
#include <crypto/snc.c>
#include <snMisc.c>
#include <snTime.c>
#include <snRand.c>

SN_PRIVATE_CONST(snSize) SNC_DEFAULT_MODE = SNC_512;

SN_PRIVATE_CONST(snChar *) default_key = {
    "13094329482059831058320438435981"
    "3915u3928hasjfhd9135y39y9qwtye71"
    // "31905uy9dashfi3p1j;su0ed8uy8092y"
};

SN_PRIVATE_CONST(snChar *) default_iv = {
    "134085yhidsugc92etr987123t5813t3"
};

SN_PRIVATE_CONST(snChar *) default_plaintext = {
    "0000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000000000000000000000000000000000000"
};

int main(int argc, char **argv)
{
    SNC_ctx *snc = snNull;
    SNC_new(&snc, SNC_DEFAULT_MODE);
    SNC_init(snc, (snByte *)default_key, (snByte *)default_iv);

    snSize size = strlen(default_plaintext);
    snByte *buf = (snByte *)malloc(2048);
    snZeroObject(buf, 2048);
    memcpy(buf, default_plaintext, size);
    if(size % SNC_BLOCKLEN) {
        snPadding_add(buf, &size, SNC_BLOCKLEN, false);
    }

    SNC_CBC_Encrypt(snc, buf, size);
    SNC_CBC_Decrypt(snc, buf, size);

    snMisc_PRINT(buf, size, 16, 1, 0);

    SNC_release(&snc);
    return 0;
}
