#include <iostream>
using namespace std;
// static_cast ---explicit
class A
{
    int x;

public:
    explicit A(int n) : x(n) {}   // 转换构造函数
    explicit operator int() const // 类型转换函数
    {
        return x;
    }
    void print_x()
    {
        cout << x << endl;
    }
};

int main(int argc, char const *argv[])
{
    A a(6);
    // --------------
    // a = 7; // 隐式类型转换--int转A
    // a = static_cast<A>(7); // explict禁用隐式类型转换，用static_cast显示声明
    // ----------

    // cout << a + 7 << endl; // 有A类型转换成int类型的隐式转换
    cout << static_cast<int>(a) + 7 << endl;
    a.print_x();
    return 0;
}
