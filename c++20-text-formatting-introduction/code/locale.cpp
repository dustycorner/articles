#include <fmt/format.h>
#include <iostream>
#include <cstdio>
#include <string>

using namespace std;
using namespace fmt;

int main()
{
    double dval = 1.5;
    int ival = 1'000'000;

    cout << "Using default locale:\n";
    cout << format("format  : {:.2f} {:12d}\n", dval, ival);
    cout << format("format+L: {:.2Lf} {:12Ld}\n", dval, ival);

    cout << "\nUsing global locale de_DE:\n";
    locale::global(locale("de_DE"));
    cout << format("format  : {:.2f} {:12d}\n", dval, ival);
    cout << format("format+L: {:.2Lf} {:12Ld}\n", dval, ival);

    cout << "\nUsing function-specific locale:\n";
    cout << format(locale("en_US"), "en_US: {0:.2f} {0:.2Lf} {1:12d} {1:12Ld}\n", dval, ival);
    cout << format(locale("de_DE"), "de_DE: {0:.2f} {0:.2Lf} {1:12d} {1:12Ld}\n", dval, ival);
}
