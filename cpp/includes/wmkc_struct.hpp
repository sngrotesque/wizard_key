#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_STRUCT
#define WMKC_CPP_STRUCT
#include <algorithm>
#include <vector>

// 后续请考虑使用vector容器来作为第一参数之后的参数，针对实际上内存占用不同的情况，
// 无所谓，直接强转就行，也就是说，将第二个参数直接设置为vector<size_t>，然后按照实际的类型在函数内进行转换。
class wmkcStruct {
    private:
        wmkcChar *ptr;
    public:
        std::string pack(std::string format, ...);
        std::vector<wmkcSize> unpack(std::string format, ...);
};

#endif
#endif
