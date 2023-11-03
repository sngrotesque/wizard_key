#include <crypto/snc_412.c>

#include <wmkc_memory.c>
#include <wmkc_common.c>
#include <wmkc_random.c>
#include <wmkc_base64.c>
#include <wmkc_basic.c>
#include <wmkc_misc.c>
#include <wmkc_time.c>
#include <wmkc_pad.c>

wmkcByte *b64decode(wmkcCSTR src)
{
    wmkcByte *dst = wmkcNull;
    wmkcBase64_decode(&dst, (wmkcByte *)src, strlen(src));
    return dst;
}

int main()
{
    SNC_ctx *snc = wmkcNull;
    wmkcByte *key = b64decode("aPdbVFG94xWSCJxrZyceusYbmNzsfOddVuiCh0aCRLF0kb7H/6FjrP7OaLpiT4dFqOgT0TMyUbwnLHbgPt1iR0B/GqQFElpEw74WuSnSJYCkTj9LDIlRxSZiRNl4aTQS");
    wmkcByte *iv = b64decode("ksZbta0YIpW4VaMBMdKr31rXJPvnTirqBBAZYSlAg4I=");
    wmkcByte *buf = b64decode("ivWdCTpsjFnyvDziYjVs9ECmrlL236h9rCcWrskkEys=");

    SNC_malloc_init(&snc, SNC_768);
    SNC_init_ctx(snc, key, iv);
    SNC_CBC_Decrypt(snc, buf, 32);
    SNC_release(&snc);

    wmkcMisc_PRINT_RAW(buf, 32, 1);
    return 0;
}
