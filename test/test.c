#include <network/wmkc_net.c>
#include <image/wmkc_png.c>
#include <crypto/snc.c>
#include <wmkc_binascii.c>
#include <wmkc_hexdump.c>
#include <wmkc_common.c>
#include <wmkc_winapi.c>
#include <wmkc_object.c>
#include <wmkc_random.c>
#include <wmkc_struct.c>
#include <wmkc_thread.c>
#include <wmkc_base64.c>
#include <wmkc_stream.c>
#include <wmkc_chat.c>
#include <wmkc_dict.c>
#include <wmkc_file.c>
#include <wmkc_misc.c>
#include <wmkc_math.c>
#include <wmkc_time.c>
#include <wmkc_hash.c>
#include <wmkc_sort.c>
#include <wmkc_pad.c>
#include <wmkc_key.c>

#include <zlib/zlib.h>
#include <libpng/png.h>
#include <openssl/sha.h>

#define CIPHER_TEST false

#if defined(WMKC_SNC) && (CIPHER_TEST)
static wmkcByte testKey[96] = {
    0x67, 0x65, 0x5b, 0x7b, 0x33, 0x78, 0x74, 0x6e, 0x49, 0x62, 0x6d, 0x3b, 0x7a, 0x77, 0x5d, 0x53,
    0x71, 0x52, 0x4a, 0x30, 0x3e, 0x2e, 0x63, 0x24, 0x67, 0x3b, 0x6e, 0x61, 0x6b, 0x3a, 0x21, 0x73,
    0x30, 0x66, 0x6c, 0x23, 0x74, 0x76, 0x45, 0x5d, 0x21, 0x3d, 0x50, 0x79, 0x60, 0x6c, 0x5d, 0x75,
    0x70, 0x6a, 0x68, 0x66, 0x52, 0x6e, 0x55, 0x4c, 0x7c, 0x51, 0x4e, 0x28, 0x31, 0x72, 0x7a, 0x40,
    0x49, 0x4e, 0x3b, 0x36, 0x6b, 0x69, 0x54, 0x37, 0x36, 0x70, 0x30, 0x3e, 0x3d, 0x60, 0x73, 0x42,
    0x5f, 0x61, 0x6f, 0x79, 0x7a, 0x36, 0x5e, 0x6d, 0x35, 0x40, 0x5f, 0x6d, 0x63, 0x57, 0x49, 0x31};
static wmkcByte testIv[32] = {
    0x7b, 0x75, 0x76, 0x37, 0x24, 0x59, 0x72, 0x29, 0x6e, 0x40, 0x5f, 0x65, 0x2d, 0x55, 0x75, 0x68,
    0x3d, 0x41, 0x78, 0x35, 0x48, 0x50, 0x7d, 0x73, 0x60, 0x4e, 0x33, 0x6f, 0x23, 0x47, 0x4c, 0x36};
#endif

wmkcErr_obj wmkcChat_main()
{
    wmkcErr_obj error;
    wmkcChat_obj *chat = wmkcNull;
    wmkcSize key_size;

    wmkcChat_new(&chat);

    key_size = chat->snc->KN * chat->snc->NR;
    // key_size = chat->snc->KN;

    wmkcChat_signup(chat);
    wmkcChat_getUserHash(chat);

    printf("用户名：%s\n", chat->name);
    printf("用户ID：%llu\n", chat->uid);
    printf("用户盐：\n");
    wmkcMisc_PRINT(chat->salt, WMKC_CHAT_SALT_SIZE, 32, false, true);
    printf("用户加密算法密钥：\n");
    wmkcMisc_PRINT(chat->snc->roundKey, key_size, 32, false, true);
    printf("用户加密算法初始向量：\n");
    wmkcMisc_PRINT(chat->snc->iv, SNC_BLOCKLEN, 32, false, true);
    printf("用户哈希：\n");
    wmkcMisc_PRINT(chat->hash, WMKC_CHAT_HASH_SIZE, 32, false, true);

    wmkcChat_free(&chat);
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

void test()
{
    
}

int main(int argc, char **argv)
{
    // wmkcChat_main();
    test();

    return 0;
}
