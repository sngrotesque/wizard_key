#include <network/wmkc_tp.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcTp_init WMKC_OPEN_API
WMKC_OF((wmkcCSTR addr, wmkc_u16 port, wmkc_u32 msxRetry, SNC_mode mode))
{
    wmkcErr_obj error;
    if(!addr || !port) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcTp_init: addr or port is NULL.");
    }


}

