#include <wmkc_conf.h>

#if WMKC_SUPPORT
#ifndef WMKC_MEMORY
#define WMKC_MEMORY
#include <wmkc_error.h>

// 内存空间申请
#define wmkcMem_new(type, x, size) (x = (type)malloc(size))
// 内存空间释放并指向空指针
#define wmkcMem_free(x) free(x); x = wmkcNull
// 内存内容初始化为零
#define wmkcMem_zero(x, s) memset(x, 0x0, s)

/**
 * @brief 安全清除内存数据函数
 * @authors SN-Grotesque
 * @note 因某些情况下memset函数会被编译器优化掉，所以
 *       需要调用更安全的函数来执行内存清零操作。
 *       因为性能考虑所以不使用操作系统提供的对应函数。
 *       并且使用操作系统提供的函数的话可能会导致无意义的程序大小增加。
 *       此函数会针对4的倍数长度的缓冲区内存区域清零做优化。
 * @param buf 指针，指向数据的地址。
 * @param size 这是一个长度，代表数据的字节长度。
 * @return 无
 */
WMKC_PUBLIC(wmkcVoid) wmkcMem_secure WMKC_OPEN_API
WMKC_OF((wmkcVoid *buf, wmkcSize size));

#endif /* WMKC_MEMORY */
#endif /* WMKC_SUPPORT */
