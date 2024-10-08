#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_BUFFER
#define WUK_CPP_BUFFER
#include <config/WukException.hh>
#include <WukMemory.hh>

namespace  wuk {
    class LIBWUK_API Buffer {
    public:
        wByte *data;
        wSize size;

        Buffer();
        Buffer(wByte *content, wSize length);

        ~Buffer();

        void append(const wByte *content, wSize length);
        void append(std::string content);
    };
}

#endif
#endif
