#include <wmkc_dict.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcDict_read WMKC_OPEN_API
WMKC_OF((wmkcDict_obj *obj, wmkcChar *string, wmkcChar delimiter))
{
    wmkcErr_obj error;
    if(!obj || !string) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcDict_read: obj or string is NULL.");
    }
    wmkcChar *_key = wmkcNull;
    wmkcChar *_val = wmkcNull;
    wmkcChar *swap = wmkcNull;
    wmkcSize index = 0;
    wmkcSize size;

    wmkc_trim(&swap, string, delimiter);
    size = strlen(swap);

    for(wmkcSize tmp = 0; tmp < size; ++tmp) {
        if(*(swap + tmp) == delimiter) {
            index = tmp;
            break;
        }
    }
    if(!index)
        goto done;

    *(swap + index) = 0;
    _key = strdup(swap);
    _val = strdup(swap + index + 1);

    wmkc_trim(&obj->key,   _key, ' ');
    wmkc_trim(&obj->value, _val, ' ');
    obj->delimiter = delimiter;

    wmkcMem_free(swap);
    wmkcMem_free(_key);
    wmkcMem_free(_val);

done:
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcDict_new WMKC_OPEN_API
WMKC_OF((wmkcDict_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcDict_read: obj is NULL.");
    }
    if(!wmkcMem_new(wmkcDict_obj *, (*obj), sizeof(wmkcDict_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcDict_read: Failed to allocate memory for (*obj).");
    }

    (*obj)->key = wmkcNull;
    (*obj)->value = wmkcNull;
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

    if((*obj)->key) {
        wmkcMem_free((*obj)->key);
    }
    if((*obj)->value) {
        wmkcMem_free((*obj)->value);
    }
    wmkcMem_free((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
