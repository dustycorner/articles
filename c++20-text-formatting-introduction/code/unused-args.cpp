#include <fmt/format.h>
#include <iostream>
#include <string>

using namespace std;
using namespace fmt;

void write_success(int warnings, int errors)
{
    string fmtspec = "Compilation ";
    if (errors == 0)
    {
        fmtspec += "succeeded";
        if (warnings != 0)
        {
           fmtspec += " with {0} Warning(s)";
        }
    }
    else
    {
        fmtspec += "failed with {1} Error(s)";
        if (warnings != 0)
        {
            fmtspec += " and {0} Warning(s)";
        }
    }
    fmtspec += "\n";
    cout << format(fmtspec, warnings, errors);
}

int main()
{
    write_success(0, 0);
    write_success(10, 0);
    write_success(0, 10);
    write_success(10, 10);
}
