/**
 * 此模块的开发注意事项：
 * 1. 如果涉及直接的数据传入，如`Buffer::Buffer(wByte *content, wSize length)`这个构造函数，那么
 *    必须要将`data_offset`指针置于`data`之后，偏移长度为传入的数据的长度。
 * 2. 在任何时候，`data_offset`指向地址都不应该比`data`小，但是是否有必要增加一个判断，还有待商榷。
 */

#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_BUFFER
#define WUK_CPP_BUFFER
#include <config/WukException.hh>
#include <WukMemory.hh>

// 后续考虑是否将`wByte *data`改为`std::vector<wByte>`。
namespace wuk {
    class LIBWUK_API Buffer {
    private:
        wByte *data;
        wByte *data_offset; // 在当前已申请空间的情况下写入数据时使用

        wSize data_len;     // 代表实际长度
        wSize data_size;    // 代表已申请的内存空间长度

        // 用于增加可用内存大小
        void expand_memory(wSize length);
        // 检查当前已申请的内存空间是否足够
        bool is_memory_sufficient(wSize length);

    public:
        // 默认构造函数
        Buffer();
        // 给予数据的构造函数
        Buffer(const wByte *content, wSize length);
        // 兼容std::string
        Buffer(const std::string content);
        // 申请指定大小内存空间备用的构造函数
        Buffer(wSize memory_size);
        ~Buffer();

        // 在需要写入指定长度的大小的内容且同时需要指针的情况下调用此方法
        wByte *append_write(wSize length);

        // 追加写入，可用于直接追加和已申请空间的情况下
        void append(const wByte *content, wSize length);
        void append(const std::string content);

        // 这个方法不应该被实现，留在这是为了提醒以后使用运算符重载将其实现
        // void append(const wuk::Buffer buffer);
        // 尝试性实现了+和+=，出现了一个奇怪的BUG，打算后续重新实现了

        // 属性
        wByte *get_data() const;
        const char *get_cStr() const;
        wSize get_length() const;
        wSize get_size() const;
    };
}

#endif
#endif
