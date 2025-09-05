#include <WukBuffer.hh>

#include <utils/bytes.hh>
#include <WukMemory.hh>
#include <vector>

namespace wuk {
    /**
     * @brief 用于增加可用内存大小
     * @authors SN-Grotesque
     * @note 不需要在函数外部重新分配this->data_size大小，此方法会自动完成
     * @param length 需要增加的长度（非总长度），比如要加16字节，就传入16。
     * @return 无
     */
    void Buffer::expand_memory(wuk::ulong length)
    {
        wuk::ulong offset_val = (this->m_offset) ? (this->m_offset - this->m_data) : (0);

        this->m_size = this->m_len + length;

        wuk::byte *tmp_ptr = wuk::m_realloc<wuk::byte *>(this->m_data, this->m_size);
        if (tmp_ptr == nullptr) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::expand_memory",
                "Expanding memory size failed.");
        }
        this->m_data = tmp_ptr;
        this->m_offset = this->m_data + offset_val;
    }

    /**
     * @brief 用于减少可用内存大小
     * @authors SN-Grotesque
     * @note 不需要在函数外部重新分配this->data_size大小，此方法会自动完成
     * @param length 需要减少的长度（非总长度），比如要减16字节，就传入16。
     * @return 无
     */
    void Buffer::shrink_memory(wuk::ulong length)
    {
        if (!this->m_data) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::Buffer::shrink_memory",
                "Attempt to shrink the memory space of an nullptr.");
        }
        if (this->m_size <= length) {
            this->clear();
            return;
        }

        wuk::ulong offset_val = this->m_offset - this->m_data;
        wuk::byte *tmp_ptr = wuk::m_realloc<wuk::byte *>(this->m_data, this->m_size - length);
        if (!tmp_ptr) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::shrink_memory",
                "shrink memory size failed.");
        }
        this->m_data = tmp_ptr;
        this->m_offset = this->m_data + offset_val;

        this->m_size -= length;
    }

    /**
     * @brief 检查当前已申请的内存空间是否足够
     * @authors SN-Grotesque
     * @note 无
     * @param length 新数据的长度
     * @return 如果足够就返回True，否则False
     */
    bool Buffer::is_memory_sufficient(wuk::ulong length)
    {
        return (this->m_len + length) <= this->m_size;
    }

    //////////////////////////////////////////////////////////////////////

    Buffer::~Buffer()
    {
        wuk::m_free(this->m_data);
    }

    //////////////////////////////////////////////////////////////////////

    Buffer::Buffer(const Buffer &other)
    {
        this->m_len = other.m_len;
        this->m_size = other.m_size;

        this->m_data = wuk::m_alloc<wuk::byte *>(this->m_size);
        if (!this->m_data) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::Buffer",
                "Failed to allocate memory for this->m_data.");
        }
        memcpy(this->m_data, other.m_data, other.m_len);

        this->m_offset = this->m_data + (other.m_offset - other.m_data);
    }

    Buffer::Buffer(Buffer &&other) noexcept
    {
        this->m_data = other.m_data;
        this->m_offset = other.m_offset;
        this->m_len = other.m_len;
        this->m_size = other.m_size;

        other.m_data = nullptr;
        other.m_offset = nullptr;
        other.m_len = 0;
        other.m_size = 0;
    }

    Buffer &Buffer::operator=(const Buffer &other)
    {
        if(this == &other) {
            return *this;
        }
        wuk::m_free(this->m_data);
        this->m_len = other.m_len;
        this->m_size = other.m_size;

        this->m_data = wuk::m_alloc<wuk::byte *>(this->m_size);
        if (!this->m_data) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::operator=",
                "Failed to allocate memory for this->m_data.");
        }
        memcpy(this->m_data, other.m_data, other.m_len);
        this->m_offset = this->m_data + (other.m_offset - other.m_data);

        return *this;
    }

    Buffer &Buffer::operator=(Buffer &&other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        wuk::m_free(this->m_data);
        this->m_data = other.m_data;
        this->m_offset = other.m_offset;
        this->m_len = other.m_len;
        this->m_size = other.m_size;

        other.m_data = nullptr;
        other.m_offset = nullptr;
        other.m_len = 0;
        other.m_size = 0;

        return *this;
    }

    //////////////////////////////////////////////////////////////////////

    Buffer::Buffer(const std::string &buffer)
    {
        this->m_len = buffer.length();
        this->m_size = buffer.capacity();

        this->m_data = wuk::m_alloc<wuk::byte *>(this->m_size);
        if (!this->m_data) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::Buffer",
                "Failed to allocate memory for this->m_data.");
        }
        memcpy(this->m_data, buffer.data(), buffer.length());
        this->m_offset = this->m_data + buffer.length();
    }

    Buffer::Buffer(std::string &&buffer)
    {
        this->m_len = buffer.length();
        this->m_size = buffer.capacity();

        this->m_data = wuk::m_alloc<wuk::byte *>(this->m_size);
        if (!this->m_data) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::Buffer",
                "Failed to allocate memory for this->m_data.");
        }
        memcpy(this->m_data, buffer.data(), buffer.length());
        this->m_offset = this->m_data + buffer.length();

        buffer.clear();
        buffer.shrink_to_fit();
    }

    Buffer &Buffer::operator=(const std::string &other)
    {
        wuk::m_free(this->m_data);

        this->m_len = other.length();
        this->m_size = other.size();

        this->m_data = wuk::m_alloc<wuk::byte *>(this->m_size);
        if (!this->m_data) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::operator=",
                "Failed to allocate memory for this->m_data.");
        }
        memcpy(this->m_data, other.data(), other.length());
        this->m_offset = this->m_data + other.length();

        return *this;
    }

    Buffer &Buffer::operator=(std::string &&other)
    {
        wuk::m_free(this->m_data);

        this->m_len = other.length();
        this->m_size = other.size();

        this->m_data = wuk::m_alloc<wuk::byte *>(this->m_size);
        if (!this->m_data) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::operator=",
                "Failed to allocate memory for this->m_data.");
        }
        memcpy(this->m_data, other.data(), other.length());
        this->m_offset = this->m_data + other.length();

        other.clear();
        other.shrink_to_fit();
        return *this;
    }

    //////////////////////////////////////////////////////////////////////

    Buffer::Buffer(wuk::ulong length)
    {
        this->m_size = length;

        this->m_data = wuk::m_alloc<wuk::byte *>(this->m_size);
        if (!this->m_data) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::Buffer",
                "Failed to allocate memory for this->m_data.");
        }

        this->m_offset = this->m_data;
    }

    Buffer::Buffer(wuk::ulong length, wuk::byte ch)
    {
        this->m_size = length;

        this->m_data = wuk::m_alloc<wuk::byte *>(this->m_size);
        if (!this->m_data) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::Buffer",
                "Failed to allocate memory for this->m_data.");
        }
        memset(this->m_data, ch, length);

        this->m_offset = this->m_data;
    }

    Buffer::Buffer(const wuk::byte *buffer, wuk::ulong length)
    {
        if (!buffer) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::Buffer::Buffer",
                "buffer is nullptr.");
        }
        this->m_len = length;
        this->m_size = length;

        this->m_data = wuk::m_alloc<wuk::byte *>(this->m_len);
        if (!this->m_data) {
            throw wuk::Exception(wuk::Error::MEMORY, "wuk::Buffer::Buffer",
                "Failed to allocate memory for this->m_data.");
        }
        memcpy(this->m_data, buffer, length);

        this->m_offset = this->m_data + length;
    }

    //////////////////////////////////////////////////////////////////////

    Buffer Buffer::operator+(const Buffer &other)
    {
        Buffer result{*this};

        if (!result.is_memory_sufficient(other.m_len)) {
            result.expand_memory(other.m_len);
        }

        memcpy(result.m_data + result.m_len, other.m_data, other.m_len);

        result.m_offset = result.m_data + result.m_len + other.m_len;
        result.m_len += other.m_len;

        return result;
    }

    Buffer &Buffer::operator+=(const Buffer &other)
    {
        if (!this->is_memory_sufficient(other.m_len)) {
            this->expand_memory(other.m_len);
        }

        memcpy(this->m_data + this->m_len, other.m_data, other.m_len);

        this->m_offset = this->m_data + this->m_len + other.m_len;
        this->m_len += other.m_len;

        return *this;
    }

    bool Buffer::operator==(const Buffer &other)
    {
        if (this == &other) {
            return true;
        }
        if (this->m_len != other.m_len) {
            return false;
        }

        for (wuk::ulong i = 0; i < this->m_len; ++i) {
            if (this->m_data[i] != other.m_data[i]) {
                return false;
            }
        }

        return true;
    }

    bool Buffer::operator!=(const Buffer &other)
    {
        if (this == &other) {
            return false;
        }

        return !(*this == other);
    }

    wuk::byte &Buffer::operator[](const wuk::ulong &index) noexcept
    {
        return this->m_data[index];
    }

    const wuk::byte &Buffer::operator[](const wuk::ulong &index) const
    {
        return this->m_data[index];
    }

    wuk::byte &Buffer::at(const wuk::ulong &index)
    {
        if (index > this->m_size) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::Buffer::at",
                "Index out of range.");
        }
        return this->m_data[index];
    }

    const wuk::byte &Buffer::at(const wuk::ulong &index) const
    {
        if (index > this->m_size) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::Buffer::at",
                "Index out of range.");
        }
        return this->m_data[index];
    }

    //////////////////////////////////////////////////////////////////////

    bool Buffer::is_empty() const noexcept
    {
        return ((!this->m_data) || (!this->m_len) || (!this->m_size));
    }

    void Buffer::write(const wuk::byte *buffer, wuk::ulong length)
    {
        if (!buffer) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::Buffer::write",
                "buffer in nullptr.");
        }
        if (!this->is_memory_sufficient(length)) {
            this->expand_memory(length);
        }

        memcpy(this->m_data, buffer, length);

        this->m_offset = this->m_data + length;
        this->m_len = length;
    }

    void Buffer::write(const std::string &buffer)
    {
        if (buffer.empty()) {
            return;
        }
        const wuk::byte *p = reinterpret_cast<const wuk::byte *>(buffer.data());
        this->write(p, buffer.size());
    }

    wuk::byte *Buffer::write(wuk::ulong length)
    {
        if (!this->is_memory_sufficient(length)) {
            this->expand_memory(length);
        }
        this->m_len = length;
        this->m_offset = this->m_data + length;
        return this->m_data;
    }

    void Buffer::append(const wuk::byte *buffer, wuk::ulong length)
    {
        if (!buffer) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::Buffer::append",
                "buffer in nullptr.");
        }
        if (!this->is_memory_sufficient(length)) {
            this->expand_memory(length);
        }

        memcpy(this->m_offset, buffer, length);

        this->m_offset += length;
        this->m_len += length;
    }

    void Buffer::append(const std::string &buffer)
    {
        if (buffer.empty()) {
            return;
        }
        this->append(reinterpret_cast<const wuk::byte *>(buffer.c_str()),
                    buffer.size());
    }

    wuk::byte *Buffer::append(wuk::ulong length)
    {
        if (!this->is_memory_sufficient(length)) {
            this->expand_memory(length);
        }

        this->m_len += length;
        this->m_offset += length;

        return this->m_offset - length;
    }

    void Buffer::shrink_to_fit()
    {
        if (this->m_len == this->m_size) {
            return;
        }
        if (this->m_len == 0) {
            this->clear();
            return;
        }

        this->shrink_memory(this->m_size - this->m_len);
    }

    //////////////////////////////////////////////////////////////////////

    const wuk::byte *Buffer::get_data() const noexcept
    {
        return this->m_data;
    }

    const char *Buffer::get_cstr() const noexcept
    {
        return reinterpret_cast<const char *>(this->m_data);
    }

    wuk::ulong Buffer::get_length() const noexcept
    {
        return this->m_len;
    }

    wuk::ulong Buffer::get_size() const noexcept
    {
        return this->m_size;
    }

    std::string Buffer::to_str() const noexcept
    {
        return std::string(this->get_cstr(), this->get_length());
    }

    std::string Buffer::to_hex() const noexcept
    {
        if (this->is_empty()) {
            return {};
        }
        std::vector<wuk::byte> input(this->m_data, this->m_offset);
        std::vector<char> output = wuk::utils::bytes_to_hex(input);

        return std::string(output.data(), output.size());
    }

    void Buffer::clear(bool secure) noexcept
    {
        if (!this->m_data) {
            return;
        }
        void (*mem_zero)(void *, wuk::ulong) = \
            (!secure) ? wuk::memory_zero : wuk::memory_secure;

        mem_zero(this->m_data, this->m_size);

        wuk::m_free(this->m_data);
        this->m_data = nullptr;
        this->m_offset = nullptr;
        this->m_len = 0;
        this->m_size = 0;
    }
}
