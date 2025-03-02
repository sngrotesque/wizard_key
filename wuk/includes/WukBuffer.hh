/**
 * 此模块的开发注意事项：
 * 1. 如果涉及直接的数据传入，如`Buffer::Buffer(wByte *content, wSize length)`这个构造函数，那么
 *    必须要将`data_offset`指针置于`data`之后，偏移长度为传入的数据的长度。
 * 2. 在任何时候，`data_offset`指向地址都不应该比`data`小，但是是否有必要增加一个判断，还有待商榷。
 */
#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukEndianness.hh>
#include <config/WukException.hh>
#include <WukMemory.hh>

namespace wuk {
    class LIBWUK_API Buffer {
    private:
        wByte *data;

        // 在当前已申请空间的情况下写入数据时使用（指向数据末端用于追加写入）
        // 可以简单理解为这个指针指向的位置永远必须是`data + data_len`。
        wByte *data_offset;

        wSize data_len;     // 代表实际使用长度
        wSize data_size;    // 代表已申请的内存空间长度

        // 用于增加可用内存大小
        void expand_memory(wSize length);
        // 用于减少可用内存大小
        void shrink_memory(wSize length);
        // 检查当前已申请的内存空间是否足够
        bool is_memory_sufficient(wSize length);

    public:
        // 构造函数
        Buffer();
        Buffer(const wuk::Buffer &other);
        Buffer(wuk::Buffer &&other) noexcept;
        // 给予数据的构造函数
        Buffer(const wByte *content, wSize length);
        // 申请指定大小内存空间备用的构造函数
        Buffer(wSize memory_size);
        // 兼容std::string
        Buffer(const std::string &content);

        // 析构函数
        ~Buffer();

    public:
        // 拷贝赋值运算符
        wuk::Buffer &operator=(const wuk::Buffer &other);
        // 移动赋值运算符
        wuk::Buffer &operator=(wuk::Buffer &&other) noexcept;

        wuk::Buffer &operator=(const std::string &other_string);
        wuk::Buffer &operator=(std::string &&other_string);

        Buffer operator+(const Buffer &other);
        Buffer &operator+=(const Buffer other);

        bool operator==(const Buffer &other);
        bool operator!=(const Buffer &other);

    public:
        // 判断是否为空
        bool is_empty();
        // 在需要写入指定长度的大小的内容且同时需要指针的情况下调用此方法
        wByte *append_write(wSize length);
        // 直接写入，从指针起始处写入，覆盖原数据，不追加。
        void write(const wByte *content, wSize length);
        void write(std::string other_string);
        // 追加写入，可用于直接追加和已申请空间的情况下
        void append(const wByte *content, wSize length);
        void append(const std::string content);

        // 传入数字并序列化
        // 比如传入2 (int)，得到 00 00 00 02 (Hex)
        template <typename T>
        void append_number(T val);

        // 将占用的内存空间与实际使用的内存空间保持同步（防止无意义的内存占用）
        void shrink_to_fit();

    public:
        const wByte *get_data() const noexcept;
        const char *get_cstr() const noexcept;
        wSize get_length() const noexcept;
        wSize get_size() const noexcept;
    };
}

#endif
