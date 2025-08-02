#include <WukTime.hh>

#ifdef WUK_PLATFORM_WINOS
#include <Windows.h>
#elif defined(WUK_PLATFORM_LINUX)
#include <unistd.h>
#endif

void wuk::WukTime::sleep(double delay)
{
#   if defined(WUK_PLATFORM_WINOS)
    Sleep(delay * 1e3);
#   elif defined(WUK_PLATFORM_LINUX)
    usleep(delay * 1e6);
#   endif
}

