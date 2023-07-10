#include <wmkc_conf.h>
#include <wmkc_memory.h>

typedef struct {
    WMKC_PUBLIC(wmkc_u32) (*add)(wmkc_u32 a, wmkc_u32 b);
} wmkcClass_obj;

WMKC_PUBLIC(wmkcClass_obj *) wmkcClass_new WMKC_OPEN_API
WMKC_OF(());
