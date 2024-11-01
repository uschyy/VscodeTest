#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <list>
using namespace std;
void print_list(const list<int> &arr)
{
    for (int i : arr)
    {
        cout << i << " ";
    }
}
int main(int argc, char const *argv[])
{
    // 动态数组
    // 正向迭代器
    vector<int> arr{1, 2, 3, 4, 5};
    for (vector<int>::iterator i = arr.begin(); i != arr.end(); ++i)
    {
        cout << *i << " ";
    }
    cout << endl;
    for (int i = 0; i < arr.size(); i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
    // ===================

    // 常量正向迭代器
    for (vector<int>::const_iterator i = arr.cbegin(); i != arr.cend(); ++i)
    {
        cout << *i << " ";
    }
    cout << endl;
    // ====================

    // 反向迭代器
    vector<int>::reverse_iterator it = arr.rbegin();
    for (it; it != arr.rend(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;
    // =======================

    // 常量反向迭代器
    for (vector<int>::const_reverse_iterator i = arr.crbegin(); i != arr.crend(); ++i)
    {
        cout << *i << " ";
    }
    cout << "=====" << endl;

    // 链表
    list<int> a{1, 2, 3, 4, 5, 6};
    // a.erase(a); // 擦除第一个元素
    print_list(a);
    cout << endl;
    a.push_front(18); // 增加首元素
    a.push_back(17);  // 增加尾元素
    for (list<int>::iterator i = a.begin(); i != a.end(); ++i)
    {

        if ((*i) % 2 == 0)
        {
            cout << *i << " ";
            i = a.erase(i);
        }
    }
    cout << endl;
    print_list(a);
    cout << "首元素" << a.front() << endl;

    cout << "尾元素" << a.back() << endl;
    cout << "链表长度" << a.size() << endl;

    cout << endl;

    return 0;
}
