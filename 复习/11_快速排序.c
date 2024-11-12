#include <stdio.h>
void print(int *arr, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
/*
    快速排序：
        分治法：比大小，再分区
        1.从数组中选择一个数作为基准数，通常选择的是第一个数
        2.分区：把比基准数小的数放在基准数的左边，把比基准数大的数放到基准数的右边。
        3.再对左右区间重复第二步，直到区间中只有一个数。
*/
void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
void QuickSort(int *arr, int left, int right)
{
    int L = left;
    int R = right;
    if (L >= R)
    {
        return;
    }
    int key = arr[left]; // 指定基数
    while (L != R)
    {
        while (arr[R] >= key && L < R)
        {
            R--;
        }
        while (arr[L] <= key && L < R)
        {
            L++;
        }
        if (L < R)
        {
            swap(&arr[L], &arr[R]);
        }
    }
    swap(&arr[left], &arr[L]);
    QuickSort(arr, left, L - 1);
    QuickSort(arr, L + 1, right);
}
int main(int argc, char const *argv[])
{
    int arr[5] = {5, 4, 3, 2, 1};
    // 快速排序
    printf("快速排序:");
    QuickSort(arr, 0, 4);
    print(arr, 5);
    printf("-------------\n");

    return 0;
}
