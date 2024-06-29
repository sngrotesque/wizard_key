#include <config/wuk.hpp>

/**
 * 对于在使用浮点数的情况下的pack函数出现问题的解决方法：
 *    ！！！你如果是作为字面量传入的话，不要使用直接的整数！！！
 *    ！！！你如果是作为字面量传入的话，不要使用直接的整数！！！
 *    ！！！你如果是作为字面量传入的话，不要使用直接的整数！！！
 *    比如从```pack("df", 0, 1)```改为```pack("df", 0.0, 1.0)```或者```pack("df", 0.0e0, 1.0e0)```。
*/
#if WUK_SUPPORT
#ifndef WUK_CPP_STRUCT
#define WUK_CPP_STRUCT
#include <config/exception.hpp>

#include <any>
#include <vector>
#include <cstdarg>

namespace wuk {
    enum class endianness {
        NO,
        LE,
        BE};

    class LIBWUK_API Struct {
    public:
        Struct();
        std::string pack(std::string format);
        // std::string pack(std::string format, ...);
        std::vector<std::any> unpack(std::string format, const wByte *buffer, wSize length);
        std::vector<std::any> unpack(std::string format, std::string buffer);
    };
}

#endif
#endif
