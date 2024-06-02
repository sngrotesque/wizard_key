#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_HEXDUMP
#define WMKC_CPP_HEXDUMP
#include <config/exception.hpp>
#include <fstream>

namespace wmkc {
    template <typename T>
    wVoid hexdump(T file_path);
    wVoid hexdump(wByte *data, wSize length);
}

#endif /* WMKC_CPP_BASE */
#endif /* WMKC_SUPPORT */
