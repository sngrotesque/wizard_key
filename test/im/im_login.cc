#include <openssl/evp.h>
#include <openssl/rand.h>

#include <config/WukException.hh>
#include <WukBuffer.hh>
#include <WukTime.hh>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <format>

constexpr wU32 salt_length = 16;
constexpr wU32 hash_length = 32;

namespace im {
    struct _user_info {
        wI64 uid;
        std::string name;
        wuk::Buffer salt;
        wuk::Buffer pwd_hash;
        double created;
        bool active;
    };
    typedef _user_info UserInfo;

    wuk::WukTime wuk_time;

    wI64 generator_uid()
    {
        wI64 n{0};
        RAND_bytes(reinterpret_cast<wByte *>(&n), sizeof(n));
        return n & 0xffffffff;
    }

    wuk::Buffer generator_salt()
    {
        wByte salt[salt_length] {0};
        RAND_bytes(salt, salt_length);
        wuk::Buffer result(salt, salt_length);
        return result;
    }

    wuk::Buffer generator_hash(std::string password, wuk::Buffer salt)
    {
        wByte hash_value[hash_length] {0};
        PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                          salt.get_data(), salt.get_length(),
                          12408, EVP_sha256(),
                          hash_length, hash_value);
        wuk::Buffer result(hash_value, hash_length);
        return result;
    }

    double get_current_time()
    {
        return wuk_time.time();
    }

    UserInfo create_account(std::string name, std::string password)
    {
        UserInfo result {0};
        result.uid = generator_uid();
        result.name = name;
        result.salt = generator_salt();
        result.pwd_hash = generator_hash(password, result.salt);
        result.created = get_current_time();
        result.active = true;

        return result;
    }
}

std::string get_sql(im::UserInfo &info)
{
    std::string table_name("users");

    std::stringstream sql;
    sql << "INSERT INTO " << table_name << " "
        << "(uid, name, salt, pwd_hash, created, active)\n"
        << "VALUES\n"
        << "(" << info.uid
        << ", '" << info.name
        << "', '" << info.salt.hex()
        << "', '" << info.pwd_hash.hex()
        << "', " << std::fixed << std::setprecision(5) << info.created
        << ", " << ((info.active)?"TRUE":"FALSE") << ");";

    return sql.str();
}

int main()
{
    im::UserInfo info = im::create_account("test_user_0005", "12345678");

    std::string sql = get_sql(info);

    std::cout << sql << std::endl;

    return 0;
}
