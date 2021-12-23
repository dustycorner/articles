#include <fmt/format.h>
#include <iostream>

using namespace std;
using namespace fmt;

int main()
{
    int i1 = -10;
    int i2 = 10;

    cout << format("Default: {} {} {}\n", i1, 0, i2);
    cout << format("Decimal type: {:d} {:d} {:d}\n", i1, 0, i2);
    cout << format("Binary type: {0:b} {0:B} {1:b} {1:B} {2:b} {2:B}\n", i1, 0, i2);
    cout << format("Binary '#' : {0:#b} {0:#B} {1:#b} {1:#B} {2:#b} {2:#B}\n", i1, 0, i2);
    cout << format("Octal type: {0:o} {1:o} {2:o}\n", i1, 0, i2);
    cout << format("Octal '#' : {0:#o} {1:#o} {2:#o}\n", i1, 0, i2);
    cout << format("Hex type: {0:x} {0:X} {1:x} {1:X} {2:x} {2:X}\n", i1, 0, i2);
    cout << format("Hex '#' : {0:#x} {0:#X} {1:#x} {1:#X} {2:#x} {2:#X}\n", i1, 0, i2);
    cout << format("Char type: |{:c}| |{:c}|\n", 32, 126);
}
