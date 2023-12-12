#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_STRUCT
#define WMKC_CPP_STRUCT
#include <vector>

class wmkcStruct {
    private:
        wmkcChar *ptr;
    public:
        // std::vector<std::string> pack(std::string format, ...);
        // std::vector<wmkcSize> unpack(std::string format, ...);
        std::string pack(std::string format, ...);
        wmkcSize unpack(std::string format, ...);
};

#endif
#endif
