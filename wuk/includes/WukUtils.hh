#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>
#include <WukRandom.hh>

namespace wuk {
    class Utils {
    public:
        void secure_delete_file(const std::string &path, wU32 passes = 3);
    };
}

#endif
