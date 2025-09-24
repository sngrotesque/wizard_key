#include <core/WukLIBSSL.hh>
#include <db/WukPsql.hh>
#include <im/WukIM.hh>
#include <WukBuffer.hh>
#include <WukRandom.hh>
#include <WukTime.hh>
#include <WukMisc.hh>

#include <iostream>

// #define USE_LOG

#ifdef USE_LOG
#   define LOG_UTF8(x) wuk::misc::log_utf8(x)
#else
#   define LOG_UTF8(x) x
#endif

using namespace wuk::db;
constexpr wuk::u32 ITER_COUNT = 0x656b3U;

namespace userinfo {
    enum class ACCOUNT_STATUS {
        NORMAL,  // 正常
        BANNED,  // 封禁
        MUTED,   // 禁言
        DELETED, // 删除
    };

    std::string random_string(const std::vector<char> &set,
                              wuk::i32 min_size, wuk::i32 max_size)
    {
        wuk::Random random;
        wuk::ulong result_size = random.randint(min_size, max_size);
        std::string result(result_size, '\0');
        for (auto &item : result) {
            item = random.choice(set);
        }
        return result;
    }

    std::string generate_uid()
    {
        wuk::im::Snowflake sf(0);
        return fmt::format("{0}", sf.generate_id() & 0xffffffff);
    }

    std::string generate_name()
    {
        constexpr char name[] = \
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        constexpr wuk::ulong size = sizeof(name) - 1;
        std::vector<char> name_set(name, name + size);

        return random_string(name_set, 6, 12);
    }

    std::string generate_password()
    {
        constexpr char password[] = \
            R"(0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz)"
            R"(!"#%&'()*+,-./:;<=>?@[\]^_`{|}~)";
        constexpr wuk::ulong password_size = sizeof(password) - 1;
        std::vector<char> password_set(password, password + password_size);

        return random_string(password_set, 8, 16);
    }

    wuk::Buffer generate_salt()
    {
        wuk::Random random;
        return random.bytes(16);
    }

    wuk::Buffer generate_hash(const std::string &password, const wuk::Buffer &salt)
    {
        wuk::Buffer user_hash(32);
        PKCS5_PBKDF2_HMAC(password.data(), password.size(),
                        salt.data(), salt.size(),
                        ITER_COUNT, EVP_sha256(),
                        32, user_hash.write(32));
        return user_hash;
    }

    std::string generate_status(ACCOUNT_STATUS status)
    {
        switch (status) {
            case ACCOUNT_STATUS::NORMAL:  return "NORMAL";
            case ACCOUNT_STATUS::BANNED:  return "BANNED";
            case ACCOUNT_STATUS::MUTED:   return "MUTED";
            case ACCOUNT_STATUS::DELETED: return "DELETED";
        }
        return "NORMAL";
    }

    std::string generate_time()
    {
        wuk::Time time;
        return fmt::format("{0:.5f}", time.time<double>());
    }
}

class Account {
private:
    psql::Work work;

private:
    psql::Connection create_conninfo()
    {
        std::string conninfo = std::string(
            "host=47.79.146.143 "
            "port=54324 "
            "dbname=im "
            // "user=test password=psql_test_user"
            "user=postgres password=sngrotesque_sql"
        );
        return psql::Connection(conninfo);
    }

public:
    Account() : work(psql::Work(this->create_conninfo())) {}

    bool query_uid_exists(psql::Work &work, const std::string &col_name, const std::string &uid)
    {
        std::string sql = fmt::format(
            "SELECT EXISTS(SELECT NULL FROM test WHERE {0} = $1);",
            col_name
        );
        std::vector<psql::Param> params{psql::Param{uid, 0}};
        psql::Result result = work.exec(sql, params, psql::ResultFormat::TEXT);
        return result[0][0][0] == 't';
    }

    template <bool check_succ>
    bool create()
    {
        std::string uid;

        while (true) {
            uid = userinfo::generate_uid();
            if (this->query_uid_exists(this->work, "uid", uid)) {
                std::cout << LOG_UTF8("UID已存在，重新生成。") << std::endl;
                continue;
            }
            break;
        }
        std::string name;
        std::string password;

        std::cout << LOG_UTF8("请输入用户名：");
        std::getline(std::cin, name);

        std::cout << LOG_UTF8("请输入密码：");
        std::getline(std::cin, password);

        if (name.empty()) {
            name = "rand_" + userinfo::generate_name();
            std::cout << LOG_UTF8(fmt::format("未输入名字，生成随机名字：{0}", name)) << std::endl;
        }
        if (password.empty()) {
            password = userinfo::generate_password();
            std::cout << LOG_UTF8(fmt::format("未输入密码，生成随机密码：{0}", password)) << std::endl;
        }

        wuk::Buffer salt = userinfo::generate_salt();
        wuk::Buffer hash = userinfo::generate_hash(password, salt);
        std::string status = userinfo::generate_status(userinfo::ACCOUNT_STATUS::NORMAL);
        std::string created = userinfo::generate_time();
        std::string modified = created;
        std::string accessed = created;
        std::string deleted = "0";

        std::string salts = salt.to_hex();
        std::string hashs = hash.to_hex();

        std::cout << fmt::format("uid:      {0}", uid)      << std::endl;
        std::cout << fmt::format("name:     {0}", name)     << std::endl;
        std::cout << fmt::format("salt:     {0}", salts)    << std::endl;
        std::cout << fmt::format("hash:     {0}", hashs)    << std::endl;
        std::cout << fmt::format("status:   {0}", status)   << std::endl;
        std::cout << fmt::format("created:  {0}", created)  << std::endl;
        std::cout << fmt::format("modified: {0}", modified) << std::endl;
        std::cout << fmt::format("accessed: {0}", accessed) << std::endl;
        std::cout << fmt::format("deleted:  {0}", deleted)  << std::endl;

        std::cout << LOG_UTF8("正在将数据写入数据库...") << std::endl;
        std::string sql(
            "INSERT INTO test (uid, name, salt, hash, status, created, modified, accessed, deleted) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9);");
        std::vector<psql::Param> params{
            psql::Param{uid,           0},
            psql::Param{name,          0},
            psql::Param{salt.to_str(), 1},
            psql::Param{hash.to_str(), 1},
            psql::Param{status,        0},
            psql::Param{created,       0},
            psql::Param{modified,      0},
            psql::Param{accessed,      0},
            psql::Param{deleted,       0},
        };

        this->work.exec(sql, params, psql::ResultFormat::TEXT);

        if constexpr (check_succ) {
            if (this->query_uid_exists(this->work, "uid", uid)) {
                std::cout << LOG_UTF8("数据写入数据库成功。") << std::endl;
            } else {
                std::cout << LOG_UTF8("数据写入数据库失败，请重试。") << std::endl;
            }
        } else {
            std::cout << LOG_UTF8("数据写入数据库成功。") << std::endl;
        }

        return true;
    }
};

int main()
{
    try {
        constexpr wuk::i32 count = 1;
        Account acc;
        for (wuk::i32 r = 0; r < count; ++r) {
            acc.create<false>();
        }
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

