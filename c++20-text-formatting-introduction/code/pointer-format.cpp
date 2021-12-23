#include <fmt/format.h>
#include <iostream>
#include <memory>

using namespace std;
using namespace fmt;

int main()
{
    int* pi = new int;
    void* vpi = static_cast<void*>(pi);
    double* pd = new double;
    void* vpd = static_cast<void*>(pd);
    void* pnull = nullptr;

    cout << format("Default: |{}| |{}| |{}|\n", vpi, vpd, pnull);
    cout << format("Type   : |{:p}| |{:p}| |{:p}|\n", vpi, vpd, pnull);
    cout << format("Width  : |{:20p}| |{:20p}| |{:20p}|\n", vpi, vpd, pnull);
}
