#include <fmt/format.h>
#include <iostream>

using namespace std;
using namespace fmt;

int main()
{
    int v1 = 10;
    int v2 = 10'000'000;

    cout << format("Specified width: |{0:4}| |{0:12}| |{1:4}| |{1:12}|\n", v1, v2);
    cout << format("Variable width, automatic numbering: |{:{}}| |{:{}}|\n", v1, 5, v2, 12);
    for (int len = 7; len < 11; ++len)
    {
        cout << format("Variable width={0:>2}, manual numbering: |{1:{0}}| |{2:{0}}|\n", len, v1, v2);
    }
}

