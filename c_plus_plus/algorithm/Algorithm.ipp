#include "Algorithm.h"
#include <algorithm>
#include <cstring>
#include <utility>
#include <vector>

using std::copy;
using std::min;
using std::swap;
using std::vector;

/*
参考资料：
https://www.runoob.com/w3cnote/ten-sorting-algorithm.html
https://zhuanlan.zhihu.com/p/42586566
https://www.cnblogs.com/onepixel/articles/7674659.html
*/

/*******************************************************************************************************
*                                              公共函数
*******************************************************************************************************/


/*******************************************************************************************************
*                                              冒泡排序
*******************************************************************************************************/

/*
原理：https://www.runoob.com/w3cnote/bubble-sort.html
1. 比较相邻的元素。如果第一个比第二个大，就交换他们两个。
2. 对每一对相邻元素作同样的工作，从开始第一对到结尾的最后一对。这步做完后，最后的元素会是最大的数。
3. 针对所有的元素重复以上的步骤，除了最后一个。
4. 持续每次对越来越少的元素重复上面的步骤，直到没有任何一对数字需要比较。
*/

/*
*Function    : bubbleSort_Ascend
*Description : 冒泡排序（升序）
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array，数组
*Return      : void
*Caution     :
*/
template<class T>
void bubbleSort_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //数组长度

    for (int32_t i = 0; i < len - 1; ++i)    //外循环为排序趟数，len个数进行len-1趟
    {
        for (int32_t j = 0; j < len - 1 - i; ++j)    //内循环为每趟比较的次数，第i趟比较len-i次
        {
            if (array[j] > array[j + 1])    //相邻元素比较并交换
            {
                swap(array[j], array[j + 1]);
            }
        }
    }
}    //bubbleSort_Ascend()

/*******************************************************************************************************
*                                              选择排序
*******************************************************************************************************/

/*
算法步骤：https://www.runoob.com/w3cnote/selection-sort.html
1. 首先在未排序序列中找到最小元素，存放到排序序列的起始位置。
2. 再从剩余未排序元素中继续寻找最小元素，然后放到已排序序列的末尾。
3. 重复第二步，直到所有元素均排序完毕。
*/

/*
*Function    : selectSort_Ascend
*Description : 选择排序（升序）
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array，数组
*Return      : void
*Caution     :
*/
template<class T>
void selectSort_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //数组长度

    for (int32_t i = 0; i < len - 1; ++i)    //len个数进行len-1趟选择交换
    {
        int32_t minIndex = i;
        for (int32_t j = minIndex + 1; j < len; ++j)    //走访未排序的元素
        {
            if (array[j] < array[minIndex])
            {
                minIndex = j;    //记录最小值的位置
            }
        }
        swap(array[i], array[minIndex]);    //未排序元素的头和未排序元素中的最小值交换
    }
}    //selectSort_Ascend()

/*******************************************************************************************************
*                                              插入排序
*******************************************************************************************************/

/*
算法步骤：https://www.runoob.com/w3cnote/insertion-sort.html
1. 将第一待排序序列第一个元素看做一个有序序列，把第二个元素到最后一个元素当成是未排序序列。
2. 从头到尾依次扫描未排序序列，将扫描到的每个元素插入有序序列的适当位置。（如果待插入的元素与有序序列中的某个元素相等，则将待插入元素插入到相等元素的后面。）
*/

/*
*Function    : insertSort_Ascend
*Description : 插入排序（升序）
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array
*Return      : void
*Caution     :
*/
template<class T>
void insertSort_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //数组长度

    for (int32_t i = 1; i < len; ++i)    //从下标为1的元素开始选择合适的位置插入，因为下标为0的只有一个元素，默认是有序的
    {
        T insertElem = array[i];    //记录要插入的数据

        //从已经排序的序列最右边的开始比较，找到比其小或相等的数
        int32_t j;
        for (j = i - 1; j >= 0 && insertElem < array[j]; --j)
        {
            array[j + 1] = array[j];
        }

        array[j + 1] = insertElem;    //插入到比其小或相等的数的后边一个位置
    }
}    //insertSort_Ascend()

/*******************************************************************************************************
*                                              希尔排序
*******************************************************************************************************/

/*
算法步骤：https://blog.csdn.net/qq_39207948/article/details/80006224
1. 选择一个增量序列 t1，t2，……，tk，其中 ti > tj, tk = 1；
2. 按增量序列个数 k，对序列进行 k 趟排序；
3. 每趟排序，根据对应的增量 ti，将待排序列分割成若干长度为 m 的子序列，分别对各子表进行直接插入排序。仅增量因子为 1 时，整个序列作为一个表来处理，表长度即为整个序列的长度。
*/

/*
*Function    : shellSort_Ascend
*Description : 希尔排序（升序）
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array，数组
*Return      : void
*Caution     :
*/
template<class T>
void shellSort_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //数组长度

    //计算增量：[O(n^(3/2)) by Knuth, 1973]: 1, 4, 13, 40, 121, ...
    int32_t gap = 1;
    while (gap < len / 3)
    {
        gap = gap * 3 + 1;
    }

    //若干次的插入排序
    for (; gap >= 1; gap = gap / 3)
    {
        //插入排序
        //对各组的插入排序并不是先对一个组进行排序完毕后再对另一个组进行排序，而是轮流对每个组进行插入排序
        for (int32_t i = gap; i < len; ++i)
        {
            T insertElem = array[i];
            int32_t j;
            for (j = i - gap; j >= 0 && insertElem < array[j]; j -= gap)
            {
                array[j + gap] = array[j];
            }
            array[j + gap] = insertElem;
        }
    }
}    //shellSort_Ascend()

/*******************************************************************************************************
*                                              归并排序
*******************************************************************************************************/

/*********************************递归法************************************/

/*
算法步骤：https://www.runoob.com/w3cnote/merge-sort.html
1. 申请空间，使其大小为两个已经排序序列之和，该空间用来存放合并后的序列；
2. 设定两个指针，最初位置分别为两个已经排序序列的起始位置；
3. 比较两个指针所指向的元素，选择相对小的元素放入到合并空间，并移动指针到下一位置；
4. 重复步骤 3 直到某一指针达到序列尾；
5. 将另一序列剩下的所有元素直接复制到合并序列尾。
*/

template<class T>
void mergeSortedArray_Recursion_Ascend(INOUT vector<T>& array, IN vector<T>& tempArray, int32_t start1, int32_t end1, int32_t start2, int32_t end2)
{
    int32_t start1Old = start1;
    int32_t k = start1;

    //比较两个指针所指向的元素，选择相对小的元素放入到合并空间，并移动指针到下一位置
    while (start1 <= end1 && start2 <= end2)
    {
        tempArray[k++] = array[start1] < array[start2] ? array[start1++] : array[start2++];
    }

    //将另一序列剩下的所有元素直接复制到合并序列尾（两个while只有一个会执行）
    while (start1 <= end1)
    {
        tempArray[k++] = array[start1++];
    }
    while (start2 <= end2)
    {
        tempArray[k++] = array[start2++];
    }

    //合并结果复制回原数组中（注意：这里只复制合并的那一段，而不是全部复制）
    memcpy(&array[start1Old], &tempArray[start1Old], (end2 - start1Old + 1) * sizeof(T));
}    //mergeSortedArray_Recursion_Ascend()

template<class T>
void internalMergeSort_Recursion_Ascend(INOUT vector<T>& array, IN vector<T>& tempArray, int32_t start, int32_t end)
{
    if (start < end)    //当 start >= end 时停止分割
    {
        int32_t middle = (start + end) / 2;
        internalMergeSort_Recursion_Ascend(array, tempArray, start, middle);    //递归分割左子数组
        internalMergeSort_Recursion_Ascend(array, tempArray, middle + 1, end);    //递归分割右子数组
        mergeSortedArray_Recursion_Ascend(array, tempArray, start, middle, middle + 1, end);    //合并左右数组
    }
}    //internalMergeSort_Recursion_Ascend()

/*
*Function    : mergeSort_Recursion_Ascend
*Description : 归并排序（递归法；升序）
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array，数组
*Return      : void
*Caution     :
*/
template<class T>
void mergeSort_Recursion_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //数组长度
    vector<T> tempArray(len);    //内部排序临时存储数组
    internalMergeSort_Recursion_Ascend(array, tempArray, 0, len - 1);    //排序
}    //mergeSort_Recursion_Ascend()

/********************************非递归法***********************************/

/*
算法步骤：
1. 将序列每相邻两个数字进行归并操作，形成ceil(n/2)个序列，排序后每个序列包含两/一个元素
2. 若此时序列数不是1个则将上述序列再次归并，形成ceil(n/4)个序列，每个序列包含四/三个元素
3. 重复步骤2，直到所有元素排序完毕，即序列数为1
*/

template<class T>
void mergeSortedArray_Iteration_Ascend(INOUT vector<T>& array, IN vector<T>& tempArray, int32_t start1, int32_t end1, int32_t start2, int32_t end2)
{
    int32_t start1Old = start1;
    int32_t k = start1;

    //比较两个指针所指向的元素，选择相对小的元素放入到合并空间，并移动指针到下一位置
    while (start1 <= end1 && start2 <= end2)
    {
        tempArray[k++] = array[start1] < array[start2] ? array[start1++] : array[start2++];
    }

    //将另一序列剩下的所有元素直接复制到合并序列尾（两个while只有一个会执行）
    while (start1 <= end1)
    {
        tempArray[k++] = array[start1++];
    }
    while (start2 <= end2)
    {
        tempArray[k++] = array[start2++];
    }
}    //mergeSortedArray_Iteration_Ascend()

/*
*Function    : mergeSort_Iteration_Ascend
*Description : 归并排序（迭代法；升序）
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array，数组
*Return      : void
*Caution     :
*/
template<class T>
void mergeSort_Iteration_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //数组长度
    vector<T> tempArray(len);    //内部排序临时存储数组

    for (int32_t segment = 1; segment < len; segment *= 2)    //对整个序列依次分段，1，2，4，8，...
    {
        for (int32_t i = 0; i < len; i += segment * 2)    //归并排序
        {
            int32_t low = i, middle = min(i + segment - 1, len - 1), high = min(i + segment * 2 - 1, len - 1);
            int32_t start1 = low, end1 = middle, start2 = middle + 1, end2 = high;
            mergeSortedArray_Iteration_Ascend(array, tempArray, start1, end1, start2, end2);
        }

        //交换两个数组的值；
        //迭代法内层for循环依次归并完之后tempArray数组包含了array的所有元素，因此可以在内层for执行之后交换两个数组的值，
        //不用在内层for循环的每次归并后把合并结果复制回原数组中，可以提高效率；
        //递归法每次归并tempArray只是部分位置放置了array的元素，因此每次归并需要把合并结果复制回原数组中
        array.swap(tempArray);
    }
}    //mergeSort_Iteration_Ascend()