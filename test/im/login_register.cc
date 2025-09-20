#if 1
#include <db/WukPsql.hh>
#include <im/WukIM.hh>
#include <WukBinascii.hh>
#include <WukRandom.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <windows.h>
#include <iostream>

static wuk::Buffer derive_sha256_digest(const std::string &password,
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
    wuk::db::psql::Connection conn;
    wuk::db::psql::Work work;
    wuk::db::psql::Result result;

public:
    Account()
    {
        conn.connect(
            "host=47.79.146.143 "
            "port=54324 "
            "user=test "
            "password=psql_test_user "
            "dbname=im");
        work = std::move(wuk::db::psql::Work(std::move(conn)));
    }

    void sign_up(const std::string &name, const std::string &password)
    {
        wuk::im::Snowflake sf(0);
        wuk::im::UserInfo info;
        wuk::Time time;

        wuk::Buffer salt;
        wuk::Buffer hash;

        // RAND_bytes(salt.write(16), 16);
        // hash = derive_sha256_digest(password, salt);

        info.set_uid(sf.generate_id() & 0xffffffffU)
            .set_name(name)
            .set_created(time.time<wuk::f64>());

        result = work.exec("SELECT uid FROM test where uid = $1;",
            {wuk::db::psql::Param{info.get_uid_str(), 0}},
            wuk::db::psql::ResultFormat::TEXT);
        if (result.get_row_count()) {
            // 如果不为空，则说明此id已存在，将直接放弃。
            return;
        }

        result = work.exec("INSERT INTO test (uid, name, created) VALUES ($1, $2, $3)",
            {
                wuk::db::psql::Param{info.get_uid_str(), 0},
                wuk::db::psql::Param{info.get_name_str(), 0},
                wuk::db::psql::Param{info.get_created_str(), 0},
            },
            wuk::db::psql::ResultFormat::TEXT);
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
        // for (wuk::u32 i = 0; i < 31; ++i) {
        //     acc.sign_up(random_username(random, random.randint(6, 10)), "12345678");
        // }
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
#endif
