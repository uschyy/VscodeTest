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
    插入排序:
        给一组数据，先对前一个元素进行排序（就是第一个数），
        然后对前两个数进行排序，再对前三个数进行排序
        。。。。一直到排完为止。
*/

void InsertSort(int *arr, int count)
{
    int tmp, i, j;
    for (i = 1; i < count; i++)
    {
        tmp = arr[i];
        for (j = i - 1; j >= 0; j--)
        {
            if (arr[j] > tmp) // 第二个数比第一个数大
            {
                arr[j + 1] = arr[j];
            }
            else
                break;
        }
        arr[j + 1] = tmp;
    }
}
int main(int argc, char const *argv[])
{
    int arr[5] = {5, 4, 3, 2, 1};
    // 插入排序
    printf("插入排序:");
    InsertSort(arr, 5);
    print(arr, 5);
    printf("-------------\n");

    return 0;
}
