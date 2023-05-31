#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_DICT
#define WMKC_DICT

typedef struct wmkcDict_obj {
    wmkcByte *key;
    wmkcByte *value;
    struct wmkcDict_obj *next;
} wmkcDict_obj;


#endif /* WMKC_DICT */
#endif /* WMKC_SUPPORT */
