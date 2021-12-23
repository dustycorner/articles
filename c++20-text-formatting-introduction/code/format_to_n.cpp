#include <fmt/format.h>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;
using namespace fmt;

string VecOut(const vector<int>& v)
{
    string retval;
    back_insert_iterator<string> out(retval);
    for (const auto& i: v)
    {
        auto res = format_to_n(out, 5, "{}", i);
        retval += ' ';
    }
    return retval;
}

int main()
{
    vector<int> v1{1, 100, 10000};
    cout << VecOut(v1) << "\n";
    vector<int> v2{1, 1000, 1000000};
    cout << VecOut(v2) << "\n";
}
