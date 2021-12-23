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
        out = format_to(out, "{} ", i);
    }
    return retval;
}

int main()
{
    vector<int> v1{2, 3, 5};
    cout << VecOut(v1) << "\n";
    vector<int> v2{1, 2, 4, 8, 16, 32};
    cout << VecOut(v2) << "\n";
    vector<int> v3{1, 4, 9, 16, 25, 36, 49, 64, 81, 100};
    cout << VecOut(v3) << "\n";
}
