#include <snConf.h>
#include <snMisc.h>
#include <snRand.h>
#include <snBase64.h>
#include <snPadding.h>

#include <snKey.h>

// v2.2.0
#include <crypto/snc/snc.h>
#include <crypto/snc/snc.c>

static snChar inputBuffer;
static snByte default_iv[SNC_BLOCKLEN] = {
    110,  40, 214, 251, 211, 165, 216, 235,
    97,  158, 183, 186,  91, 251, 126, 227,
    92,  152, 233,   8, 170,   7, 183,  52,
    236, 128,  84, 137, 251,  14,  87,  70
};

sn_u32 main(sn_u32 argc, snChar **argv)
{

    return 0;
}

