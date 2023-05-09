/**
 *  此库提供了排序算法，分别有：
 *      冒泡排序，选择排序，插入排序，
 *      归并排序，希尔排序，快速排序，
 *      堆排序，计数排序，双调排序。
 *  
 *  后续可能会提供更多的排序算法。
 *  所有的排序算法均为从网络上获得并修改。
*/
#ifndef __SN_SORT__
#define __SN_SORT__

#include <snConf.h>
#if !defined(__SN_MEMORY)
#include <stdlib.h>
#endif

// 冒泡排序，65536字节长度最坏情况，用时：15.66s
SN_PUBLIC(snVoid) bubbleSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size));

// 选择排序，65536字节长度最坏情况，用时：8.53s
SN_PUBLIC(snVoid) selectionSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size));

// 插入排序，65536字节长度最坏情况，用时：1.31s
SN_PUBLIC(snVoid) insertionSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size));

// 归并排序，16777216字节长度最坏情况，用时（三次取最快）：0.80s
SN_PUBLIC(snVoid) mergeSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size));

// 希尔排序，16777216字节长度最坏情况，用时（三次取最快）：1.08s
SN_PUBLIC(snVoid) shellSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size));

// 快速排序，16777216字节长度最坏情况，用时（三次取最快）：0.41s
SN_PUBLIC(snVoid) quickSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize first, snSize last));

// 堆排序，16777216字节长度最坏情况，用时（三次取最快）：2.14s
SN_PUBLIC(snVoid) heapSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size));

// 计数排序，16777216字节长度最坏情况，用时（三次取最快）：0.09s
SN_PUBLIC(snVoid) countSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size));

// 双调排序，16777216字节长度最坏情况，用时（三次取最快）：4.00s
SN_PUBLIC(snVoid) bitonicSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size));

#endif