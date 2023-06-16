#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_PADDING
#define WMKC_PADDING
#include <wmkc_random.h>
#include <wmkc_time.h>

#define WMKC_PAD_VALUE 0xac // 填充值

/**
 * @brief 计算距离块大小的偏离量
 * @authors SN-Grotesque
 * @note 无
 * @param BlockSize 数字，块大小
 * @param size 数字，源大小
 * @return 返回一个偏移量
 */
WMKC_PUBLIC(wmkcSize) wmkcPad_offset WMKC_OPEN_API
WMKC_OF((wmkc_u32 BlockSize, wmkcSize size));

/**
 * @brief 填充函数
 * @authors SN-Grotesque
 * @note 如果randomPadding为True则忽略WMKC_PAD_VALUE。否则使用WMKC_PAD_VALUE。
 *       后续考虑一下参数是否应改为传入wmkc对象或src与dst这种形式。
 *       因为当前这个函数成功执行的前提是必须保证传入的指针指向的内容长度
 *       大于等于总长度的（也就是加上填充数据后的长度）。
 * @param buf 这是一个指针，指向需要被填充的内容的地址。
 * @param size 这是一个指针，指向buf指向的内容的长度的值（会被改变）。
 * @param BlockSize 这是块长度，数据将被填充至这个数字的倍数。
 * @param randomPadding 如果为True，那么使用随机数填充。
 * @return 无
*/
WMKC_PUBLIC(wmkcVoid) wmkcPad_add WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize *size, wmkc_u32 BlockSize, wmkcBool randomPadding));

/**
 * @brief 删除填充的值
 * @authors SN-Grotesque
 * 
 * 删除填充的数据，全部归零
 * 
 * @note 此函数不会改变buf指向的内容的总长度，只是将填充的数据全部替换成00。
 * @param buf 这是一个指针，指向需要被填充的内容的地址。
 * @param size 这是一个指针，指向buf指向的内容的长度的值（会被改变）。
 * @return 无
*/
WMKC_PUBLIC(wmkcVoid) wmkcPad_remove WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize *size));

/**
 * @brief Token填充函数
 * @authors SN-Grotesque
 * 
 * 将Token填充，直到块长度的倍数
 * 
 * @note 请确保key指向的地址的已申请内存长度大于等于BlockSize，
 *       并且这个函数只能处理不含00字节的Token。
 *       此函数的填充逻辑是使用Token本身循环填充。
 * @example token = "1234567." -> Pad(12) -> "1234567.1234"
 * @param token 这是一个指针，指向需要被填充的内容的地址。
 * @param BlockSize 这是块长度，数据将被填充至这个数字的倍数。
 * @return 无
*/
WMKC_PUBLIC(wmkcVoid) wmkcPad_TokenAdd WMKC_OPEN_API
WMKC_OF((wmkcByte *token, wmkc_u32 BlockSize));

#endif /* WMKC_PADDING */
#endif /* WMKC_SUPPORT */
