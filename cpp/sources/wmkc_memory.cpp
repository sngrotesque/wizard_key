#include <wmkc_memory.hpp>

WMKC_PUBLIC(wmkcVoid) wmkcMem_secure WMKC_OPEN_API
WMKC_OF((wmkcVoid *buf, wmkcSize size))
{
#   if defined(WMKC_PLATFORM_WINOS)
    SecureZeroMemory(buf, size);
#   elif defined(WMKC_PLATFORM_LINUX)
    explicit_bzero(buf, size);
#   endif
}
