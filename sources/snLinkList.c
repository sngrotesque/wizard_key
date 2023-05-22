#include <snLinkList.h>

SN_PUBLIC(snErr_ctx) snLinkList_new SN_OPEN_API
SN_FUNC_OF((snLink **obj, snSize length))
{
    snErr_ctx error;
    if(!obj || !length) {
        snErr_return(error, snErr_ErrNullData,
            "snLinkList_new: obj or length is NULL.");
    }
    snLink *head = snNull;
    snLink *ptr = snNull;
    snSize index;

    for(index = 0; index < length; ++index) {
        if(head == snNull) {
            if(!snMemoryNew(snLink *, head, sizeof(snLink))) {
                snErr_return(error, snErr_ErrMemory,
                    "snLinkList_new: head failed to apply for memory.");
            }
            ptr = head;
        } else {
            if(!snMemoryNew(snLink *, ptr->next, sizeof(snLink))) {
                snErr_return(error, snErr_ErrMemory,
                    "snLinkList_new: ptr->next failed to apply for memory.");
            }
            ptr = ptr->next;
        }
    }

    ptr->next = snNull;
    (*obj) = head;

    snErr_return(error, snErr_OK, "OK.");
}

SN_PUBLIC(snErr_ctx) snLinkList_free SN_OPEN_API
SN_FUNC_OF((snLink **obj))
{
    snErr_ctx error;
    if(!obj) {
        snErr_return(error, snErr_ErrNullData,
            "snLinkList_new: obj or length is NULL.");
    }
    snLink *ptr = snNull;
    while(*obj)
    {
        ptr = *obj;
        *obj = (*obj)->next;
        free(ptr);
    }

    snErr_return(error, snErr_OK, "OK.");
}