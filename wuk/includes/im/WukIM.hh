#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <net/WukPacket.hh>
#include <net/WukSocket.hh>
#include <WukBuffer.hh>
#include <WukRandom.hh>

#include <openssl/evp.h>
#include <libpq-fe.h>

#include <atomic>
#include <mutex>

namespace wuk::im {
    class Snowflake {
    private:
        wuk::i64 datacenter_id = 0;
        wuk::i64 worker_id = 0;
        wuk::i64 sequence = 0;
        wuk::i64 last_timestamp = -1;
        std::mutex mutex;

    private:
        wuk::i64 current_timestamp() const;
        wuk::i64 wait_next_millis(wuk::i64 last_timestamp);

    public:
        Snowflake(wuk::i64 worker_id, wuk::i64 datacenter_id = 0);

    public:
        wuk::i64 generate_id();
    };

    class LIBWUK_API UserInfo {
    private:
        wuk::i64    uid;  // 用户ID
        std::string name; // 用户名
        wuk::Buffer salt; // 用户盐
        wuk::Buffer hash; // 用户密码对应的哈希
        wuk::f64    cadt; // 用户创建时间（Create Account Date）
        bool        active; // 账户启用状态

    public:
        UserInfo() = default;
        UserInfo(wuk::i64    uid,
                 std::string name,
                 wuk::Buffer salt,
                 wuk::Buffer hash,
                 wuk::f64    cadt,
                 bool        active);

        UserInfo &set_uid(const wuk::i64 &uid);
        UserInfo &set_name(const std::string &name);
        UserInfo &set_salt(const wuk::Buffer &salt);
        UserInfo &set_hash(const wuk::Buffer &hash);
        UserInfo &set_created_at_time(const wuk::f64 &timestamp);
        UserInfo &set_active(bool status);
    };

    UserInfo create_account(const std::string &name, const std::string &password)
    {
        wuk::Random random;
        wuk::Time timer;
        Snowflake sf(1);

        wuk::Buffer salt;
        wuk::Buffer hash;
        UserInfo info;

        random.bytes(salt.append_write(16), 16);

        PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                          salt.get_data(), salt.get_size(),
                          10524, EVP_sha256(),
                          32, hash.append_write(32));

        // info.uid = sf.generate_id();
        // info.name = name;
        // info.salt = salt;
        // info.hash = hash;
        // info.created = timer.time<double>();
        // info.active = true;

        return info;
    }
}

#endif