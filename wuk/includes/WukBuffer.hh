/**
 * 此模块的开发注意事项：
 * 1. 如果涉及直接的数据传入，如`Buffer::Buffer(wuk::byte *content, wuk::ulong length)`这个构造函数，那么
 *    必须要将`data_offset`指针置于`data`之后，偏移长度为传入的数据的长度。
 * 2. 在任何时候，`data_offset`指向地址都不应该比`data`小，但是是否有必要增加一个判断，还有待商榷。
 */
#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukEndianness.hh>
#include <core/WukException.hh>

namespace wuk {
    class LIBWUK_API Buffer {
    private: // 私有成员
        wuk::byte *data        = nullptr;
        wuk::byte *data_offset = nullptr; // 偏移量，应永远等于data + data_len。

        wuk::ulong data_len    = 0; // 代表实际使用长度
        wuk::ulong data_size   = 0; // 代表已申请的内存空间长度

    private: // 私有方法
        // 用于增加可用内存大小
        void expand_memory(wuk::ulong length);
        // 用于减少可用内存大小
        void shrink_memory(wuk::ulong length);
        // 检查当前已申请的内存空间是否足够
        bool is_memory_sufficient(wuk::ulong length);

    public: // 构造函数
        // 构造函数
        Buffer() = default;
        // 拷贝构造函数
        Buffer(const wuk::Buffer &other);
        // 移动构造函数
        Buffer(wuk::Buffer &&other) noexcept;
        // 给予数据的构造函数
        Buffer(const wuk::byte *content, wuk::ulong length);
        // 申请指定大小内存空间备用的构造函数
        explicit Buffer(wuk::ulong memory_size);
        // 兼容std::string
        Buffer(const std::string &content);

        // 析构函数
        ~Buffer();

    public: // 运算符重载
        // 拷贝赋值运算符
        wuk::Buffer &operator=(const wuk::Buffer &other);
        // 移动赋值运算符
        wuk::Buffer &operator=(wuk::Buffer &&other) noexcept;

        wuk::Buffer &operator=(const std::string &other_string);
        wuk::Buffer &operator=(std::string &&other_string);

        Buffer operator+(const Buffer &other);
        Buffer &operator+=(const Buffer &other);

        bool operator==(const Buffer &other);
        bool operator!=(const Buffer &other);

        wuk::byte &operator[](const wuk::ulong &index);
        const wuk::byte &operator[](const wuk::ulong &index) const;

    public: // 公共方法
        // 判断是否为空
        bool is_empty() const noexcept;
        // 在需要写入指定长度的大小的内容且同时需要指针的情况下调用此方法
        wuk::byte *append_write(wuk::ulong length);
        // 直接写入，从指针起始处写入，覆盖原数据，不追加。
        void write(const wuk::byte *content, wuk::ulong length);
        void write(std::string other_string);
        // 追加写入，可用于直接追加和已申请空间的情况下
        void append(const wuk::byte *content, wuk::ulong length);
        void append(const std::string content);

        // 传入数字并序列化
        // 比如传入2 (uint32_t)，得到 00 00 00 02 (Hex)
        template <typename T>
        inline void append_number(T val)
        {
            if constexpr (!std::is_integral_v<T> && !std::is_floating_point_v<T>) {
                throw wuk::Exception(wuk::Error::ERR, "void wuk::Buffer::append_number",
                    "The parameter must be a number.");
            }
            wuk::byte buffer[sizeof(T)];
            memcpy(buffer, &val, sizeof(T));
#           ifdef WUK_NATIVE_LE
            wuk::reversal_array(buffer, sizeof(T));
#           endif
            this->append(buffer, sizeof(T));
        }

        // 将占用的内存空间与实际使用的内存空间保持同步（防止无意义的内存占用）
        void shrink_to_fit();

    public: // 取值方法
        const wuk::byte *get_data() const noexcept;
        const char *get_cstr() const noexcept;
        wuk::ulong get_length() const noexcept;
        wuk::ulong get_size() const noexcept;

        // 转为std::string类型
        std::string to_str() const noexcept;
        // 转为十六进制字符串
        std::string hex() const noexcept;
        // 取下标（异常处理版）
        wuk::byte &at(const wuk::ulong &index);

    public: // 内存安全相关
        void clear(bool secure = false) noexcept;
    };
}

#endif
