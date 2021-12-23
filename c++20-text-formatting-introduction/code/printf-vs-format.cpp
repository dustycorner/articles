#include <fmt/format.h>
#include <iostream>
#include <cstdio>

using namespace std;
using namespace fmt;

void printf_output()
{
    //  Integer output
    int iv = -1; short sv = -2; long lv = -3; long long llv = -4;
    printf("%i, %hi, %li, %lli\n", iv, sv, lv, llv);
    unsigned int uiv = 1; unsigned short usv = 2; unsigned long ulv = 3; unsigned long long ullv = 4;
    printf("%u, %hu, %lu, %llu\n", uiv, usv, ulv, ullv);

    //  Floating-point output
    float f = 1.234; double d = 2.345; long double ld = 3.456;
    printf("%g %g %Lg\n", f, d, ld);

    //  Character output
    char c = 'A';
    printf("%c %hhd\n", c, c);

    // String output
    const char* cptr = "Mary had a little lamb";
    std::string str(cptr);
    std::string_view strview(str);
    printf("%s\n", cptr);
    printf("%s\n", str.c_str());
    // printf cannot handle std::string_view

    // Pointer output
    printf("%p\n", static_cast<const void*>(cptr));

    //  Field Alignment
    printf("[%-10d] [%10d]\n", iv, iv);
    // printf doesn't support centred alignment

    //  Sign Field
    printf("[%+d] [% d] [%+d] [% d]\n", -1, -1, 1, 1);
    // printf does not support '-' as a sign specifier

    // Run-time width and precision
    int width=10, prec=5;
    double val=123.456;
    printf("%*.*f\n", width, prec, val);

    //  Positional Parameters
    float fv = 123.456;
    printf("%1$*2$.*3$f\n", fv, 10, 5);
}

void format_output()
{
    //  Integer output
    int iv = -1; short sv = -2; long lv = -3; long long llv = -4;
    cout << format("{}, {}, {}, {}\n", iv, sv, lv, llv);
    unsigned int uiv = 1; unsigned short usv = 2; unsigned long ulv = 3; unsigned long long ullv = 4;
    cout << format("{}, {}, {}, {}\n", uiv, usv, ulv, ullv);

    //  Floating-point output
    float f = 1.234; double d = 2.345; long double ld = 3.456;
    cout << format("{} {} {}\n", f, d, ld);

    //  Character output
    char c = 'A';
    cout << format("{} {:d}\n", c, c);

    // String output
    const char* cptr = "Mary had a little lamb";
    std::string str(cptr);
    std::string_view strview(str);
    cout << format("{}\n", cptr);
    cout << format("{}\n", str);
    cout << format("{}\n", strview);

    // Pointer output
    cout << format("{}\n", static_cast<const void*>(cptr));

    //  Field Alignment
    cout << format("[{:<10}] [{:>10}]\n", iv, iv);
    cout << format("[{:^10}]\n", iv);

    //  Sign Field
    cout << format("[{:+}] [{: }] [{:+}] [{: }]\n", -1, -1, 1, 1);
    cout << format("[{:-}] [{:-}]\n", -1, 1);

    // Run-time width and precision
    int width=10, prec=5;
    double val=123.456;
    cout << format("{:{}.{}f}\n", val, width, prec);

    //  Positional Parameters
    float fv = 123.456;
    cout << format("{0:{1}.{2}f}\n", fv, 10, 5);
}

int main()
{
    printf_output();
    std::cout << "\n";
    format_output();
}

