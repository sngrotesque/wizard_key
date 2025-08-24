#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <chrono>
#include <thread>

namespace wuk {
    class LIBWUK_API Time {
    public:
        Time() = default;

    public:
        template <typename T>
        inline void sleep(T delay)
        {
            std::this_thread::sleep_for(std::chrono::duration<T>(delay));
        }

        template <typename T>
        inline T time()
        {
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            return std::chrono::duration<T>(duration).count();
        }
    };
}

#endif /* WUK_SUPPORT */
