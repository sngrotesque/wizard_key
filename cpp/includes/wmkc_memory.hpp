#include <wmkc_conf.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_MEMORY
#define WMKC_CPP_MEMORY

#ifdef WMKC_PLATFORM_WINOS
#   include <Windows.h>
#endif

// 内存空间申请
#define wmkcMem_new(_Type, _Ptr, _Size) (_Ptr = (_Type)malloc(_Size))
// 内存追加申请
#define wmkcMem_append(_Type, _DstP, _SrcP, _Size) (_DstP = (_Type)realloc(_SrcP, _Size))
// 内存空间释放并指向空指针
#define wmkcMem_free(_Ptr) {free(_Ptr); _Ptr = wmkcNull;}
// 内存内容初始化为零
#define wmkcMem_zero(_Ptr, _Size) memset(_Ptr, 0, _Size)

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

#endif /* WMKC_CPP_MEMORY */
#endif /* WMKC_SUPPORT */
