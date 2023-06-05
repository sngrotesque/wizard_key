#include <network/wmkc_net.h>
#include <crypto/snc.h>
#include <wmkc_binascii.h>
#include <wmkc_object.h>
#include <wmkc_stream.h>
#include <wmkc_random.h>
#include <wmkc_struct.h>
#include <wmkc_hash.h>
#include <wmkc_file.h>
#include <wmkc_misc.h>

#include <network/wmkc_net.c>
#include <crypto/snc.c>
#include <wmkc_binascii.c>
#include <wmkc_object.c>
#include <wmkc_stream.c>
#include <wmkc_random.c>
#include <wmkc_struct.c>
#include <wmkc_hash.c>
#include <wmkc_file.c>
#include <wmkc_misc.c>

#define WIZ_CHAT_USERNAME_SIZE 64 // 用户名的字节长度
#define WIZ_CHAT_PASSWORD_SIZE 32 // 密码的字节长度

#define WIZ_CHAT_CIPHER_MODE SNC_768 // 加密算法密钥模式
#if (WIZ_CHAT_CIPHER_MODE == SNC_768)
#define WIZ_CHAT_CIPHER_KEY_SIZE 96
#elif (WIZ_CHAT_CIPHER_MODE == SNC_512)
#define WIZ_CHAT_CIPHER_KEY_SIZE 64
#elif (WIZ_CHAT_CIPHER_MODE == SNC_256)
#define WIZ_CHAT_CIPHER_KEY_SIZE 32
#endif /* (WIZ_CHAT_CIPHER_MODE == 2) */

#define WIZ_CHAT_HASH_SIZE 32 // 哈希摘要的长度
#define WIZ_CHAT_SALT_SIZE 32 // 盐的长度

typedef struct {
    wmkcSize  id;   // 用户ID
    wmkcChar *name; // 用户名
    wmkcByte *hash; // 用户哈希值，用于查找用户
    wmkcByte *salt; // 用户的盐，用于生成固定的密钥
    wmkcByte *key;  // 用户对应的加密算法密钥
    wmkcByte *iv;   // 用户对应的加密算法初始向量
} WizChat_obj;

/**
 * @brief 给WizChat对象申请内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param 无
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) WizChat_new WMKC_OPEN_API
WMKC_OF((WizChat_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "WizChat_new: obj is NULL.");
    }
    if(!wmkcMemoryNew(WizChat_obj *, (*obj), sizeof(WizChat_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "WizChat_new: (*obj) failed to apply for memory.");
    }

    if(!wmkcMemoryNew(wmkcChar *, (*obj)->name, WIZ_CHAT_USERNAME_SIZE)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "WizChat_new: (*obj)->name failed to apply for memory.");
    }

    if(!wmkcMemoryNew(wmkcByte *, (*obj)->hash, WIZ_CHAT_HASH_SIZE)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "WizChat_new: (*obj)->hash failed to apply for memory.");
    }

    if(!wmkcMemoryNew(wmkcByte *, (*obj)->salt, WIZ_CHAT_SALT_SIZE)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "WizChat_new: (*obj)->salt failed to apply for memory.");
    }

    if(!wmkcMemoryNew(wmkcByte *, (*obj)->key, WIZ_CHAT_CIPHER_KEY_SIZE)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "WizChat_new: (*obj)->key failed to apply for memory.");
    }

    if(!wmkcMemoryNew(wmkcByte *, (*obj)->iv, SNC_BLOCKLEN)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "WizChat_new: (*obj)->iv failed to apply for memory.");
    }

    wmkcMemoryZero((*obj)->name, WIZ_CHAT_USERNAME_SIZE);
    wmkcMemoryZero((*obj)->hash, WIZ_CHAT_HASH_SIZE);
    wmkcMemoryZero((*obj)->salt, WIZ_CHAT_SALT_SIZE);
    wmkcMemoryZero((*obj)->key, WIZ_CHAT_CIPHER_KEY_SIZE);
    wmkcMemoryZero((*obj)->iv, SNC_BLOCKLEN);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) WizChat_free WMKC_OPEN_API
WMKC_OF((WizChat_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "WizChat_new: obj is NULL.");
    }

    wmkcMemoryFree((*obj)->name);
    wmkcMemoryFree((*obj)->hash);
    wmkcMemoryFree((*obj)->salt);
    wmkcMemoryFree((*obj)->key);
    wmkcMemoryFree((*obj)->iv);
    wmkcMemoryFree((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) WizChat_salt WMKC_OPEN_API
WMKC_OF((WizChat_obj *obj, wmkcString password))
{
    wmkcErr_obj error;
    if(!obj || !password) {
        wmkcErr_return(error, wmkcErr_ErrMemory, "WizChat_new: obj or password is NULL.");
    }
    wmkcByte buf[WIZ_CHAT_CIPHER_KEY_SIZE + SNC_BLOCKLEN];
    wmkcByte salt[WIZ_CHAT_SALT_SIZE];

    // 生成安全随机数
    error = wmkcRandom_urandom(salt, WIZ_CHAT_SALT_SIZE);
    if(error.code) {
        return error;
    }

    memcpy(obj->salt, salt, WIZ_CHAT_SALT_SIZE);

    // 通过密码口令和盐生成对应的加密算法的密钥和初始向量。
    PKCS5_PBKDF2_HMAC(password, strlen(password), obj->salt, WIZ_CHAT_SALT_SIZE, 16,
        EVP_sha384(), WIZ_CHAT_CIPHER_KEY_SIZE + SNC_BLOCKLEN, buf);

    memcpy(obj->key, buf, WIZ_CHAT_CIPHER_KEY_SIZE);
    memcpy(obj->iv, buf + WIZ_CHAT_CIPHER_KEY_SIZE, SNC_BLOCKLEN);
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) WizChat_createUser WMKC_OPEN_API
WMKC_OF((WizChat_obj *obj))
{
    wmkcErr_obj error;
    wmkcChar name[WIZ_CHAT_USERNAME_SIZE];
    wmkcChar pass[WIZ_CHAT_PASSWORD_SIZE];
    wmkcByte uid[8];
    printf("你现在在注册新账户。\n\n");

    printf("请输入用户名(Max: %u bytes.)：", WIZ_CHAT_USERNAME_SIZE);
    wmkcStream_Scanf((wmkcByte *)name, WIZ_CHAT_USERNAME_SIZE);
    printf("请输入密码(Max: %u bytes.)：", WIZ_CHAT_PASSWORD_SIZE);
    wmkcStream_Scanf((wmkcByte *)pass, WIZ_CHAT_PASSWORD_SIZE);

    WizChat_salt(obj, pass);

    memcpy(obj->name, name, strlen(name));
    wmkcRandom_urandom(uid, 8);
    wmkcStruct_unpack(">Q", &obj->id, uid);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

wmkcErr_obj WizChat_main()
{
    wmkcErr_obj error;
    WizChat_obj *wiz = wmkcNull;

    WizChat_new(&wiz);

    WizChat_createUser(wiz);

    printf("用户名：%s\n", wiz->name);
    printf("用户ID：%llu\n", wiz->id);
    printf("用户盐：\n");
    wmkcMisc_PRINT(wiz->salt, WIZ_CHAT_SALT_SIZE, 16, true, true);
    printf("用户哈希：\n");
    wmkcMisc_PRINT(wiz->hash, WIZ_CHAT_HASH_SIZE, 16, true, true);
    printf("用户加密算法密钥：\n");
    wmkcMisc_PRINT(wiz->key, WIZ_CHAT_CIPHER_KEY_SIZE, 16, true, true);
    printf("用户加密算法初始向量：\n");
    wmkcMisc_PRINT(wiz->iv, SNC_BLOCKLEN, 16, true, true);

    WizChat_free(&wiz);
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

int main(int argc, char **argv)
{
    WizChat_main();

    return 0;
}

