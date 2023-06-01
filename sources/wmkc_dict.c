#include <wmkc_dict.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcDict_read WMKC_OPEN_API
WMKC_OF((wmkcDict_obj *dict, wmkcChar *string, wmkcChar delimiter))
{
    wmkcErr_obj error;
    if(!dict || !string) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcDict_read: dict or string is NULL.");
    }
    dict->delimiter = delimiter;

    wmkcChar *p = strchr(string, delimiter);
    if(p) {
        *p = 0x00;
        dict->key = string;
        dict->value = p + 1;
    } else {
        dict->key = wmkcNull;
        dict->value = wmkcNull;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcDict_new WMKC_OPEN_API
WMKC_OF((wmkcDict_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcDict_read: obj is NULL.");
    }
    if(!wmkcMemoryNew(wmkcDict_obj *, (*obj), sizeof(wmkcDict_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcDict_read: (*obj) failed to apply for memory.");
    }

    (*obj)->key = wmkcNull;
    (*obj)->value = wmkcNull;
    (*obj)->next = wmkcNull;
    (*obj)->delimiter = 0x00;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcDict_free WMKC_OPEN_API
WMKC_OF((wmkcDict_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcDict_read: obj is NULL.");
    }

    wmkcDict_obj *p = wmkcNull;
    while(*obj) {
        p = *obj;
        *obj = (*obj)->next;
        free(p);
    }
    p = wmkcNull;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
