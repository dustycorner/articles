#include <fmt/format.h>
#include <iostream>
#include <string>

using namespace std;
using namespace fmt;

int main()
{
    int i = 10;

    printf("%d %o %x\n", i, i, i);
    cout << i << " " << std::oct << i << " " << std::hex << i << std::dec << "\n";
    cout << format("{0} {0:o} {0:x}\n", i);
}
