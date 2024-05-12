#include <memory.hpp>

wVoid wmkc::memory_zero(wVoid *p, wSize n)
{
    memset(p, 0x00, n);
}

wVoid wmkc::memory_secure(wVoid *p, wSize n)
{
#   if defined(WMKC_PLATFORM_WINOS)
    SecureZeroMemory(p, n);
#   elif defined(WMKC_PLATFORM_LINUX)
    explicit_bzero(p, n);
#   endif
}
