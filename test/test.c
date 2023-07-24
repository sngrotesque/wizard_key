// #include <wmkc_hexdump.c>
// #include <wmkc_winapi.c>
// #include <wmkc_dict.c>
// #include <wmkc_sort.c>
// #include <crypto/snc_412.c>

// #include <network/wmkc_http.c>
// #include <network/wmkc_ddos.c>
// #include <network/wmkc_chunk.c>
#include <network/wmkc_net.c>
// #include <network/wmkc_tp.c>
// #include <crypto/wmkc_crypto.c>
#include <crypto/snc.c>
// #include <crypto/rsa.c>
// #include <crypto/pkc.c>
// #include <wmkc_binascii.c>
#include <wmkc_memory.c>
#include <wmkc_common.c>
#include <wmkc_base64.c>
// #include <wmkc_object.c>
#include <wmkc_random.c>
// #include <wmkc_stream.c>
#include <wmkc_struct.c>
// #include <wmkc_thread.c>
#include <wmkc_basic.c>
#include <wmkc_coder.c>
// #include <wmkc_chat.c>
#include <wmkc_file.c>
// #include <wmkc_hash.c>
#include <wmkc_time.c>
#include <wmkc_math.c>
#include <wmkc_misc.c>
#include <wmkc_img.c>
#include <wmkc_pad.c>

#if 1
static wmkcByte testKey[96] = {
    0xbc, 0x65, 0x5c, 0x7b, 0x34, 0x78, 0x74, 0x6e, 0x49, 0x62, 0x6d, 0x3b, 0x7a, 0x77, 0x5d, 0x53,
    0x71, 0x52, 0x4a, 0x30, 0x3e, 0x2e, 0x63, 0x24, 0x67, 0x3b, 0x6e, 0x61, 0x6b, 0x3a, 0x21, 0x73,
    0x30, 0x66, 0x6c, 0x23, 0x74, 0x76, 0x45, 0x5d, 0x22, 0x3d, 0x50, 0x79, 0x60, 0x6c, 0x5d, 0x75,
    0x70, 0x6a, 0x68, 0x66, 0x52, 0x6e, 0x55, 0x4c, 0x7c, 0x51, 0x4e, 0x28, 0x31, 0x72, 0x7a, 0x40,
    0x49, 0x4e, 0x3b, 0x36, 0x6b, 0x69, 0x54, 0x37, 0x35, 0x70, 0x30, 0x3e, 0x3d, 0x60, 0x73, 0x42,
    0x5f, 0x61, 0x6f, 0x79, 0x7a, 0x36, 0x5e, 0x6d, 0x35, 0x40, 0x5f, 0x6d, 0x63, 0x57, 0x49, 0x31};
static wmkcByte testIv[32] = {
    0x91, 0x75, 0x76, 0x36, 0x23, 0x59, 0x72, 0x29, 0x6e, 0x40, 0x5f, 0x65, 0x2d, 0x55, 0x75, 0x68,
    0x3d, 0x41, 0x78, 0x36, 0x4c, 0x50, 0x7d, 0x73, 0x61, 0x4e, 0x33, 0x6f, 0x23, 0x47, 0x4c, 0x36};
#endif

wmkcVoid snc_test()
{
    wmkcChar text[2048] = {
        "POST /qrcode/getLoginInfo HTTP/1.1\r\n"
        "Host: passport.bilibili.com\r\n"
        "Accept: */*; text/html\r\n"
        "Connection: keep-alive\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 41\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0\r\n\r\n"
        "oauthKey=8df018f88b37e70b95b12731a11c0472"};
    wmkcByte *buf = (wmkcByte *)text;
    wmkcSize size = strlen(text);
    wmkc_u32 segment_size = 32;

    printf("Plaintext: ");
    wmkcMisc_PRINT_RAW(buf, size, 1);
    // printf("Segment size: %u\n", segment_size);

    wmkcSNC_obj *snc = wmkcNull;
    wmkcSNC_new(&snc, SNC_256);
    wmkcSNC_init(snc, testKey, testIv);
    wmkcSNC_ctr_xcrypt(snc, buf, size);
    wmkcSNC_free(&snc);

    printf("Ciphertext: ");
    wmkcMisc_PRINT_RAW(buf, size, 1);
}

wmkcSize conversion(wmkcCSTR content, wmkcByte base)
{
    wmkcChar *copy = wmkcNull;
    wmkcSize size = strlen(content);
    wmkcSize res = 0;
    wmkcSize i;

    if(!wmkcMem_new(wmkcChar *, copy, size)) {
        return 0;
    }
    memcpy(copy, content, size);

    for(i = 0; i < size; ++i) {
        if(copy[i] >= 0x41 && copy[i] <= 0x5a) {
            copy[i] += 0x20;
        }
    }

    for(i = 0; i < size; ++i) {
        if(content[i] >= '0' && content[i] <= '9') {
            res += (content[i] - '0') * wmkcMath_pow(base, size-i-1);
        } else if(content[i] >= 'a' && content[i] <= 'z') {
            res += (content[i] - 'a' + 10) * wmkcMath_pow(base, size-i-1);
        } else {
            printf("Invalid contentber %c", content[i]);
            return 0;
        }
    }

    wmkcMem_free(copy);
    return res;
}

wmkcVoid test()
{
    snc_test();
}

wmkc_s32 main(wmkc_u32 argc, wmkcChar **argv)
{
    void (*func)() = test;
    func();
    return 0;
}
