#include <WukMemory.hh>

void wuk::memory_zero(void *p, wSize n)
{
    memset(p, 0x00, n);
}

void wuk::memory_secure(void *p, wSize n)
{
#   if defined(WUK_PLATFORM_WINOS)
    SecureZeroMemory(p, n);
#   elif defined(WUK_PLATFORM_LINUX)
    explicit_bzero(p, n);
#   endif
}
