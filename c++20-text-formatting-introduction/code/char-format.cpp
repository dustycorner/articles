#include <fmt/format.h>
#include <iostream>

using namespace std;
using namespace fmt;

int main()
{
    char c = 'a';

    cout << format("Default: {}\n", c);
    cout << format("Char type: {:c}\n", c);
    cout << format("Decimal type: {:d}\n", c);
    cout << format("Binary type: {0:b} {0:B} {0:#b} {0:#B}\n", c);
    cout << format("Octal type: {0:o} {0:#o}\n", c);
    cout << format("Hex type: {0:x} {0:X} {0:#x} {0:#X}\n", c);
}
