#include <snLinkList.h>

SN_PUBLIC(snError) snLinkList_new SN_OPEN_API
SN_FUNC_OF((snLink **obj, snSize length))
{
    if(!length || !obj) {
        return snErr_ErrNullData;
    }
    snLink *head = snNull;
    snLink *ptr = snNull;
    snSize index;

    for(index = 0; index < length; ++index) {
        if(head == snNull) {
            if(!snMemoryNew(snLink *, head, sizeof(snLink)))
                return snErr_ErrMemory;
            ptr = head;
        } else {
            if(!snMemoryNew(snLink *, ptr->next, sizeof(snLink)))
                return snErr_ErrMemory;
            ptr = ptr->next;
        }
    }

    ptr->next = snNull;
    (*obj) = head;

    return snErr_OK;
}

SN_PUBLIC(snError) snLinkList_free SN_OPEN_API
SN_FUNC_OF((snLink **obj))
{
    if(!obj || !(*obj)) {
        return snErr_ErrNullData;
    }
    snLink *ptr = snNull;
    while(*obj)
    {
        ptr = *obj;
        *obj = (*obj)->next;
        free(ptr);
    }

    return snErr_OK;
}