#include <memory.hpp>

void wmkc::memory_zero(void *p, wSize n)
{
    memset(p, 0x00, n);
}

void wmkc::memory_secure(void *p, wSize n)
{
#   if defined(WMKC_PLATFORM_WINOS)
    SecureZeroMemory(p, n);
#   elif defined(WMKC_PLATFORM_LINUX)
    explicit_bzero(p, n);
#   endif
}
