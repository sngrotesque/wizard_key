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
        // wByte *data_add; // 在当前已申请空间的情况下写入数据时使用
        wSize size;

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
        /*
        * 得做一个方法让类似于recv那样的函数可以将buffer内部的数据传入并修改后生效，且同时长度也
        * 必须是写入后的长度。
        * 例如：
        * 
        * recv(sockfd, buffer.method(), length, flag);
        */
        // 这个方法似乎符合要求，只需要在使用时记得传入数据的大小就可以了。
        wByte *write(wSize length);

        void append(const wByte *content, wSize length);
        void append(std::string content);

        wByte *get_data();
        wSize get_size();
    };
}

#endif
#endif
