#include <iostream>
#include <list>
#include <vector>
using namespace std;

int main(int argc, char const *argv[])
{
    vector<int> arr{1, 2, 3, 4};
    cout << arr.size();
    arr.push_back(5);
    auto it1 = arr.begin();
    auto it2 = arr.end();
    cout << distance(it1, it2) << endl;

    return 0;
}
