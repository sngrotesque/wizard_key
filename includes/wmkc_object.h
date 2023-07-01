#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_OBJECT
#define WMKC_OBJECT
#include <wmkc_error.h>
#include <wmkc_memory.h>

// wmkc对象（后续可能还需要添加一些成员）
typedef struct {
    wmkcByte *buf; // 缓冲区数据
    wmkcSize size; // 缓冲区长度
    wmkcSize memSize; // 内存长度（申请成功的内存空间长度）
} wmkc_obj;

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
WMKC_OF((wmkc_obj **obj, wmkcSize _memory_size, wmkcBool _Clear_memory));

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
WMKC_OF((wmkc_obj **obj));

#endif /* WMKC_OBJECT */
#endif /* WMKC_SUPPORT */
