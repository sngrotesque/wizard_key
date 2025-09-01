#include <WukBufferEx.hh>

#include <utils/bytes.hh>
#include <WukMemory.hh>

namespace wuk {
    BufferEx::BufferEx(wuk::ulong length)
    {
        this->m_data.resize(length);
    }

    BufferEx::BufferEx(wuk::ulong length, wuk::byte ch)
    {
        this->m_data.assign(length, ch);
    }

    BufferEx::BufferEx(const wuk::byte *buffer, wuk::ulong length)
    {
        this->m_data.assign(buffer, buffer + length);
    }

    BufferEx::BufferEx(const std::string &other)
    {
        const wuk::byte *p = reinterpret_cast<const wuk::byte *>(other.data());
        this->m_data.assign(p, p + other.length());
    }

    BufferEx::BufferEx(std::string &&other)
    {
        const wuk::byte *p = reinterpret_cast<const wuk::byte *>(other.data());
        this->m_data.assign(p, p + other.length());
        other.clear();
        other.shrink_to_fit();
    }

    BufferEx &BufferEx::operator=(const std::string &other)
    {
        const wuk::byte *p = reinterpret_cast<const wuk::byte *>(other.data());
        this->m_data.assign(p, p + other.length());
        return *this;
    }

    BufferEx &BufferEx::operator=(std::string &&other)
    {
        const wuk::byte *p = reinterpret_cast<const wuk::byte *>(other.data());
        this->m_data.assign(p, p + other.length());
        other.clear();
        other.shrink_to_fit();
        return *this;
    }

    BufferEx BufferEx::operator+(const BufferEx &other)
    {
        BufferEx result(*this);
        result += other;
        return result;
    }

    BufferEx &BufferEx::operator+=(const BufferEx &other)
    {
        this->m_data.insert(this->m_data.end(), other.m_data.begin(), other.m_data.end());
        return *this;
    }

    bool BufferEx::operator==(const BufferEx &other)
    {
        return this->m_data == other.m_data;
    }

    bool BufferEx::operator!=(const BufferEx &other)
    {
        return !(*this == other);
    }

    wuk::byte &BufferEx::operator[](wuk::ulong index)
    {
        return this->m_data[index];
    }

    const wuk::byte &BufferEx::operator[](wuk::ulong index) const
    {
        return this->m_data[index];
    }

    wuk::byte &BufferEx::at(wuk::ulong index)
    {
        try {
            return this->m_data.at(index);
        } catch (const std::out_of_range &e) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::BufferEx::at",
                e.what());
        }
    }

    const wuk::byte &BufferEx::at(wuk::ulong index) const
    {
        try {
            return this->m_data.at(index);
        } catch (const std::out_of_range &e) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::BufferEx::at",
                e.what());
        }
    }

    bool BufferEx::is_empty() const noexcept
    {
        return this->m_data.empty();
    }

    void BufferEx::write(const wuk::byte *buffer, wuk::ulong length)
    {
        if (!buffer) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::BufferEx::write",
                "buffer is nullptr.");
        }
        this->m_data.assign(buffer, buffer + length);
    }

    void BufferEx::write(const std::string &buffer)
    {
        if (buffer.empty()) {
            return;
        }
        this->write(reinterpret_cast<const wuk::byte *>(buffer.data()), buffer.length());
    }

    void BufferEx::append(const wuk::byte *buffer, wuk::ulong length)
    {
        if (!buffer) {
            throw wuk::Exception(wuk::Error::NPTR, "wuk::BufferEx::append",
                "buffer is nullptr.");
        }
        this->m_data.insert(this->m_data.end(), buffer, buffer + length);
    }

    void BufferEx::append(const std::string &buffer)
    {
        if (buffer.empty()) {
            return;
        }
        this->append(reinterpret_cast<const wuk::byte *>(buffer.data()), buffer.length());
    }

    wuk::byte *BufferEx::append_write(wuk::ulong length) noexcept
    {
        wuk::ulong old_length = this->m_data.size();
        this->m_data.resize(old_length + length);
        return this->m_data.data() + old_length;
    }

    void BufferEx::shrink_to_fit() noexcept
    {
        this->m_data.shrink_to_fit();
    }

    const wuk::byte *BufferEx::get_data() const noexcept
    {
        return this->m_data.data();
    }

    const char *BufferEx::get_cstr() const noexcept
    {
        return reinterpret_cast<const char *>(this->m_data.data());
    }

    wuk::ulong BufferEx::get_length() const noexcept
    {
        return this->m_data.size();
    }

    wuk::ulong BufferEx::get_size() const noexcept
    {
        return this->m_data.capacity();
    }

    std::string BufferEx::to_str() const noexcept
    {
        return std::string(this->get_cstr(), this->get_length());
    }

    std::string BufferEx::to_hex() const noexcept
    {
        if (this->is_empty()) {
            return {};
        }
        std::vector<char> output = wuk::utils::bytes_to_hex(this->m_data);
        return std::string(output.data(), output.size());
    }

    void BufferEx::clear(bool secure) noexcept
    {
        if (secure) {
            wuk::memory_secure(this->m_data.data(), this->m_data.size());
        }
        this->m_data.clear();
        this->m_data.shrink_to_fit();
    }
}
