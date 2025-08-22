#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
// #include <net/WukPacket.hh>
// #include <net/WukSocket.hh>
#include <WukBuffer.hh>

#include <openssl/evp.h>
#include <libpq-fe.h>

#include <vector>
#include <mutex>

namespace wuk::im {
    class LIBWUK_API Snowflake {
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

    enum class AccountStatus {
        NORMAL, // 正常
        BANNED, // 封禁
        MUTED,  // 禁言
        DELETED // 删除
    };
}

namespace wuk::im {
    // 详见：[im_users.sql](test\im\im_users.sql)
    class LIBWUK_API UserInfo {
    private:
        wuk::i64      m_uid{0};      // 用户UID，非空且唯一
        std::string   m_name;        // 用户名，最大32字符，非空
        wuk::Buffer   m_salt;        // 用户账户对应的盐，16字节，非空
        wuk::Buffer   m_hash;        // 用户账户密码对应的哈希，32字节（SHA-256），非空
        AccountStatus m_status;      // 用户状态，非空，默认NORMAL
        wuk::f64      m_created{0};  // 账号创建时间，非空，默认0
        wuk::f64      m_modified{0}; // 账号修改时间，非空，默认0
        wuk::f64      m_accessed{0}; // 账号最后访问时间，非空，默认0
        wuk::f64      m_deleted{0};  // 账号注销时间，非空，默认0

        template <typename T>
        inline std::string to_string(const char *format, const T &v) const noexcept {
            // 出于性能和最小化依赖的考虑，不使用std::stringstream和std::to_string。
            constexpr wuk::i32 uid_str_len = 32;
            char uid_str[uid_str_len] {0};
            snprintf(uid_str, uid_str_len, format, v);
            return std::string(uid_str);
        }

        inline std::string double_to_string(const wuk::f64 &v) const noexcept {
            return this->to_string((v != 0) ? "%.5lf" : "%.0lf", v);
        }

    public: // Setter
        inline UserInfo &set_uid(const wuk::i64 &uid) noexcept {
            this->m_uid = uid; return *this;
        }

        inline UserInfo &set_name(const std::string &name) noexcept {
            this->m_name = name; return *this;
        }

        inline UserInfo &set_salt(const wuk::Buffer &salt) noexcept {
            this->m_salt = salt; return *this;
        }

        inline UserInfo &set_hash(const wuk::Buffer &hash) noexcept {
            this->m_hash = hash; return *this;
        }

        inline UserInfo &set_status(AccountStatus status) noexcept {
            this->m_status = status; return *this;
        }

        inline UserInfo &set_created(const wuk::f64 &time_val) noexcept {
            this->m_created = time_val; return *this;
        }

        inline UserInfo &set_modified(const wuk::f64 &time_val) noexcept {
            this->m_modified = time_val; return *this;
        }

        inline UserInfo &set_accessed(const wuk::f64 &time_val) noexcept {
            this->m_accessed = time_val; return *this;
        }

        inline UserInfo &set_deleted(const wuk::f64 &time_val) noexcept {
            this->m_deleted = time_val; return *this;
        }

    public: // Getter
        inline std::string get_uid_str() const noexcept {
            return this->to_string("%zd", this->m_uid);
        }

        inline std::string get_name_str() const noexcept {
            return this->m_name;
        }

        inline std::string get_salt_str() const noexcept {
            return this->m_salt.to_str();
        }

        inline std::string get_hash_str() const noexcept {
            return this->m_hash.to_str();
        }

        inline std::string get_status_str() const noexcept {
            static constexpr const char *status_names[] = {
                "NORMAL", "BANNED", "MUTED", "DELETED"
            };
            return std::string(status_names[static_cast<wuk::i32>(this->m_status)]);
        }

        inline std::string get_created_str() const noexcept {
            return this->double_to_string(this->m_created);
        }

        inline std::string get_modified_str() const noexcept {
            return this->double_to_string(this->m_modified);
        }

        inline std::string get_accessed_str() const noexcept {
            return this->double_to_string(this->m_accessed);
        }

        inline std::string get_deleted_str() const noexcept {
            return this->double_to_string(this->m_deleted);
        }

        inline std::vector<std::string> get_params() const noexcept {
            std::vector<std::string> result;
            result.reserve(9);
            result.push_back(this->get_uid_str());
            result.push_back(this->get_name_str());
            result.push_back(this->get_salt_str());
            result.push_back(this->get_hash_str());
            result.push_back(this->get_status_str());
            result.push_back(this->get_created_str());
            result.push_back(this->get_modified_str());
            result.push_back(this->get_accessed_str());
            result.push_back(this->get_deleted_str());
            return result;
        }
    };
}

#endif