#include <fmt/format.h>
#include <iostream>
#include <string>

using namespace std;
using namespace fmt;

int main()
{
    int ineg = -10;
    int ipos = 5;
    double fneg = -1.2;
    double fpos = 2.34;

    cout << format("With sign '+' :|{:+}|{:+}|{:+}|{:+}|\n", ineg, fneg, ipos, fpos);
    cout << format("With sign '-' :|{:-}|{:-}|{:-}|{:-}|\n", ineg, fneg, ipos, fpos);
    cout << format("With sign ' ' :|{: }|{: }|{: }|{: }|\n", ineg, fneg, ipos, fpos);

    cout << format("With sign '+' and '0' :|{:+06}|{:+06}|{:+06}|{:+06}|\n", ineg, fneg, ipos, fpos);
    cout << format("With sign '-' and '0' :|{:-06}|{:-06}|{:-06}|{:-06}|\n", ineg, fneg, ipos, fpos);
    cout << format("With sign ' ' and '0' :|{: 06}|{: 06}|{: 06}|{: 06}|\n", ineg, fneg, ipos, fpos);
}
