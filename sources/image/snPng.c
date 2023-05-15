#include <image/snPng.h>

SN_PUBLIC(snError) snPng_new SN_OPEN_API
SN_FUNC_OF((snPng_block **obj, snSize length))
{
    if(!length) {
        return snErr_ErrNullData;
    }
    snPng_block *head = snNull;
    snPng_block *ptr = snNull;
    snSize index;

    for(index = 0; index < length; ++index) {
        if(head == snNull) {
            if(!(head = (snPng_block *)malloc(sizeof(snPng_block)))) {
                return snErr_Memory;
            }
            ptr = head;
        } else {
            if(!(ptr->next = (snPng_block *)malloc(sizeof(snPng_block)))) {
                return snErr_Memory;
            }
            ptr = ptr->next;
        }
    }

    ptr->next = snNull;
    (*obj) = head;

    return snErr_OK;
}

// SN_PUBLIC(snError) snPng_release SN_OPEN_API
// SN_FUNC_OF((snPng_block **obj))
// {
//     snPng_block *ptr = snNull;
//     while(*obj)
//     {
//         ptr = *obj;
//         *obj = (*obj)->next;
//         free( ptr );
//     }

//     return snErr_OK;
// }

SN_PUBLIC(snError) snPng_release SN_OPEN_API
SN_FUNC_OF((snPng_block **obj))
{
    if(!obj || !(*obj)) {
        return snErr_ErrNullData;
    }
    snPng_block *ptr = snNull;
    while(*obj)
    {
        ptr = *obj;
        *obj = (*obj)->next;
        free(ptr);
    }

    return snErr_OK;
}

