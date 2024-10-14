#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_BUFFER
#define WUK_CPP_BUFFER
#include <config/WukException.hh>
#include <WukMemory.hh>

namespace wuk {
    class LIBWUK_API Buffer {
    private:
        wByte *data;
        wByte *data_offset; // 在当前已申请空间的情况下写入数据时使用

        wSize data_len;     // 代表实际长度
        wSize data_size;    // 代表已申请的内存空间长度

        // 用于增加可用内存大小
        void expand_memory(wSize length);

    public:
        // 默认构造函数
        Buffer();
        // 给予数据的构造函数
        Buffer(wByte *content, wSize length);
        // 申请指定大小内存空间备用的构造函数
        Buffer(wSize memory_size);
        ~Buffer();

        // 在已申请了足够大小的堆空间的情况下，将数据直接写入内存。
        void write(wByte *content, wSize length);
        // 在需要写入指定长度的大小的内容且同时需要指针的情况下调用此方法
        wByte *write(wSize length);

        void append(const wByte *content, wSize length);

        // 属性
        wByte *get_data();
        wSize get_length();
        wSize get_size();
    };
}

#endif
#endif
