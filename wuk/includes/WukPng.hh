#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <WukMemory.hh>
#include <WukBuffer.hh>

namespace wuk {
    class LIBWUK_API Png {
    public:
        typedef struct {
            wuk::u32 width;
            wuk::u32 height;
        } png_ihdr;
    };
}

#endif
