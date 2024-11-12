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
void BubbleSort(int *arr, int len)
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
    int arr[5] = {5, 4, 3, 2, 1};

    // 冒泡排序
    printf("冒泡排序:");
    BubbleSort(arr, 5);
    print(arr, 5);
}