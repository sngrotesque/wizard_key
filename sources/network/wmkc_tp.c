#include <network/wmkc_tp.h>

/**
 * @brief 为wmkcTp对象申请内存空间
 * @authors SN-Grotesque
 * @note 未完成
 * @param obj 这是一个指针，指向wmkcTp对象指针的地址。
 * @param addr 这是一个指针，指向目标主机的域名（IP地址）字符串。
 * @param port 这是一个端口，为目标主机的网络端口。
 * @param maxRetry 这是一个数字，代表最大重试次数，为0时使用
 *                 WMKC_TP_DEFAULT_MAX_RETRY的值。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_new WMKC_OPEN_API
WMKC_OF((wmkcTp_obj **obj, wmkcString addr, wmkc_u16 port, wmkc_u32 maxRetry))
{
    wmkcErr_obj error;

    if(!obj || !addr) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcTp_new: obj or addr is NULL.");
    }
    if(!wmkcMemoryNew(wmkcTp_obj *, (*obj), sizeof(wmkcTp_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcTp_new: (*obj) failed to apply for memory.");
    }

    (*obj)->addr = addr;
    if(port) {
        (*obj)->port = port;
    } else {
        (*obj)->port = WMKC_TP_DEFAULT_PORT;
    }
    if(maxRetry) {
        (*obj)->maxRetry = maxRetry;
    } else {
        (*obj)->maxRetry = WMKC_TP_DEFAULT_MAX_RETRY;
    }
    (*obj)->snc = wmkcNull;
    // SNC_new(&(*obj)->snc, SNC_256);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 为wmkcTp对象释放内存空间
 * @authors SN-Grotesque
 * @note 无
 * @param obj 这是一个指针，指向wmkcTp对象指针的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_free WMKC_OPEN_API
WMKC_OF((wmkcTp_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcTp_free: obj is NULL.");
    }

    if((*obj)->snc) {
        SNC_release(&(*obj)->snc);
    }
    wmkcMemoryFree((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 接收数据端函数
 * @authors SN-Grotesque
 * @note 未完成
 * @param obj 这是一个指针，指向wmkcTp对象的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_Listen WMKC_OPEN_API
WMKC_OF((wmkcTp_obj *obj, wmkcFileString fn))
{
    wmkcErr_obj error;
    if(!obj || !obj->addr || !obj->port || !fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcTp_Listen: obj or obj->addr or obj->port or fn is NULL.");
    }
    if(!obj->maxRetry) {
        obj->maxRetry = WMKC_TP_DEFAULT_MAX_RETRY;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 发送数据端函数
 * @authors SN-Grotesque
 * @note 未完成
 * @param obj 这是一个指针，指向wmkcTp对象的地址。
 * @param fn 这是一个指针，指向路径的字符串地址，如果传入字符串而不是指针，那么
 *           应使用wmkcFile_text宏对字符串进行转换。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
 */
WMKC_PUBLIC(wmkcErr_obj) wmkcTp_Client WMKC_OPEN_API
WMKC_OF((wmkcTp_obj *obj, wmkcFileString fn))
{
    wmkcErr_obj error;
    if(!obj || !obj->addr || !obj->port || !fn) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcTp_Listen: obj or obj->addr or obj->port or fn is NULL.");
    }
    if(!obj->maxRetry) {
        obj->maxRetry = WMKC_TP_DEFAULT_MAX_RETRY;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
