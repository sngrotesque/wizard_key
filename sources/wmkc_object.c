#include <wmkc_object.h>

/**
 * @brief 为wmkc对象申请内存空间
 * @authors SN-Grotesque
 * 
 * 为wmkc对象申请内存空间并初始化
 * 
 * @note 如果_memory_size为0，那么忽略_Clear_memory的值，并将obj->buf指向NULL。
 * @param obj 是一个指针，指向wmkc对象指针的地址。
 * @param _memory_size 需要申请的内存空间的长度，可以为0。
 * @param _Clear_memory 为True会将申请到的内存空间的内容使用00初始化。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcObject_new WMKC_OPEN_API
WMKC_OF((wmkc_obj **obj, wmkcSize _memory_size, wmkcBool _Clear_memory))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcObject_new: obj is NULL.");
    }

    if(!wmkcMem_new(wmkc_obj *, (*obj), sizeof(wmkc_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcObject_new: Failed to allocate memory for (*obj).");
    }

    if(_memory_size) {
        if(!wmkcMem_new(wmkcByte *, (*obj)->buf, _memory_size)) {
            wmkcErr_return(error, wmkcErr_ErrMemory,
                "wmkcObject_new: Failed to allocate memory for (*obj)->buf.");
        }
        if(_Clear_memory)
            memset((*obj)->buf, 0x00, _memory_size);
    } else {
        (*obj)->buf = wmkcNull;
        (*obj)->size = 0;
    }
    (*obj)->memSize = _memory_size;

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 为wmkc对象释放内存空间
 * @authors SN-Grotesque
 * 
 * 为wmkc对象释放内存空间
 * 
 * @note 无
 * @param obj 是一个指针，指向wmkc对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcObject_free WMKC_OPEN_API
WMKC_OF((wmkc_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcObject_new: obj is NULL.");
    }

    if((*obj)->buf) {
        wmkcMem_free((*obj)->buf);
    }
    wmkcMem_free((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
