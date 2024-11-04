#include <iostream>
using namespace std;
#include <list>
#include <queue>
int main(int argc, char const *argv[])
{
    list<char *> L{"123", "456", "789"};
    for (list<char *>::iterator i = L.begin(); i != L.end(); ++i)
    {
        cout << *i << endl;
    }

    return 0;
}
