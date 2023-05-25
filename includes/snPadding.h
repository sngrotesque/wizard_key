#ifndef __SN_PADDING__
#define __SN_PADDING__

#include <snConf.h>
#include <snRand.h>
#include <snTime.h>

#define snPadding_offset(BlockSize, size) (BlockSize - size % BlockSize)
#define snPadding_totalSize(BlockSize, size) (snPadding_offset(BlockSize, size) + size)
#define snPadding_value 0xac

// 填充函数
WMKC_PUBLIC(wmkcVoid) snPadding_add WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize *size, wmkc_u16 BlockSize, wmkcBool randomPadding));

// 删除已填充的数据量
WMKC_PUBLIC(wmkcVoid) snPadding_remove WMKC_OPEN_API
WMKC_OF((wmkcByte *buf, wmkcSize *size));

// 为密钥填充
WMKC_PUBLIC(wmkcVoid) snPadding_keyAdd WMKC_OPEN_API
WMKC_OF((wmkcByte *token, wmkc_u16 BlockSize));

#endif
