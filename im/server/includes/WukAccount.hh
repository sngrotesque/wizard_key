#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <server/includes/WukIM.hh>
#include <core/WukLIBSSL.hh>
#include <net/WukSocket.hh>
#include <utils/bytes.hh>
#include <db/WukPsql.hh>
#include <WukBuffer.hh>
#include <WukRandom.hh>
#include <WukTime.hh>

static constexpr wuk::u32 ITER_COUNT = 415411;

namespace wuk::im::server {
    enum class STATUS {
        NORMAL,  // 正常
        BANNED,  // 封禁
        MUTED,   // 禁言
        DELETED, // 删除
    };

    class LIBWUK_API Generator {
    private:
        wuk::Random rnd;
        wuk::Time tm;
        Snowflake sf;

    private:
        std::string random_string(const std::vector<char> &set,
                                  wuk::i32 min_size,
                                  wuk::i32 max_size);

    public:
        wuk::i64    uid();
        std::string name();
        std::string password();
        wuk::Buffer salt();
        wuk::Buffer hash(const std::string &password, const wuk::Buffer &salt);
        STATUS      status();
        wuk::f64    created();
        wuk::f64    modified();
        wuk::f64    accessed();
        wuk::f64    deleted();

    public:
        std::string uid_str();
        std::string name_str();
        std::string password_str();
        std::string salt_str();
        std::string hash_str(const std::string &password, const wuk::Buffer &salt);
        std::string status_str();
        std::string created_str();
        std::string modified_str();
        std::string accessed_str();
        std::string deleted_str();
    };

    class LIBWUK_API Account {
    private:
        wuk::net::Socket fd;
        wuk::Random random;
        wuk::Time tm;
        Snowflake snow;

    public:
        Account() = default;
        ~Account() = default;

        Account(const Account &other) = default;
        Account(Account &&other) = default;
        Account &operator=(const Account &other) = default;
        Account &operator=(Account &&other) = default;

        Account(const std::string &bind_addr, wuk::u16 bind_port);

    public:
        void hanlder();
    };
}

#endif
