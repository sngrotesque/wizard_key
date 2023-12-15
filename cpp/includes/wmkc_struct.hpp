#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_STRUCT
#define WMKC_CPP_STRUCT
#include <algorithm>
#include <vector>

class wmkcStruct {
    private:
        wmkcChar *ptr;
    public:
        std::string pack(std::string format, std::vector<wmkcSize> args);
        std::vector<wmkcSize> unpack(std::string format, std::string args);
};

#endif
#endif
