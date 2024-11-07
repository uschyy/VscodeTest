#include <iostream>

using namespace std;

class animal
{
private:
public:
    virtual void move() = 0;
};

class finsh : public animal
{
private:
public:
    void move() override
    {
        cout << "fish swim" << endl;
    }
};

class kun : public animal
{
private:
public:
    void move() override
    {
        cout << "kun jump" << endl;
    }
};

void fun(animal &p)
{
    p.move();
}

int main(int argc, char const *argv[])
{
    finsh F;
    kun K;
    fun(F);
    fun(K);
    return 0;
}
