#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_HEXDUMP
#define WMKC_CPP_HEXDUMP
#include <config/exception.hpp>
#include <fstream>

namespace wmkc {
    template <typename T>
    void hexdump(T file_path);
    void hexdump(wByte *data, wSize length);
}

#endif /* WMKC_CPP_BASE */
#endif /* WMKC_SUPPORT */
