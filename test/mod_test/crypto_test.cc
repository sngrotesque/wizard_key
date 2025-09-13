#include <crypto/WukOP4.hh>
#include <crypto/WukChaCha20.hh>
#include <crypto/WukHash.hh>
// #include <WukBinascii.hh>
#include <WukBuffer.hh>
#include <WukMisc.hh>

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <new>

#include <openssl/rand.h>
#include <openssl/evp.h>
#ifdef WUK_PLATFORM_WINOS
#   include <windows.h>
#endif

using namespace wuk::crypto;
using namespace wuk::misc;
namespace fs = std::filesystem;
constexpr wuk::u32 buffer_size = 4096;
constexpr wuk::u32 PBKDF2_ROUNDS = 415411;

wuk::Buffer derive_key(const std::string &password, const wuk::Buffer &salt, const wuk::u32 &dklen)
{
    wuk::Buffer derived;

    PKCS5_PBKDF2_HMAC(password.data(), password.length(),
                      salt.data(), salt.get_length(),
                      PBKDF2_ROUNDS, EVP_sha256(), dklen,
                      derived.write(dklen));

    return derived;
}

template <wuk::crypto::HashlibType Algo>
std::string file_hexdigest(const fs::path &path)
{
    std::fstream f(path, std::ios::in | std::ios::binary);
    if (!f.is_open()) {
        throw wuk::Exception(wuk::Error::FTOFO, "file_hexdigest",
            "failed to file optn.");
    }

    wuk::crypto::Hashlib<Algo> hash;
    wuk::Buffer buffer;

    for (;;) {
        f.read(reinterpret_cast<char *>(buffer.write(4096)), 4096);
        wuk::ulong length = f.gcount();
        if (length == 0) {
            break;
        }

        hash.update(buffer.data(), length);
    }

    return hash.hexdigest();
}

void file_xcrypt(const fs::path &in_path,
                 const fs::path &out_path,
                 const std::string &password,
                 bool encrypt)
{
    if (!fs::exists(in_path)) {
        throw wuk::Exception(wuk::Error::FNOTF, "file_encryption",
            log_utf8("输入路径的文件不存在。"));
    }
    std::fstream fin(in_path,   std::ios::in  | std::ios::binary);
    std::fstream fout(out_path, std::ios::out | std::ios::binary);

    if (!fin.is_open() || !fout.is_open()) {
        throw wuk::Exception(wuk::Error::FTOFO, "file_encryption",
            log_utf8("输入路径或输出路径的文件打开失败。"));
    }
    constexpr wuk::u32 salt_size = 16;
    const wuk::byte *key = nullptr;
    const wuk::byte *nonce = nullptr;
    wuk::Buffer salt;

    // 初始化密码套件
    if (encrypt) {
        RAND_bytes(salt.write(salt_size), salt_size);
        fout.write(salt.c_str(), salt.get_length());
    } else {
        fin.read(reinterpret_cast<char *>(salt.write(salt_size)), salt_size);
    }
    wuk::Buffer key_with_nonce = derive_key(password, salt, OP4_KL + OP4_NL);
    key = key_with_nonce.data();
    nonce = key_with_nonce.data() + wuk::crypto::OP4_KL;

    wuk::crypto::OP4 op4(key);

    // 准备缓冲区
    wuk::Buffer in_buffer(buffer_size);
    wuk::Buffer out_buffer(buffer_size);
    wuk::ulong in_length = 0;
    for (;;) {
        fin.read(reinterpret_cast<char *>(in_buffer.write(buffer_size)), buffer_size);

        if ((in_length = fin.gcount()) == 0) {
            break;
        }

        op4.ctr_stream(out_buffer.write(in_length), in_buffer.data(), in_length, nonce);
        fout.write(out_buffer.c_str(), in_length);
    }
}

void file_xcrypt_test()
{
    using HashType = wuk::crypto::HashlibType;
    fs::path plaintext("L:/");
    fs::path ciphertext("/home/sn/Desktop/sslkey.log.op4");
    fs::path decrypted("/home/sn/Desktop/sslkey.log.op4.log");
    std::string password("zzzzzzzzzzzzzz");

    try {
        file_xcrypt(plaintext, ciphertext, password, true);
        file_xcrypt(ciphertext, decrypted, password, false);

        std::cout << "Plaintext  sha256: " << file_hexdigest<HashType::SHA_256>(plaintext) << std::endl;
        std::cout << "ciphertext sha256: " << file_hexdigest<HashType::SHA_256>(ciphertext) << std::endl;
        std::cout << "decrypted  sha256: " << file_hexdigest<HashType::SHA_256>(decrypted) << std::endl;
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}

int main()
{
    file_xcrypt_test();

    return 0;
}
