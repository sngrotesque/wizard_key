#include <wmkc_memory.h>

WMKC_PUBLIC(wmkcVoid) wmkcMem_secure WMKC_OPEN_API
WMKC_OF((wmkcVoid *buf, wmkcSize size))
{
    wmkcSize c = size;
    if(!(size & 3)) {
        wmkc_u32 *p = (wmkc_u32 *)buf;
        while(c) {
            *p++ = 0U;
            c -= 4;
        }
    } else {
        wmkcByte *p = (wmkcByte *)buf;
        while(c) {
            *p++ = 0;
            --c;
        }
    }
}
