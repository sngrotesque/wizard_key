#include <server/includes/WukUtils.hh>
#include <WukTime.hh>

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

// 纪元时间(2025-07-01 00:00:00 UTC) 毫秒
static constexpr wuk::i64 epoch = 1751299200000LL;

namespace wuk::im::server {
    wuk::i64 Snowflake::current_timestamp() const noexcept
    {
        return static_cast<wuk::i64>(this->m_time.time<wuk::f64>() * 1000);
    }

    wuk::i64 Snowflake::wait_next_millis(wuk::i64 last_timestamp) const noexcept
    {
        auto timestamp = current_timestamp();
        while (timestamp <= last_timestamp) {
            timestamp = current_timestamp();
        }
        return timestamp;
    }

    Snowflake::Snowflake(wuk::i64 worker_id, wuk::i64 datacenter_id)
    {
        if ((worker_id > max_worker_id) || (worker_id < 0)) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::im::Snowflake::Snowflake",
                "Worker ID is out of range.");
        }
        if ((datacenter_id > max_datacenter_id) || (datacenter_id < 0)) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::im::Snowflake::Snowflake",
                "Datacenter ID is out of range.");
        }

        this->m_datacenter_id = datacenter_id;
        this->m_worker_id = worker_id;
    }

    wuk::i64 Snowflake::generate_id()
    {
        std::lock_guard<std::mutex> lock(this->m_mutex);

        // 获取当前时间戳(毫秒)
        auto timestamp = current_timestamp();

        // 处理时钟回拨
        if (timestamp < this->m_last_timestamp) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::im::Snowflake::generate_id",
                "Clock callback, refusal to generate ID.");
        }

        // 同一毫秒内生成多个ID
        if (timestamp == this->m_last_timestamp) {
            this->m_sequence = (this->m_sequence + 1) & sequence_mask;
            if (this->m_sequence == 0) {
                timestamp = wait_next_millis(this->m_last_timestamp);
            }
        } else {
            this->m_sequence = 0;
        }

        this->m_last_timestamp = timestamp;

        // 组合各部分生成最终ID
        return ((timestamp - epoch) << timestamp_shift) |
            (this->m_datacenter_id << datacenter_id_shift) |
            (this->m_worker_id << worker_id_shift) |
            this->m_sequence;
    }
}
