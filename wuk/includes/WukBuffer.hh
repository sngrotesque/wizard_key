#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_BUFFER_CPP
#include <config/WukException.hh>
#include <WukMemory.hh>

namespace wuk {
    class Buffer {
    public:
        wByte *data;
        w_size length;

        Buffer(wByte *data, w_size length);
        void append(Buffer buffer);
    };
}

#endif
#endif
