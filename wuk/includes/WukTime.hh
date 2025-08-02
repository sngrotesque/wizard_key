#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>
#include <chrono>

namespace wuk {
    class LIBWUK_API WukTime {
    public:
        WukTime() = default;
    
    public:
        void sleep(double delay);
        
        inline double time()
        {
            auto now = std::chrono::high_resolution_clock::now();
            auto duration = now.time_since_epoch();
            return std::chrono::duration<double>(duration).count();
        }
    };
}

#endif /* WUK_SUPPORT */
