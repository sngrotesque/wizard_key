#ifndef __SN_PADDING__
#define __SN_PADDING__

#include <snConf.h>
#include <snRand.h>
#include <snTime.h>

#define snPadding_offset(BlockSize, size) (BlockSize - size % BlockSize)
#define snPadding_totalSize(BlockSize, size) (snPadding_offset(BlockSize, size) + size)
#define snPadding_value 0xac

// 填充函数
SN_PUBLIC(snVoid) snPadding_add SN_OPEN_API
SN_FUNC_OF((snByte *buf, snSize *size, sn_u16 BlockSize, snBool randomPadding));

// 删除已填充的数据量
SN_PUBLIC(snVoid) snPadding_remove SN_OPEN_API
SN_FUNC_OF((snByte *buf, snSize *size));

// 为密钥填充
SN_PUBLIC(snVoid) snPadding_keyAdd SN_OPEN_API
SN_FUNC_OF((snByte *token, sn_u16 BlockSize));

#endif
