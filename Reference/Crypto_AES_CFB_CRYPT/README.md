> git@github.com:sstutanotam/aes_cfb_crypt.git

# aes_cfb_crypt

unsigned char *key = (unsigned char *)malloc(32);

unsigned char *iv = (unsigned char *)malloc(16);

//set key and iv :

sha384_key_iv(user_password, strlen(user_password), key, iv);


random_iv = (unsigned char *)malloc(16);

//set random_iv :

randomIV(random_iv);


//encrypt

AES_KEY aesKey;

int setKey = AES_set_encrypt_key((const unsigned char *)key, 256, &aesKey);

AES_cfb128_encrypt(random_iv, encrypted_text, 16, (const void *)&aesKey, iv, &num, AES_ENCRYPT);

AES_cfb128_encrypt(plain_text, encrypted_text + 16, strlen(plain_text), (const void *)&aesKey, random_iv, &num, AES_ENCRYPT);


//decrypt

AES_KEY aesKey;

int setKey = AES_set_encrypt_key((const unsigned char *)key, 256, &aesKey);

AES_cfb128_encrypt(encrypted_text, random_iv, 16, (const void *)&aesKey, iv, &num, AES_DECRYPT);

AES_cfb128_encrypt(encrypted_text + 16, plain_text, strlen(encrypted_text) - 16, (const void *)&aesKey, random_iv, &num, AES_DECRYPT);

我试图通过以下的方式增加安全性：
通过对用户密码取 sha384
取到的结果 后16个字节 作为 IV
前面的32个字节 作为 KEY
然后取一个16字节的随机数 作为 IV2
用 KEY和IV2 加密数据
用 KEY和IV 加密 IV2
然后把 加密后的IV2 拼接到 加密后的数据 前面
# --------------------------------------------------------------
# build : 

windows with mingw :

gcc *.c -O3 -o cryptFile.exe -static -Wall


linux :

gcc *.c -O3 -o cryptFile -static -Wall


android with NDK :

D:\ndk\android-ndk-r13c\toolchains\arm-linux-androideabi-4.9\prebuilt\windows\bin\arm-linux-androideabi-gcc --sysroot=D:\ndk\android-ndk-r13c\platforms\android-19\arch-arm -o cryptFileAndroid sha512.c cryptFile.c cfb128.c aes_core.c aes_cfb_crypt.c -std=gnu99 -O3  -Wall -pie -fPIE -static

OR call it with jni


# --------------------------------------------------------------
# License : 

follow OpenSSL 1.1.1
