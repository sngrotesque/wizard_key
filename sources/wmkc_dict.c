#include <wmkc_dict.h>

WMKC_PRIVATE(wmkcChar *) wmkcDict_trim
WMKC_OF((wmkcChar *src, wmkcChar symbol))
{
    wmkcChar *end = wmkcNull;

    while (*src == symbol) {
        src++;
    }

    end = src + strlen(src) - 1;
    while ((end > src) && (*end == symbol)) {
        end--;
    }

    *(end + 1) = 0x00;
    return src;
}

/**
 * @brief 此函数需要优化一下，以免发生内存泄露
 * @authors SN-Grotesque
 * @note 无
 * @param 无
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcDict_read WMKC_OPEN_API
WMKC_OF((wmkcDict_obj *dict, wmkcChar *string, wmkcChar delimiter))
{
    wmkcErr_obj error;
    if(!dict || !string) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcDict_read: dict or string is NULL.");
    }
    dict->delimiter  = delimiter;
    char *_key_tmp   = wmkcNull;
    char *_value_tmp = wmkcNull;

    wmkcChar *stringCopy = strdup(string);
    wmkcChar *p = strchr(stringCopy, delimiter);
    if(!p) {
        dict->key = dict->value = wmkcNull;
        goto done;
    }
    *p = 0x00;
    _key_tmp = strdup(stringCopy);
    _value_tmp = strdup(p + 1);

    dict->key   = wmkcDict_trim(_key_tmp,   ' ');
    dict->value = wmkcDict_trim(_value_tmp, ' ');

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
