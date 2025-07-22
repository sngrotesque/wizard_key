#include <WukTime.hh>

void wuk::Time::sleep(double delay)
{
#   if defined(WUK_PLATFORM_WINOS)
    Sleep(delay * 1e3);
#   elif defined(WUK_PLATFORM_LINUX)
    usleep(delay * 1e6);
#   endif
}

