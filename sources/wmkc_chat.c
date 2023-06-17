#include <wmkc_chat.h>

/**
 * @brief 初始化对应用户的盐和密钥与初始向量
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcChat对象的地址
 * @param password 这是一个指针，指向用户的密码口令字符串的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PRIVATE(wmkcErr_obj) wmkcChat_initCipher
WMKC_OF((wmkcChat_obj *obj, wmkcSTR password))
{
    wmkcErr_obj error;
    if(!obj || !password) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcChat_initCipher: obj or password is NULL.");
    }

    wmkcByte *_key = wmkcNull; // 声明临时密钥指针
    wmkcByte *_iv = wmkcNull;  // 声明临时初始向量指针
    wmkcByte *buf = wmkcNull;  // 声明缓冲区指针

    // 为缓冲区申请内存空间
    if(!wmkcMemoryNew(wmkcByte *, buf, obj->snc->KN + SNC_BLOCKLEN)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcChat_initCipher: buf failed to apply for memory.");
    }
    // 初始化缓冲区和临时盐
    wmkc_secureMemory(buf, obj->snc->KN + SNC_BLOCKLEN);

    // 将密钥和初始向量指针指向正确的缓冲区位置
    _key = buf;
    _iv = buf + obj->snc->KN;

    // 使用系统函数来生成密码学安全随机数
    error = wmkcRandom_urandom(obj->salt, WMKC_CHAT_SALT_SIZE);
    if(error.code) return error;

    // 通过用户密码口令和盐生成SNC加密算法的密钥和初始向量并储存至缓冲区。
    PKCS5_PBKDF2_HMAC(password, strlen(password), obj->salt, WMKC_CHAT_SALT_SIZE,
        WMKC_CHAT_ITERATIONS, EVP_sha384(), obj->snc->KN + SNC_BLOCKLEN, buf);

    // 使用缓冲区内的密钥与初始向量来初始化SNC加密算法
    wmkcSNC_init(obj->snc, _key, _iv);

    // 将缓冲区的内容清零并释放其内存空间
    wmkc_secureMemory(buf, obj->snc->KN + SNC_BLOCKLEN);
    wmkcMemoryFree(buf);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PRIVATE(wmkcErr_obj) wmkcChat_binascii_a2b
WMKC_OF((wmkcByte *dst, wmkcByte *src, wmkcSize srcSize))
{
    wmkcErr_obj error;
    if(!dst || !src || !srcSize) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcChat_initCipher: dst or src or srcSize is NULL.");
    }
    wmkcByte *tmp_dst = wmkcNull;

    error = wmkcBinascii_a2b_hex(&tmp_dst, src, srcSize);
    if(error.code) return error;

    memcpy(dst, tmp_dst, srcSize >> 1);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 给wmkcChat对象申请内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcChat对象指针的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcChat_new WMKC_OPEN_API
WMKC_OF((wmkcChat_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcChat_new: obj is NULL.");
    }
    // 为wmkcChat对象申请内存空间
    if(!wmkcMemoryNew(wmkcChat_obj *, (*obj), sizeof(wmkcChat_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcChat_new: (*obj) failed to apply for memory.");
    }

    // 将wmkcChat对象的所有成员初始化
    wmkc_secureMemory((*obj)->name, WMKC_CHAT_NAME_SIZE);
    wmkc_secureMemory((*obj)->hash, WMKC_CHAT_HASH_SIZE);
    wmkc_secureMemory((*obj)->salt, WMKC_CHAT_SALT_SIZE);
    error = wmkcSNC_new(&((*obj)->snc), WMKC_CHAT_CIPHER_MODE);

    // 如果SNC_new函数出现错误就返回错误代码
    if(error.code) return error;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 释放wmkcChat对象的内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcChat对象指针的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcChat_free WMKC_OPEN_API
WMKC_OF((wmkcChat_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcChat_free: obj is NULL.");
    }

    // 将wmkcChat对象的所有成员清零并释放内存空间以及wmkcChat对象的内存空间
    wmkc_secureMemory((*obj)->name, WMKC_CHAT_NAME_SIZE);
    wmkc_secureMemory((*obj)->hash, WMKC_CHAT_HASH_SIZE);
    wmkc_secureMemory((*obj)->salt, WMKC_CHAT_SALT_SIZE);
    error = wmkcSNC_free(&((*obj)->snc));
    if(error.code) return error;
    wmkcMemoryFree((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 创建用户并计算出其对应的数据内容
 * @authors SN-Grotesque
 * @note 对于UID，目前的生成方式不能采用，请后续修改
 * @param obj 这是一个指针，指向wmkcChat对象的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcChat_signup WMKC_OPEN_API
WMKC_OF((wmkcChat_obj *obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcChat_signup: obj is NULL.");
    }

    // 声明用户名变量与密码口令变量
    wmkcChar name[WMKC_CHAT_NAME_SIZE + 1];
    wmkcChar pass[WMKC_CHAT_PASSWORD_SIZE + 1];

    // 将name与pass指向的内存地址的内容清零
    wmkc_secureMemory(name, WMKC_CHAT_NAME_SIZE + 1);
    wmkc_secureMemory(pass, WMKC_CHAT_PASSWORD_SIZE + 1);

    printf("请输入用户名(Max: %u bytes.)：", WMKC_CHAT_NAME_SIZE);
    wmkc_scanf((wmkcByte *)name, WMKC_CHAT_NAME_SIZE);
    printf("请输入密码(Max: %u bytes.)：", WMKC_CHAT_PASSWORD_SIZE);
    wmkc_scanf((wmkcByte *)pass, WMKC_CHAT_PASSWORD_SIZE);

    // 将用户名复制到wmkcChat对象的name成员中
    memcpy(obj->name, name, strlen(name));

    // 初始化对应用户的盐和密钥与初始向量
    error = wmkcChat_initCipher(obj, pass);
    if(error.code) return error;

    // 使用随机函数生成UID
    wmkcRandom_seed();
    obj->uid = wmkcRandom_randint(1, 999999);

    // 将密码口令清零并交由操作系统释放
    wmkc_secureMemory(pass, WMKC_CHAT_PASSWORD_SIZE);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 生成用户哈希摘要
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcChat对象的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcChat_getUserHash WMKC_OPEN_API
WMKC_OF((wmkcChat_obj *obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcChat_getUserHash: obj is NULL.");
    }

    // 声明EVP_MD对象，声明wmkcHash对象
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    wmkcHash_obj *hash = wmkcNull;

    // 为wmkcHash申请内存空间并初始化EVP_MD对象
    wmkcHash_new(&hash, wmkcHash_SHA256);
    EVP_DigestInit_ex(md_ctx, hash->md, wmkcNull);

    /*
    * 使用以下内容来计算一个用户的哈希值
    *     obj->uid             size of 8
    *     obj->name            size of strlen(obj->name)
    *     obj->salt            size of WMKC_CHAT_SALT_SIZE
    *     obj->snc->iv         size of SNC_BLOCKLEN
    *     obj->snc->roundKey   size of (obj->snc->KN * obj->snc->NR)
    */
    EVP_DigestUpdate(md_ctx, &obj->uid, sizeof(obj->uid));
    EVP_DigestUpdate(md_ctx, obj->name, strlen(obj->name));
    EVP_DigestUpdate(md_ctx, obj->salt, WMKC_CHAT_SALT_SIZE);
    EVP_DigestUpdate(md_ctx, obj->snc->iv, SNC_BLOCKLEN);
    EVP_DigestUpdate(md_ctx, obj->snc->roundKey, obj->snc->KN * obj->snc->NR);

    // 释放EVP_MD对象
    EVP_DigestFinal_ex(md_ctx, hash->digest, wmkcNull);
    EVP_MD_CTX_free(md_ctx);

    // 将哈希摘要复制到wmkcChat对象的hash成员中
    memcpy(obj->hash, hash->digest, WMKC_CHAT_HASH_SIZE);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcChat_loadUserInfo WMKC_OPEN_API
WMKC_OF((wmkcChat_obj *obj, wmkcFileString fn))
{
    wmkcErr_obj error;
    if(!obj || !fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcChat_loadUserInfo: obj or fn is NULL.");
    }

    cJSON    *json     = wmkcNull; // 声明cJSON对象
    wmkcChar *jString  = wmkcNull; // 声明JSON字符串指针
    wmkcSize  fileSize = 0;        // 声明文件长度变量

    // 将json文件的内容传入cJSON对象
    wmkcFile_fread((wmkcByte **)&jString, &fileSize, fn);
    json = cJSON_Parse(jString);

    // 将临时变量中的数据存入wmkcChat对象
    obj->uid = (wmkcSize)cJSON_GetNumberValue(cJSON_GetObjectItem(json, "uid"));
    memcpy(obj->name, cJSON_GetStringValue(cJSON_GetObjectItem(json, "name")),
        strlen(cJSON_GetStringValue(cJSON_GetObjectItem(json, "name"))));
    wmkcChat_binascii_a2b(obj->hash,
        (wmkcByte *)cJSON_GetStringValue(cJSON_GetObjectItem(json, "hash")),
        WMKC_CHAT_HASH_SIZE << 1);
    wmkcChat_binascii_a2b(obj->salt,
        (wmkcByte *)cJSON_GetStringValue(cJSON_GetObjectItem(json, "salt")),
        WMKC_CHAT_SALT_SIZE << 1);

    // 释放内存与保证内存安全
    // 因为cJSON对象是个链表，所以只需要执行cJSON_Delete(json)即可。
    cJSON_Delete(json);
    wmkc_secureMemory(jString, fileSize);
    wmkcMemoryFree(jString);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcChat_saveUserInfo WMKC_OPEN_API
WMKC_OF((wmkcChat_obj *obj, wmkcFileString fn))
{
    wmkcErr_obj error;
    if(!obj || !fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcChat_saveUserInfo: obj or fn is NULL.");
    }

    /*
    * 读取用户信息时，请使用用户提供的密码口令来生成密钥和初始向量，
    * 之后使用读取的盐和哈希来判断此用户是否为正确用户。
    */
    cJSON    *json = wmkcNull; // 声明cJSON对象
    wmkcByte *hash_hex = wmkcNull;         // 声明哈希十六进制字符串
    wmkcByte *salt_hex = wmkcNull;         // 声明盐十六进制字符串
    wmkcChar *jString = wmkcNull;          // 声明JSON字符串

    // 如果cJSON对象创建失败
    if(!(json = cJSON_CreateObject())) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcChat_saveUserInfo: json failed to apply for memory.");
    }

    // 将wmkcChat对象中的哈希与盐转换为十六进制字符串
    error = wmkcBinascii_b2a_hex(&hash_hex, obj->hash, WMKC_CHAT_HASH_SIZE);
    if(error.code) return error;
    error = wmkcBinascii_b2a_hex(&salt_hex, obj->salt, WMKC_CHAT_SALT_SIZE);
    if(error.code) return error;

    // 将wmkcChat对象的数据传入到cJSON对象中
    cJSON_AddNumberToObject(json, "uid", obj->uid);
    cJSON_AddStringToObject(json, "name", obj->name);
    cJSON_AddStringToObject(json, "hash", hash_hex);
    cJSON_AddStringToObject(json, "salt", salt_hex);

    // 将cJSOn对象转为字符串用于保存为文件
    jString = cJSON_Print(json);
    wmkcFile_fwrite((wmkcByte *)jString, strlen(jString), fn);

    // 释放内存与保证内存安全
    cJSON_Delete(json);
    cJSON_free(jString);
    wmkc_secureMemory(hash_hex, WMKC_CHAT_HASH_SIZE << 1);
    wmkc_secureMemory(salt_hex, WMKC_CHAT_SALT_SIZE << 1);
    wmkcMemoryFree(hash_hex);
    wmkcMemoryFree(salt_hex);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

wmkcErr_obj wmkcChat_main()
{
    wmkcErr_obj error;
    wmkcChat_obj *chat = wmkcNull;
    wmkcBool read = 0;

    wmkcChat_new(&chat);

    if(read) {
        wmkcChat_loadUserInfo(chat, wmkcFile_text("user_info/user.json"));
        printf("用户ID：%llu\n", chat->uid);
        printf("用户名：%s\n",   chat->name);
        printf("用户哈希：\n");
        wmkcMisc_PRINT(chat->hash, WMKC_CHAT_HASH_SIZE, WMKC_CHAT_HASH_SIZE, false, true);
        printf("用户盐：\n");
        wmkcMisc_PRINT(chat->salt, WMKC_CHAT_SALT_SIZE, WMKC_CHAT_SALT_SIZE, false, true);
    } else {
        wmkcChat_signup(chat);
        wmkcChat_getUserHash(chat);
        wmkcChat_saveUserInfo(chat, wmkcFile_text("user_info/user.json"));
        printf("用户ID：%llu\n", chat->uid);
        printf("用户名：%s\n", chat->name);
        printf("用户哈希：\n");
        wmkcMisc_PRINT(chat->hash, WMKC_CHAT_HASH_SIZE, WMKC_CHAT_HASH_SIZE, 0, 1);
        printf("用户盐：\n");
        wmkcMisc_PRINT(chat->salt, WMKC_CHAT_SALT_SIZE, WMKC_CHAT_SALT_SIZE, 0, 1);
        printf("用户密码模式：\n\tSNC-%d\n", (chat->snc->mode + 1) * 256);
        printf("用户IV：\n");
        wmkcMisc_PRINT(chat->snc->iv, SNC_BLOCKLEN, SNC_BLOCKLEN, 0, 1);
        printf("用户KEY：\n");
        wmkcMisc_PRINT(chat->snc->roundKey, chat->snc->KN * chat->snc->NR, SNC_BLOCKLEN, 0, 1);
    }

    wmkcChat_free(&chat);
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

