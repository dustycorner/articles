#include <fmt/format.h>
#include <iostream>
#include <cstdio>
#include <string>

using namespace std;
using namespace fmt;

int main()
{
    int i = 10;
    double f = 1.234;
    string s = "Hello World!";
    string fmt_str{"{} {} {}\n"};

    cout << "Length of formatted data: " << formatted_size(fmt_str, i, f, s) << "\n";
    cout << format(fmt_str, i, f, s);
    cout << "123456789|123456789|12\n";
}
