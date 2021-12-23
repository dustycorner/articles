#include <fmt/format.h>
#include <iostream>
#include <string>

using namespace std;
using namespace fmt;

int main()
{
    string str;
    cout << "No fill character specified:\n";
    str = format("|{:^10}| |{:<10}| |{:^10}| |{:>10}|\n", "default", "left", "centre", "right");
    cout << str;
    string fmtstr = "|{0:10}| |{0:<10}| |{0:^10}| |{0:>10}|\n";
    str = format(fmtstr, 123);
    cout << str;
    str = format(fmtstr, 1.23);
    cout << str;
    str = format(fmtstr, "abcde");
    cout << str;

    cout << "\nFill character set to '*'\n";
    str = format("|{:*<10}| |{:*^10}| |{:*>10}|\n", "left", "centre", "right");
    cout << str;
    fmtstr = "|{0:*<10}| |{0:*^10}| |{0:*>10}|\n";
    str = format(fmtstr, 123);
    cout << str;
    str = format(fmtstr, 1.23);
    cout << str;
    str = format(fmtstr, "abcde");
    cout << str;
}
