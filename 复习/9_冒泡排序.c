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
    冒泡排序：
        从第一个数开始，两两比较
*/

void BubbleSort(int *arr, int len) // 从小到大
{
    for (int i = 1; i < len; i++)
    {
        for (int j = 0; j < len - 1; j++)
        {
            if (arr[j] > arr[i]) // 前一个比后一个大
            {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}
int main(int argc, char const *argv[])
{
    int arr[6] = {6, 4, 5, 3, 1, 2};

    // 冒泡排序
    printf("冒泡排序:");
    BubbleSort(arr, 6);
    print(arr, 6);
}