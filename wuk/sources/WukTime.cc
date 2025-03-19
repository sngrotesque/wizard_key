#include <WukTime.hh>

void wuk::Time::sleep(double delay)
{
#   if defined(WUK_PLATFORM_WINOS)
    Sleep(delay * 1e3);
#   elif defined(WUK_PLATFORM_LINUX)
    usleep(delay * 1e6);
#   endif
}

double wuk::Time::time()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double>(duration).count();
}
