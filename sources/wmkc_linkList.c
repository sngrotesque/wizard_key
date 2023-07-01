/**
 * @note 此库具有实验性质，可以不编写注释。
*/
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
            if(!wmkcMem_new(wmkcLink *, head, sizeof(wmkcLink))) {
                wmkcErr_return(error, wmkcErr_ErrMemory,
                    "wmkcLinkList_new: Failed to allocate memory for head.");
            }
            ptr = head;
        } else {
            if(!wmkcMem_new(wmkcLink *, ptr->next, sizeof(wmkcLink))) {
                wmkcErr_return(error, wmkcErr_ErrMemory,
                    "wmkcLinkList_new: Failed to allocate memory for ptr->next.");
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