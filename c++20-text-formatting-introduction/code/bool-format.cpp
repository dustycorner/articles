#include <fmt/format.h>
#include <iostream>

using namespace std;
using namespace fmt;

int main()
{
    cout << "      b B d o x X\n";
    cout << format("{0}  {0:b} {0:B} {0:d} {0:o} {0:x} {0:X}\n", true);
    cout << format("{0} {0:b} {0:B} {0:d} {0:o} {0:x} {0:X}\n", false);
    cout << "\nUsing alternate form\n";
    cout << "#b  #B  #d #o #x  #X\n";
    cout << format("{0:#b} {0:#B} {0:#d}  {0:#o} {0:#x} {0:#X}\n", true);
    cout << format("{0:#b} {0:#B} {0:#d}  {0:#o}  {0:#x} {0:#X}\n", false);
    cout << "\nUsing type=s\n";
    cout << format("|{:s}| |{:s}|\n", false, true);
    cout << "\nUsing type=c\n";
    cout << format("|{:c}| |{:c}|\n", false, true);
}
