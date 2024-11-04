#include <iostream>
using namespace std;
#include <set>
#include <map>
int main(int argc, char const *argv[])
{
    set<string> ST;
    ST.insert("123");
    ST.insert("dog");
    for (auto i = ST.begin(); i != ST.end(); ++i)
    {
        cout << *i << " ";
    }
    cout << endl;
    cout << "-----------------------" << endl;

    map<string, string> ssmap;
    ssmap.insert({"he", "ok"});
    ssmap["you"] = "good man";
    ssmap.insert(make_pair("she", "nice"));
    try
    {
        auto value = ssmap.at("he");
        cout << "value:" << value << endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    cout << "-----------" << endl;
    auto it = ssmap.find("you");
    if (it != ssmap.end())
    {
        cout << "find ok结果:-->";
        cout << it->first << ":" << it->second << endl;
    }
    else
    {
        cout << "not find!" << endl;
    }
    return 0;
}
