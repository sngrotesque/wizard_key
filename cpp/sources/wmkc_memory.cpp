#include <wmkc_memory.hpp>

wmkcVoid wmkcMem_secure(wmkcVoid *buf, wmkcSize size)
{
#   if defined(WMKC_PLATFORM_WINOS)
    SecureZeroMemory(buf, size);
#   elif defined(WMKC_PLATFORM_LINUX)
    explicit_bzero(buf, size);
#   endif
}
