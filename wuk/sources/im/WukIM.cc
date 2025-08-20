#include <im/WukIM.hh>

#include <iomanip>

#define RETURN return *this

////////////////////////////////////////////////////////////////////

// 各部分的位数定义
static constexpr wuk::i64 datacenter_id_bits = 5;  // 数据中心ID位数
static constexpr wuk::i64 worker_id_bits     = 5;  // 工作节点ID位数
static constexpr wuk::i64 sequence_bits      = 12; // 序列号位数

// 最大值计算
static constexpr wuk::i64 max_datacenter_id = (1 << datacenter_id_bits) - 1;
static constexpr wuk::i64 max_worker_id     = (1 << worker_id_bits)     - 1;
static constexpr wuk::i64 sequence_mask     = (1 << sequence_bits)     - 1;

// 时间戳偏移量
static constexpr wuk::i64 datacenter_id_shift = sequence_bits + worker_id_bits;
static constexpr wuk::i64 worker_id_shift     = sequence_bits;
static constexpr wuk::i64 timestamp_shift     = sequence_bits + worker_id_bits + datacenter_id_bits;

// 纪元时间(2025-07-01 00:00:00 UTC)
static constexpr wuk::i64 epoch = 1751299200000LL;

namespace wuk::im {
    wuk::i64 Snowflake::current_timestamp() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }

    wuk::i64 Snowflake::wait_next_millis(wuk::i64 last_timestamp)
    {
        auto timestamp = current_timestamp();
        while (timestamp <= last_timestamp) {
            timestamp = current_timestamp();
        }
        return timestamp;
    }

    Snowflake::Snowflake(wuk::i64 worker_id, wuk::i64 datacenter_id)
    {
        if (worker_id > max_worker_id || worker_id < 0) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::im::Snowflake::Snowflake",
                "Worker ID is out of range.");
        }
        if (datacenter_id > max_datacenter_id || datacenter_id < 0) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::im::Snowflake::Snowflake",
                "Datacenter ID is out of range.");
        }

        this->datacenter_id = datacenter_id;
        this->worker_id = worker_id;
    }

    wuk::i64 Snowflake::generate_id()
    {
        std::lock_guard<std::mutex> lock(mutex);

        // 获取当前时间戳(毫秒)
        auto timestamp = current_timestamp();

        // 处理时钟回拨
        if (timestamp < last_timestamp) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::im::Snowflake::generate_id",
                "Clock callback, refusal to generate ID.");
        }

        // 同一毫秒内生成多个ID
        if (timestamp == last_timestamp) {
            sequence = (sequence + 1) & sequence_mask;
            if (sequence == 0) {
                timestamp = wait_next_millis(last_timestamp);
            }
        } else {
            sequence = 0;
        }

        last_timestamp = timestamp;

        // 组合各部分生成最终ID
        return ((timestamp - epoch) << timestamp_shift) |
            (datacenter_id << datacenter_id_shift) |
            (worker_id << worker_id_shift) |
            sequence;
    }

///////////////////////////////////////////////////////////////////

    UserInfo::UserInfo( wuk::i64    uid,
                        std::string name,
                        wuk::Buffer salt,
                        wuk::Buffer hash,
                        wuk::f64    cadt,
                        bool        active)
    {
        this->uid    = uid;
        this->name   = name;
        this->salt   = salt;
        this->hash   = hash;
        this->cadt   = cadt;
        this->active = active;
    }

    UserInfo &UserInfo::set_uid(const wuk::i64 &uid)
    {
        this->uid = uid; RETURN;
    }

    UserInfo &UserInfo::set_name(const std::string &name)
    {
        this->name = name; RETURN;
    }

    UserInfo &UserInfo::set_salt(const wuk::Buffer &salt)
    {
        this->salt = salt; RETURN;
    }

    UserInfo &UserInfo::set_hash(const wuk::Buffer &hash)
    {
        this->hash = hash; RETURN;
    }

    UserInfo &UserInfo::set_cadt(const wuk::f64 &timestamp)
    {
        this->cadt = timestamp; RETURN;
    }

    UserInfo &UserInfo::set_active(bool status)
    {
        this->active = status; RETURN;
    }

    wuk::i64 UserInfo::get_uid() const noexcept
    {
        return this->uid;
    }

    std::string UserInfo::get_name() const noexcept
    {
        return this->name;
    }

    wuk::Buffer UserInfo::get_salt() const noexcept
    {
        return this->salt;
    }

    wuk::Buffer UserInfo::get_hash() const noexcept
    {
        return this->hash;
    }

    wuk::f64 UserInfo::get_cadt() const noexcept
    {
        return this->cadt;
    }

    bool UserInfo::get_active() const noexcept
    {
        return this->active;
    }

    std::string UserInfo::get_uid_str() const noexcept
    {
        return std::to_string(this->uid);
    }

    std::string UserInfo::get_name_str() const noexcept
    {
        return this->name;
    }

    std::string UserInfo::get_salt_str() const noexcept
    {
        return this->salt.hex();
    }

    std::string UserInfo::get_hash_str() const noexcept
    {
        return this->hash.hex();
    }

    std::string UserInfo::get_cadt_str() const noexcept
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(5) << this->cadt;
        return ss.str();
    }

    std::string UserInfo::get_active_str() const noexcept
    {
        return this->active ? "TRUE" : "FALSE";
    }
}
