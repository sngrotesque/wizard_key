#include <crypto/WukOP4.hh>
#include <WukBuffer.hh>

#include <openssl/evp.h>
#include <openssl/rand.h>
#ifdef WUK_PLATFORM_WINOS
#   include <windows.h>
#endif
#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;
constexpr wuk::u32 buffer_size = 4096;

std::string log_utf8(const std::string &utf8Str)
{
#   ifdef WUK_PLATFORM_WINOS
    // Step 1: UTF-8 → UTF-16
    wuk::i32 wlen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
    if (wlen == 0) return {};

    std::wstring wstr(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, wstr.data(), wlen);

    // Step 2: UTF-16 → 当前代码页（如GBK）
    wuk::i32 len = WideCharToMultiByte(CP_ACP, 0, wstr.data(), -1, nullptr, 0, nullptr, nullptr);
    if (len == 0) return {};

    std::string result(len, '\0');
    WideCharToMultiByte(CP_ACP, 0, wstr.data(), -1, result.data(), len, nullptr, nullptr);
    return result;
#   else
    return utf8Str;
#   endif
}

wuk::Buffer derive_key(const std::string &password,
                       const wuk::Buffer &salt,
                       const wuk::u32 &dklen = wuk::crypto::OP4_KL + wuk::crypto::OP4_NL)
{
    wuk::Buffer derived;

    PKCS5_PBKDF2_HMAC(password.data(), password.length(),
                      salt.get_data(), salt.get_length(),
                      102401, EVP_sha256(), dklen,
                      derived.append_write(dklen));

    return derived;
}

void file_xcrypt(const fs::path &in_path, const fs::path &out_path, const std::string &password, bool encrypt)
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
        RAND_bytes(salt.append_write(salt_size), salt_size);
        fout.write(salt.get_cstr(), salt.get_length());
    } else {
        fin.read(reinterpret_cast<char *>(salt.append_write(salt_size)), salt_size);
    }
    wuk::Buffer key_with_nonce = derive_key(password, salt);
    key = key_with_nonce.get_data();
    nonce = key_with_nonce.get_data() + wuk::crypto::OP4_KL;

    wuk::crypto::OP4 op4(key);

    // 准备缓冲区
    wuk::byte in_buffer[buffer_size];
    wuk::byte out_buffer[buffer_size];
    for (;;) {
        wuk::ulong in_len = fin.read(reinterpret_cast<char *>(in_buffer), buffer_size).gcount();
        if (in_len == 0) {
            break;
        }
        op4.ctr_stream(out_buffer, in_buffer, in_len, nonce);
        fout.write(reinterpret_cast<char *>(out_buffer), in_len);
    }
}

void file_xcrypt_test()
{
    fs::path plaintext("F:/Pitchers/二次元玉足/133343667_p0.png");
    fs::path ciphertext("133343667_p0.png.op4");
    fs::path decrypted("133343667_p0.png.op4.png");
    std::string password("1234567890");

    try {
        file_xcrypt(plaintext, ciphertext, password, true);
        file_xcrypt(ciphertext, decrypted, password, false);
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}

int main()
{
    

    return 0;
}
