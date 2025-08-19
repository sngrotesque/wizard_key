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
        // 各部分的位数定义
        static constexpr wuk::i64 datacenterIdBits = 5;  // 数据中心ID位数
        static constexpr wuk::i64 workerIdBits     = 5;  // 工作节点ID位数
        static constexpr wuk::i64 sequenceBits     = 12; // 序列号位数

        // 最大值计算
        static constexpr wuk::i64 maxDatacenterId = (1 << datacenterIdBits) - 1;
        static constexpr wuk::i64 maxWorkerId     = (1 << workerIdBits)     - 1;
        static constexpr wuk::i64 sequenceMask    = (1 << sequenceBits)     - 1;

        // 时间戳偏移量
        static constexpr wuk::i64 datacenterIdShift = sequenceBits + workerIdBits;
        static constexpr wuk::i64 workerIdShift     = sequenceBits;
        static constexpr wuk::i64 timestampShift    = sequenceBits + workerIdBits + datacenterIdBits;

        // 纪元时间(2025-07-01 00:00:00 UTC)
        static constexpr wuk::i64 epoch = 1751299200000LL;

        wuk::i64 datacenterId = 0;
        wuk::i64 workerId = 0;
        wuk::i64 sequence = 0;
        wuk::i64 lastTimestamp = -1;
        std::mutex mutex;

    public:
        Snowflake(wuk::i64 workerId, wuk::i64 datacenterId = 0) 
        : datacenterId(datacenterId), workerId(workerId)
        {
            if (workerId > maxWorkerId || workerId < 0) {
                throw wuk::Exception(wuk::Error::ERR, "wuk::im::Snowflake::Snowflake",
                    "Worker ID is out of range.");
            }
            if (datacenterId > maxDatacenterId || datacenterId < 0) {
                throw wuk::Exception(wuk::Error::ERR, "wuk::im::Snowflake::Snowflake",
                    "Datacenter ID is out of range.");
            }
        }

        wuk::i64 generate_id() {
            std::lock_guard<std::mutex> lock(mutex);
            
            // 获取当前时间戳(毫秒)
            auto timestamp = currentTimestamp();
            
            // 处理时钟回拨
            if (timestamp < lastTimestamp) {
                throw wuk::Exception(wuk::Error::ERR, "wuk::im::Snowflake::generate_id",
                    "Clock callback, refusal to generate ID.");
            }

            // 同一毫秒内生成多个ID
            if (timestamp == lastTimestamp) {
                sequence = (sequence + 1) & sequenceMask;
                if (sequence == 0) {
                    timestamp = waitNextMillis(lastTimestamp);
                }
            } else {
                sequence = 0;
            }

            lastTimestamp = timestamp;

            // 组合各部分生成最终ID
            return ((timestamp - epoch) << timestampShift) |
                (datacenterId << datacenterIdShift) |
                (workerId << workerIdShift) |
                sequence;
        }

    private:
        wuk::i64 currentTimestamp() const
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        }

        wuk::i64 waitNextMillis(wuk::i64 lastTimestamp)
        {
            auto timestamp = currentTimestamp();
            while (timestamp <= lastTimestamp) {
                timestamp = currentTimestamp();
            }
            return timestamp;
        }
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