/**
 * @note 此库具有实验性质，可以不编写注释。
*/
#ifndef WMKC_LINKLIST
#define WMKC_LINKLIST
#include <wmkc_error.h>
#include <wmkc_memory.h>
#include <wmkc_conf.h>

typedef struct wmkcLink {
    wmkcByte *content;
    struct wmkcLink *next;
} wmkcLink;

WMKC_PUBLIC(wmkcErr_obj) wmkcLinkList_new WMKC_OPEN_API
WMKC_OF((wmkcLink **obj, wmkcSize length));

WMKC_PUBLIC(wmkcErr_obj) wmkcLinkList_free WMKC_OPEN_API
WMKC_OF((wmkcLink **obj));

#endif
