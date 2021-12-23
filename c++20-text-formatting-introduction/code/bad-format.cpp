#include <fmt/format.h>
#include <iostream>

using namespace std;
using namespace fmt;

int main()
{
    int i = 10;

    try
    {
        cout << format("Using format: {:s}\n", i);
    }
    catch (const format_error& fe)
    {
        cout << "Caught format_error: " << fe.what() << "\n";
    }
}
