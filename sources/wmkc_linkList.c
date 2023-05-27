#include <wmkc_linkList.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcLinkList_new WMKC_OPEN_API
WMKC_OF((wmkcLink **obj, wmkcSize length))
{
    wmkcErr_obj error;
    if(!obj || !length) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcLinkList_new: obj or length is NULL.");
    }
    wmkcLink *head = wmkcNull;
    wmkcLink *ptr = wmkcNull;
    wmkcSize index;

    for(index = 0; index < length; ++index) {
        if(head == wmkcNull) {
            if(!wmkcMemoryNew(wmkcLink *, head, sizeof(wmkcLink))) {
                wmkcErr_return(error, wmkcErr_ErrMemory,
                    "wmkcLinkList_new: head failed to apply for memory.");
            }
            ptr = head;
        } else {
            if(!wmkcMemoryNew(wmkcLink *, ptr->next, sizeof(wmkcLink))) {
                wmkcErr_return(error, wmkcErr_ErrMemory,
                    "wmkcLinkList_new: ptr->next failed to apply for memory.");
            }
            ptr = ptr->next;
        }
    }

    ptr->next = wmkcNull;
    (*obj) = head;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcLinkList_free WMKC_OPEN_API
WMKC_OF((wmkcLink **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcLinkList_free: obj or length is NULL.");
    }
    wmkcLink *ptr = wmkcNull;
    while(*obj)
    {
        ptr = *obj;
        *obj = (*obj)->next;
        free(ptr);
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}