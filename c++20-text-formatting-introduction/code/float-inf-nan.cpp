#include <fmt/format.h>
#include <limits>
#include <iostream>

using namespace std;
using namespace fmt;

int main()
{
    auto pinf = std::numeric_limits<double>::infinity();
    auto ninf = -std::numeric_limits<double>::infinity();
    auto pnan = std::numeric_limits<double>::quiet_NaN();
    auto nnan = -std::numeric_limits<double>::quiet_NaN();

    cout << "Default:\n";
    cout << format("Default: {} {} {} {}\n", ninf, pinf, nnan, pnan);
    cout << format("Type f : {:f} {:f} {:f} {:f}\n", ninf, pinf, nnan, pnan);
    cout << format("Type F : {:F} {:F} {:F} {:F}\n", ninf, pinf, nnan, pnan);
    cout << "\nAlternate form:\n";
    cout << format("Default: {:#} {:#} {:#} {:#}\n", ninf, pinf, nnan, pnan);
    cout << format("Type f : {:#f} {:#f} {:#f} {:#f}\n", ninf, pinf, nnan, pnan);
    cout << format("Type F : {:#F} {:#F} {:#F} {:#F}\n", ninf, pinf, nnan, pnan);
    cout << "\nWidth=7:\n";
    cout << format("Default: |{:7}| |{:7}| |{:7}| |{:7}|\n", ninf, pinf, nnan, pnan);
    cout << "\nWidth=7, using '0':\n";
    cout << format("Default: |{:07}| |{:07}| |{:07}| |{:07}|\n", ninf, pinf, nnan, pnan);
}
