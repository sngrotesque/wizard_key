#include <image/wmkc_png.h>

/**
 * @brief 为wmkcPng对象申请内存空间
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcPng对象申请内存空间。
 * 
 * @note 未完成
 * @param obj wmkcPng对象的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcPng_new WMKC_OPEN_API
WMKC_OF((wmkcPng_block **obj))
{
    wmkcErr_obj error;
    
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 为wmkcPng对象释放内存空间
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcPng对象释放内存空间。
 * 
 * @note 未完成
 * @param obj wmkcPng对象的地址
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcPng_free WMKC_OPEN_API
WMKC_OF((wmkcPng_block **obj))
{
    wmkcErr_obj error;
    
    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

