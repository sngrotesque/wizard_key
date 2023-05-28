/**
 *  此库提供了排序算法，分别有：
 *      冒泡排序，选择排序，插入排序，
 *      归并排序，希尔排序，快速排序，
 *      堆排序，计数排序，双调排序。
 *  
 *  后续可能会提供更多的排序算法。
 *  所有的排序算法均为从网络上获得并修改。
*/
#ifndef WMKC_SORT
#define WMKC_SORT

#include <wmkc_conf.h>
#if !defined(WMKC_MEMORY)
#include <stdlib.h>
#endif

// 冒泡排序，65536字节长度最坏情况，用时：15.66s
WMKC_PUBLIC(wmkcVoid) bubbleSort WMKC_OPEN_API
WMKC_OF((wmkc_s32 *arr, wmkcSize size));

// 选择排序，65536字节长度最坏情况，用时：8.53s
WMKC_PUBLIC(wmkcVoid) selectionSort WMKC_OPEN_API
WMKC_OF((wmkc_s32 *arr, wmkcSize size));

// 插入排序，65536字节长度最坏情况，用时：1.31s
WMKC_PUBLIC(wmkcVoid) insertionSort WMKC_OPEN_API
WMKC_OF((wmkc_s32 *arr, wmkcSize size));

// 归并排序，16777216字节长度最坏情况，用时（三次取最快）：0.80s
WMKC_PUBLIC(wmkcVoid) mergeSort WMKC_OPEN_API
WMKC_OF((wmkc_s32 *arr, wmkcSize size));

// 希尔排序，16777216字节长度最坏情况，用时（三次取最快）：1.08s
WMKC_PUBLIC(wmkcVoid) shellSort WMKC_OPEN_API
WMKC_OF((wmkc_s32 *arr, wmkcSize size));

// 快速排序，16777216字节长度最坏情况，用时（三次取最快）：0.41s
WMKC_PUBLIC(wmkcVoid) quickSort WMKC_OPEN_API
WMKC_OF((wmkc_s32 *arr, wmkcSize first, wmkcSize last));

// 堆排序，16777216字节长度最坏情况，用时（三次取最快）：2.14s
WMKC_PUBLIC(wmkcVoid) heapSort WMKC_OPEN_API
WMKC_OF((wmkc_s32 *arr, wmkcSize size));

// 计数排序，16777216字节长度最坏情况，用时（三次取最快）：0.09s
WMKC_PUBLIC(wmkcVoid) countSort WMKC_OPEN_API
WMKC_OF((wmkc_s32 *arr, wmkcSize size));

// 双调排序，16777216字节长度最坏情况，用时（三次取最快）：4.00s
WMKC_PUBLIC(wmkcVoid) bitonicSort WMKC_OPEN_API
WMKC_OF((wmkc_s32 *arr, wmkcSize size));

#endif