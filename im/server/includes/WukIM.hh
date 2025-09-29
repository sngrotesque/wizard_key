#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <WukBuffer.hh>
#include <WukTime.hh>

#include <vector>
#include <mutex>

namespace wuk::im::server {
    class LIBWUK_API Snowflake {
    private:
        wuk::i64 m_sequence = 0;
        wuk::i64 m_worker_id = 0;
        wuk::i64 m_datacenter_id = 0;
        wuk::i64 m_last_timestamp = -1;
        std::mutex m_mutex;
        wuk::Time m_time;

    private:
        wuk::i64 current_timestamp() const noexcept;
        wuk::i64 wait_next_millis(wuk::i64 last_timestamp) const noexcept;

    public:
        Snowflake() = default;
        Snowflake(wuk::i64 worker_id, wuk::i64 datacenter_id = 0);

        Snowflake(const Snowflake &other) = default;
        Snowflake(Snowflake &&other) = default;
        Snowflake &operator=(const Snowflake &other) = default;
        Snowflake &operator=(Snowflake &&other) = default;

    public:
        wuk::i64 generate_id();
    };
}

#endif