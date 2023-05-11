#include <crypto/snc.h>
#include <snMisc.h>
#include <snTime.h>

#include <crypto/snc.c>
#include <snMisc.c>
#include <snTime.c>

int main(int argc, char **argv)
{
    SNC_ctx *snc = snNull;
    snTime_ctx timer;

    SNC_new(&snc, SNC_768);
    SNC_init(snc,
        (snByte *)"192840348274981749832750142748017583279431985629764274917503284017489279852319058304984791879341",
        (snByte *)"19483042749817987491857324619412");

    static snByte buf[16777216];

    snTime_TimerBegin(&timer);
    SNC_CBC_Encrypt(snc, buf, 16777216);
    snTime_TimerEnd(&timer);
    snTime_TimerPrint("timer: ", &timer);

    return 0;
}
