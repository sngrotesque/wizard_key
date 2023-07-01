#include <wmkc_common.h>

WMKC_PUBLIC(wmkcErr_obj) wmkc_reverse WMKC_OPEN_API
WMKC_OF((wmkcVoid *buf, wmkcSize size, wmkc_u32 elementSize))
{
    wmkcErr_obj error;
    if(!buf || !size || !elementSize) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkc_reverse: obj or size or elementSize is NULL.");
    }

    wmkcSize i;
    wmkcByte *swap = wmkcNull;
    wmkcByte *buf_p = (wmkcByte *)buf;

    if(!wmkcMem_new(wmkcByte *, swap, elementSize)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkc_reverse: Failed to allocate memory for swap.");
    }

    for(i = 0; i < size / 2; ++i) {
        memcpy(swap, buf_p + i * elementSize, elementSize);
        memcpy(buf_p + i * elementSize, buf_p + (size - i - 1) * elementSize, elementSize);
        memcpy(buf_p + (size - i - 1) * elementSize, swap, elementSize);
    }

    wmkcMem_free(swap);
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkc_split WMKC_OPEN_API
WMKC_OF((wmkcChar **key, wmkcChar **value, wmkcChar *string, wmkcChar symbol))
{
    wmkcErr_obj error;
    wmkcSize size = strlen(string);
    wmkcSize index;
    wmkcBool signal = false;
    wmkcSize keySize;
    wmkcSize valueSize;

    for(index = 0; index < size; ++index) {
        if(string[index] == symbol) {
            signal = true;
            break;
        }
    }

    if(!signal) {
        wmkcErr_return(error, wmkcErr_Err32,
            "wmkc_split: The corresponding symbol was not found.");
    }

    keySize = index;
    valueSize = strlen(string + index + 1);

    if(!wmkcMem_new(wmkcChar *, (*key), keySize + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkc_split: Failed to allocate memory for (*key).");
    }
    if(!wmkcMem_new(wmkcChar *, (*value), valueSize + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkc_split: Failed to allocate memory for (*value).");
    }
    (*key)[keySize] = 0x00;
    (*value)[valueSize] = 0x00;

    memcpy((*key), string, keySize);
    memcpy((*value), string + keySize + 1, valueSize);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkc_scanf WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize size))
{
    wmkcErr_obj error;
    if(!buf || !size) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkc_scanf: buf or size is NULL.");
    }

    for(wmkcSize x = 0; x < size; ++x) {
        if((buf[x] = getchar()) == 0x0a) {
            buf[x] = 0x00;
            break;
        }
    }

    fflush(stdin);

    if(!(*buf)) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkc_scanf: buf is NULL.");
    }
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkc_trim WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *src, wmkcChar symbol))
{
    wmkcErr_obj error;
    if(!dst || !src) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkc_trim: dst or src is NULL.");
    }

    wmkcChar *end = wmkcNull;
    wmkcChar *swap = strdup(src);
    wmkcChar *swap_p = swap;
    wmkcSize size;

    if(!swap) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkc_trim: Failed to allocate memory for swap.");
    }

    if(*swap == symbol) {
        while(*++swap == symbol);
    }
    end = swap + strlen(swap) - 1;
    if(*end == symbol) {
        while((end > swap) && (*--end == symbol));
    }
    *(end + 1) = 0x00;

    if(!(size = strlen(swap))) {
        (*dst) = wmkcNull;
        goto done;
    }
    if(!wmkcMem_new(wmkcChar *, (*dst), size + 1)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkc_trim: Failed to allocate memory for (*dst).");
    }
    memcpy((*dst), swap, size);
    (*dst)[size] = 0x00;

done:
    wmkcMem_free(swap_p);
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
