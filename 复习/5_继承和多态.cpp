#include <iostream>
using namespace std;
class A
{
protected:
    int x;

public:
    A(int a) : x(a) {}
    void print_A()
    {
        cout << "x:" << x << endl;
    }
};
class B : virtual public A
{
protected:
    int y;

public:
    B(int a, int b) : y(a), A(b) {}
    void print_B()
    {
        cout << "y:" << y << "x:" << x << endl;
    }
    int pr(int x, int y)
    {
        cout << "pr--int" << endl;
    }
    float pr(float, float)
    {
        cout << "pr--float" << endl;
    }
};
class C : virtual public A
{
protected:
    int z;

public:
    C(int a, int b) : z(a), A(b) {}
    void print_C()
    {
        cout << "z:" << z << ",x:" << x << endl;
    }
};
class D : public B, public C
{
protected:
    int m;

public:
    D(int a, int b, int c) : m(c), B(a, b), C(a + 1, b + 1), A(a + 2) {}
    void print_D()
    {
        cout << "m:" << m << ",z:" << z << ",y:" << y << ",x:" << x << endl;
    }
};
int main()
{
    // D d(1, 2, 3);
    // d.print_D();
    B b(1, 2);
    b.pr(1, 2);
    b.pr((float)1.0, (float)3.0);
}

// class A
// {
// private:
//     /* data */
// public:
//     void print1() // 隐藏的动态多态，有一个this的指针   A* const this 始终指向的是print2这个地址，但是允许它里面更改
//     {
//         print2();
//     }

//     virtual void print2()
//     {
//         cout << "A" << endl;
//     }
// };

// class B : public A
// {
// private:
//     /* data */
// public:
//     void print2()
//     {
//         cout << "B" << endl;
//     }
// };

// int main(int argc, char const *argv[])
// {
//     A a;
//     B b;
//     a.print1(); // A
//     b.print1(); // B
//     return 0;
// }
