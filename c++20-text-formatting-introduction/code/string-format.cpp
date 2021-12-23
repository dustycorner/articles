#include <fmt/format.h>
#include <iostream>
#include <string>

using namespace std;
using namespace fmt;

int main()
{
    string s = "Hello World!";
    const char* cp = "Testing. Testing.";
    const char* cp2 = "Goodbye World!";
    std::string_view sv = cp2;

    cout << format("Default: {} {} {}\n", s, cp, sv);
    cout << format("Type   : {:s} {:s} {:s}\n", s, cp, sv);

    cout << "\nUsing width and precision:\n";
    cout << format("With width: w=7:|{0:7s}| w=20:|{0:20s}|\n", s);
    cout << format("With precision: p=4:|{0:.4s}| p=15:|{0:.15s}|\n", s);
    cout << format("With width and precision: w=7,p=4:|{0:7.4s}| w=20,p=4:|{0:20.4s}|\n", s);
    cout << format("With width, precision, align: |{0:<8.4s}| |{0:^8.4s}| |{0:>8.4s}|\n", s);
}
