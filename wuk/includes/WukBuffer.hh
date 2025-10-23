#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>

namespace wuk {
    class LIBWUK_API Buffer {
    private: // 私有成员
        wuk::byte *m_data   = nullptr;
        wuk::byte *m_offset = nullptr; // 偏移量，应永远等于data + data_len。

        wuk::ulong m_len    = 0; // 代表实际使用长度
        wuk::ulong m_size   = 0; // 代表已申请的内存空间长度

    private:
        void expand_memory(wuk::ulong length); // 用于增加可用内存大小
        void shrink_memory(wuk::ulong length); // 用于减少可用内存大小
        bool is_memory_sufficient(wuk::ulong length) const noexcept; // 检查当前已申请的内存空间是否足够

    public:
        // 构造函数
        Buffer() = default;
        // 析构函数
        ~Buffer();

    public:
        // 拷贝构造函数
        Buffer(const Buffer &other);
        // 移动构造函数
        Buffer(Buffer &&other) noexcept;
        // 拷贝赋值运算符
        Buffer &operator=(const Buffer &other);
        // 移动赋值运算符
        Buffer &operator=(Buffer &&other) noexcept;

    public:
        // 拷贝构造函数（std::string）
        Buffer(const std::string &other);
        // 移动构造函数（std::string）
        Buffer(std::string &&other);
        // 拷贝赋值运算符（std::string）
        Buffer &operator=(const std::string &other);
        // 移动赋值运算符（std::string）
        Buffer &operator=(std::string &&other);

    public:
        // 申请一个length大小的内存空间，不显式初始化内存空间。
        Buffer(wuk::ulong length);
        // 申请一个length大小的内存空间，并将元素初始化为ch。
        Buffer(wuk::ulong length, wuk::byte ch);
        // 数据构造
        Buffer(const wuk::byte *buffer, wuk::ulong length);
        Buffer(const char *buffer);

    public:
        Buffer operator+(const Buffer &other) const noexcept;
        Buffer &operator+=(const Buffer &other) noexcept;

        bool operator==(const Buffer &other) const noexcept;
        bool operator!=(const Buffer &other) const noexcept;

        bool operator==(const std::string &other) const noexcept;
        bool operator!=(const std::string &other) const noexcept;

        wuk::byte &operator[](const wuk::ulong &index) noexcept;
        const wuk::byte &operator[](const wuk::ulong &index) const noexcept;

        wuk::byte &at(const wuk::ulong &index);
        const wuk::byte &at(const wuk::ulong &index) const;

    public:
        // 判断当前容器是否为空。
        bool empty() const noexcept;

        // 覆盖写入数据（如果空间不足以写入的话将自动扩容）
        void write(const wuk::byte *buffer, wuk::ulong length);
        void write(const std::string &buffer);

        // 追加写入数据（如果空间不足以写入的话将自动扩容）
        void append(const wuk::byte *buffer, wuk::ulong length);
        void append(const std::string &buffer);

        template <typename T>
        T *write(wuk::ulong length);

        template <typename T>
        T *append(wuk::ulong length);

        // 插入数据
        void insert(wuk::ulong index, const Buffer &buffer);
        // 删除范围数据
        void erase(wuk::ulong start, wuk::ulong end);
        // 重新调整大小
        void resize(wuk::ulong length);

        // 将内存占用与实际占用保持一致
        void shrink_to_fit();

    public:
        const wuk::byte *data() const noexcept;
        const char *c_str() const noexcept;
        // 获取当前已使用的缓冲区的长度
        wuk::ulong size() const noexcept;
        wuk::ulong length() const noexcept;
        // 获取当前已申请的缓冲区的内存长度
        wuk::ulong capacity() const noexcept;

        std::string to_str() const noexcept;
        std::string to_hex() const noexcept;

    public:
        void clear(bool secure = false) noexcept;
    };
}

#include <template/WukBuffer.hh>

#endif
