#include <snLinkList.h>

WMKC_PUBLIC(snErr_ctx) snLinkList_new WMKC_OPEN_API
WMKC_OF((snLink **obj, wmkcSize length))
{
    snErr_ctx error;
    if(!obj || !length) {
        snErr_return(error, snErr_ErrNULL,
            "snLinkList_new: obj or length is NULL.");
    }
    snLink *head = wmkcNull;
    snLink *ptr = wmkcNull;
    wmkcSize index;

    for(index = 0; index < length; ++index) {
        if(head == wmkcNull) {
            if(!wmkcMemoryNew(snLink *, head, sizeof(snLink))) {
                snErr_return(error, snErr_ErrMemory,
                    "snLinkList_new: head failed to apply for memory.");
            }
            ptr = head;
        } else {
            if(!wmkcMemoryNew(snLink *, ptr->next, sizeof(snLink))) {
                snErr_return(error, snErr_ErrMemory,
                    "snLinkList_new: ptr->next failed to apply for memory.");
            }
            ptr = ptr->next;
        }
    }

    ptr->next = wmkcNull;
    (*obj) = head;

    snErr_return(error, snErr_OK, "OK.");
}

WMKC_PUBLIC(snErr_ctx) snLinkList_free WMKC_OPEN_API
WMKC_OF((snLink **obj))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNULL,
            "snLinkList_free: obj or length is NULL.");
    }
    snLink *ptr = wmkcNull;
    while(*obj)
    {
        ptr = *obj;
        *obj = (*obj)->next;
        free(ptr);
    }

    snErr_return(error, snErr_OK, "OK.");
}