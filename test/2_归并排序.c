#include <stdio.h>
void merge(int *a, int left, int right, int mid)
{
    int tmp[right - left + 1]; // 一个辅助数组，用来保存原数组的数据
    int L_left, R_left, k;
    for (k = left; k <= right; k++) // 先把要归并的区间保存到另一个数组
    {
        tmp[k - left] = a[k];
    }
    L_left = left;    // L_left是左边区域的最左边的下标
    R_left = mid + 1; // R_left是右边区域的最左边的下标
    for (k = left; k <= right; k++)
    {
        if (L_left > mid) // 左边的已经取完了，把右边的数据直接加到后面
        {
            a[k] = tmp[R_left - left];
            R_left++;
        }
        else if (R_left > right) // 右边的已经取完了，把左边的数据直接加到后面
        {
            a[k] = tmp[L_left - left];
            L_left++;
        }
        else if (tmp[R_left - left] < tmp[L_left - left]) // 谁小取谁
        {
            a[k] = tmp[R_left - left];
            R_left++;
        }
        else // 谁小取谁
        {
            a[k] = tmp[L_left - left];
            L_left++;
        }
    }
}

void merge_sort(int

                    *a,
                int left, int right)
{
    if (left >= right)
        return;
    int mid = (left + right) / 2;
    merge_sort(a, left, mid); // 这里会一直递归直分成1个
    merge_sort(a, mid + 1, right);
    merge(a, left, right, mid);
}

int main(int argc, char const *argv[])
{
    int a[10] = {2, 5, 1, 7, 4, 6, 9, 8, 3, 10};
    merge_sort(a, 0, 9);
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}
