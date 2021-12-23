#include <fmt/format.h>
#include <iostream>
#include <cstdio>
#include <string>

using namespace std;
using namespace fmt;

int main()
{
    int i = 10;
    double f = 1.234;
    string s = "Hello World!";

    printf("Using printf   : %d %g %s\n", i, f, s.c_str());
    cout << "Using iostreams: " << i << " " << f << " " << s << "\n";
    cout << format("Using format   : {} {} {}\n", i, f, s);
}
