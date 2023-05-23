#include <snSort.h>

SN_PRIVATE(snVoid) _mergeSort
SN_FUNC_OF((sn_32 *arr, snSize s1, snSize e1, snSize s2, snSize e2, sn_32 *m1, sn_32 *m2))
{
    snFast snSize len1 = e1 - s1 + 1;
    snFast snSize len2 = e2 - s2 + 1;
    snFast snSize p1 = 0;
    snFast snSize p2 = 0;
    snFast snSize p = s1;
    memcpy(m1, arr + s1, sizeof(sn_32) * len1);
    memcpy(m2, arr + s2, sizeof(sn_32) * len2);

    while (p1 < len1 && p2 < len2) {
        if(m1[p1] < m2[p2]){
            arr[p++] = m1[p1++];
        }
        else{
            arr[p++] = m2[p2++];
        }
    }
    while (p1 < len1) {
        arr[p++] = m1[p1++];
    }
    while(p2 < len2){
        arr[p++] = m2[p2++];
    }
}

SN_PRIVATE(sn_32) _quickSort
SN_FUNC_OF((sn_32 *arr, snSize first, snSize last, snSize pivot_index))
{
    snFast sn_u32 buf;
    if (pivot_index != first) {
        buf = arr[first];
        arr[first] = arr[pivot_index];
        arr[pivot_index] = buf;
    }

    pivot_index = first;
    first++;
    while (first <= last) {
        if (arr[first] <= arr[pivot_index]) {
            first++;
        } else {
            if (arr[last] > arr[pivot_index]) {
                last--;
            } else {
                buf = arr[first];
                arr[first] = arr[last];
                arr[last] = buf;
            }
        }
    }

    if (last != pivot_index) {
        buf = arr[pivot_index];
        arr[pivot_index] = arr[last];
        arr[last] = buf;
    }

    return last;
}

SN_PRIVATE(snVoid) _heapSort_heapify
SN_FUNC_OF((sn_32 *arr, snSize size, snSize i))
{
    snFast snSize largest = i; // Initialize largest as root
    snFast snSize left = 2 * i + 1; // left child
    snFast snSize right = 2 * i + 2; // right child
    snFast snSize temp;

    // If left child is larger than root
    if (left < size && arr[left] > arr[largest])
        largest = left;

    // If right child is larger than root
    if (right < size && arr[right] > arr[largest])
        largest = right;

    // If root is not largest
    if (largest != i) {
        temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        _heapSort_heapify(arr, size, largest);
    }
}

SN_PRIVATE(snVoid) _bitonicSort_exchange
SN_FUNC_OF((sn_32 *arr, snSize i, snSize j, sn_32 d))
{
    snFast sn_32 temp;
    if (d == (arr[i] > arr[j]))
    {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

SN_PRIVATE(snVoid) _bitonicSort_merge
SN_FUNC_OF((sn_32 *arr, snSize beg, snSize c, snSize d))
{
    snFast snSize k, i;
    if (c > 1)
    {
        k = c / 2;
        for (i = beg; i < (beg + k); i++)
            _bitonicSort_exchange(arr, i, i + k, d);
        _bitonicSort_merge(arr, beg, k, d);
        _bitonicSort_merge(arr, beg + k, k, d);
    }
}

SN_PRIVATE(snVoid) _bitonicSort
SN_FUNC_OF((sn_32 *arr, snSize beg, snSize c, snSize d))
{
    snFast snSize k;
    if (c > 1)
    {
        k = c / 2;
        _bitonicSort(arr, beg, k, 1);  // 按升序排序
        _bitonicSort(arr, beg + k, k, 0);  // 按降序排序
        _bitonicSort_merge(arr, beg, c, d);  // _bitonicSort_merge按升序合并序列
    }
}

SN_PUBLIC(snVoid) bubbleSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size))
{
    snFast sn_32 i, j, buf;
    for(i = 1; i < size; ++i) {
        for(j = 0; j < (size - i); ++j) {
            if(arr[j] > arr[j+1]) {
                buf = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = buf;
            }
        }
    }
}

SN_PUBLIC(snVoid) selectionSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size))
{
    snFast sn_32 i, j, buf;
    snFast sn_32 minIndex;
    for(i = 0; i < size-1; ++i) {
        minIndex = i;
        for(j = i + 1; j < size; ++j) {
            if(arr[j] < arr[minIndex])
                minIndex = j;
        }
        if(i != minIndex) {
            buf = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = buf;
        }
    }
}

SN_PUBLIC(snVoid) insertionSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size))
{
    snFast sn_32 i, j;
    snFast sn_32 buff;
    for(i = 1; i < size; ++i) {
        buff = arr[i];
        j = i - 1;
        while(j >= 0 && arr[j] > buff) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = buff;
    }
}

SN_PUBLIC(snVoid) mergeSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size))
{
    snFast snSize step = 1;
    snFast sn_32 *m1 = (sn_32 *)malloc(size * sizeof(sn_32));
    snFast sn_32 *m2 = (sn_32 *)malloc(size * sizeof(sn_32));
    if(!m1 || !m2)
        return;
    
    snFast snSize i, s1, e1, s2, e2;
    while(step < size) {
        for(i = 0; (i + step - 1) < (size - 1); i += 2 * step) {
            s1 = i;
            e1 = i + step - 1;
            s2 = e1 + 1;
            if((i + 2 * step - 1) < (size - 1)) {
                e2 = i + 2 * step - 1;
            } else {
                e2 = size - 1;
            }
            _mergeSort(arr, s1, e1, s2, e2, m1, m2);
        }
        step = step << 1;
    }
}

SN_PUBLIC(snVoid) shellSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size))
{
    snFast snSize gaps, i, j;
    snFast sn_32 buff;
    for(gaps = size / 2; gaps > 0; gaps /= 2) {
        for(i = gaps; i < size; i += 1) {
            buff = arr[i];
            for(j = i; j >= gaps && arr[j - gaps] > buff; j -= gaps) {
                arr[j] = arr[j - gaps];
            }
            arr[j] = buff;
        }
    }
}

SN_PUBLIC(snVoid) quickSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize first, snSize last))
{
    snFast snSize pivot;
    if (first >= last)
        return;
    
    pivot = (first + last) / 2;
    pivot = _quickSort(arr, first, last, pivot);
    
    if (first < pivot) {
        quickSort(arr, first, pivot - 1);
    }
    if (pivot < last) {
        quickSort(arr, pivot + 1, last);
    }
}

SN_PUBLIC(snVoid) heapSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size))
{
    snFast snSize i;
    snFast snSize temp;

    for (i = size / 2 - 1; i; i--)
        _heapSort_heapify(arr, size, i);

    // One by one extract an element from heap
    for (i = size - 1; i; i--) {
        /* Move current root element to end*/
        temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        _heapSort_heapify(arr, i, 0);
    }
}

SN_PUBLIC(snVoid) countSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size))
{
    snFast sn_32 max = arr[0];
    snFast snSize i;

    static sn_32 *dst = snNull;
    static sn_32 *count = snNull;

    for(i = 1; i < size; i++)
        if(arr[i] > max)
            max = arr[i];

    dst = (sn_32 *)malloc(sizeof(sn_32) * (size + 1));
    count = (sn_32 *)malloc(sizeof(sn_32) * (max + 1));

    memset(count, 0x00, sizeof(sn_32) * max);

    for (i = 0; i < size; i++) {
        count[arr[i]]++; // 存储每个元素的计数
    }

    for(i = 1; i <= max; i++)
        count[i] += count[i-1]; // 查找累积频率

    // 此循环将在count数组中查找原始数组的每个元素的索引，并且将元素放置在输出数组中
    for (i = size - 1; i; i--) {
        dst[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--; // 相同数字的递减计数
    }

    for(i = 0; i < size; i++) {
        arr[i] = dst[i]; // 将排序后的元素存储到主数组中
    }

    free(dst);
    free(count);
}

SN_PUBLIC(snVoid) bitonicSort SN_OPEN_API
SN_FUNC_OF((sn_32 *arr, snSize size))
{
    _bitonicSort(arr, 0, size, 1); // 1表示增量排序
}
