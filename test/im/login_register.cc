#include <im/WukPsql.hh>
#include <im/WukIM.hh>
#include <WukBinascii.hh>
#include <WukRandom.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <windows.h>
#include <iostream>

std::string log_utf8(const std::string &utf8Str)
{
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
}

wuk::Buffer derive_sha256_digest(const std::string &password,
                                 const wuk::Buffer &salt)
{
    wuk::Buffer derived;
    wuk::u32 dklen = 32;

    PKCS5_PBKDF2_HMAC(password.data(), password.length(),
                      salt.data(), salt.get_length(),
                      102401, EVP_sha256(), dklen,
                      derived.write(dklen));

    return derived;
}

class LIBWUK_API Account {
private:
    wuk::im::Psql psql;

public:
    Account()
    {
        psql.connect_db("host=127.0.0.1 port=5432 user=postgres dbname=im password=");
    }

    void sign_up(const std::string &name, const std::string &password)
    {
        wuk::im::Snowflake sf(0);
        wuk::im::UserInfo info;
        wuk::Time time;

        wuk::Buffer salt;
        wuk::Buffer hash;

        RAND_bytes(salt.write(16), 16);
        hash = derive_sha256_digest(password, salt);

        info.set_uid(sf.generate_id() & 0xffffffffU)
            .set_name(name)
            .set_salt(std::move(salt))
            .set_hash(std::move(hash))
            .set_status(wuk::im::AccountStatus::NORMAL)
            .set_created(time.time<wuk::f64>());

        std::string uid_res = this->psql.query("SELECT uid FROM users where uid = $1",
                                              {info.get_uid_str()});
        if (!uid_res.empty()) {
            // 如果不为空，则说明此id已存在，将直接放弃。
            return;
        }

        std::string insert_sql(
            "INSERT INTO users (uid, name, salt, hash, status, created, modified, accessed, deleted)\n"
            "VALUES ($1, $2, $3::bytea, $4, $5, $6, $7, $8, $9)");
        std::vector<std::string> insert_params;
        insert_params = info.get_params();

        this->psql.insert(insert_sql, insert_params);
    }

    void login(const std::string &name, const std::string &password)
    {
        
    }
};

std::string random_username(wuk::Random &random, wuk::i32 length)
{
    constexpr char charset[] = {
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
    };
    constexpr wuk::i32 charset_size = sizeof(charset) - 1;

    std::string username(length, '\0');
    for (wuk::i32 i = 0; i < length; ++i) {
        auto charset_index = random.randint(0, charset_size - 1);
        username[i] = charset[charset_index];
    }

    return username;
}

int main()
{
    try {
        wuk::Random random;
        Account acc;

        acc.sign_up("sngrotesque", "12345678");
        for (wuk::u32 i = 0; i < 31; ++i) {
            acc.sign_up(random_username(random, random.randint(6, 10)), "12345678");
        }
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
