#include <test_wmkc_class.h>

WMKC_PRIVATE(wmkc_u32) wmkcClass_add WMKC_OPEN_API
WMKC_OF((wmkc_u32 a, wmkc_u32 b))
{
    return a + b;
}

WMKC_PUBLIC(wmkcClass_obj *) wmkcClass_new WMKC_OPEN_API
WMKC_OF(())
{
    wmkcClass_obj *class = wmkcNull;
    wmkcMem_new(wmkcClass_obj *, class, sizeof(wmkcClass_obj));
    class->add = wmkcClass_add;
    return class;
}
