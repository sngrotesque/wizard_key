#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <vector>

namespace wuk {
    class LIBWUK_API BufferEx {
    private:
        std::vector<wuk::byte> m_data; // 数据成员

    public:
        // 默认构造函数
        BufferEx() = default;
        // 析构函数
        ~BufferEx() = default;

    public:
        // 拷贝构造函数（BufferEx）
        BufferEx(const BufferEx &other) = default;
        // 移动构造函数（BufferEx）
        BufferEx(BufferEx &&other) = default;
        // 拷贝赋值运算符（BufferEx）
        BufferEx &operator=(const BufferEx &other) = default;
        // 移动赋值运算符（BufferEx）
        BufferEx &operator=(BufferEx &&other) = default;

    public:
        // 拷贝构造函数（std::string）
        BufferEx(const std::string &other);
        // 移动构造函数（std::string）
        BufferEx(std::string &&other);
        // 拷贝赋值运算符（std::string）
        BufferEx &operator=(const std::string &other);
        // 移动赋值运算符（std::string）
        BufferEx &operator=(std::string &&other);

    public:
        // 申请一个length大小的内存空间，不显式初始化内存空间。
        BufferEx(wuk::ulong length);
        // 申请一个length大小的内存空间，并将元素初始化为ch。
        BufferEx(wuk::ulong length, wuk::byte ch);
        // 数据构造
        BufferEx(const wuk::byte *buffer, wuk::ulong length);

    public:
        BufferEx operator+(const BufferEx &other);
        BufferEx &operator+=(const BufferEx &other);
        bool operator==(const BufferEx &other);
        bool operator!=(const BufferEx &other);

        wuk::byte &operator[](wuk::ulong index);
        const wuk::byte &operator[](wuk::ulong index) const;

        wuk::byte &at(wuk::ulong index);
        const wuk::byte &at(wuk::ulong index) const;

    public:
        bool is_empty() const noexcept;
        void write(const wuk::byte *buffer, wuk::ulong length);
        void write(const std::string &buffer);
        void append(const wuk::byte *buffer, wuk::ulong length);
        void append(const std::string &buffer);
        wuk::byte *append_write(wuk::ulong length) noexcept;
        void shrink_to_fit() noexcept;

    public:
        const wuk::byte *get_data() const noexcept;
        const char *get_cstr() const noexcept;
        wuk::ulong get_length() const noexcept;
        wuk::ulong get_size() const noexcept;

        std::string to_str() const noexcept;
        std::string to_hex() const noexcept;

        void clear(bool secure) noexcept;
    };
}

#endif
