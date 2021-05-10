#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fmt/format.h>

enum First { First_False, First_True };
enum Second { Second_False, Second_True };
enum Third { Third_False, Third_True };

int oneflag(First f1)
{
    int v1 = rand() % 64;
    return f1 == First_True ? v1 : -v1;
}

int twoflag(First f1, Second f2)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v = f1 == First_True ? v1 : -v1;
    v = f2 == Second_True ? (v + v2) : (v - v2);
    return v;
}

int threeflag(First f1, Second f2, Third f3)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v3 = rand() % 64;
    int v = f1 == First_True ? v1 : -v1;
    v = f2 == Second_True ? (v + v2) : (v - v2);
    v = f3 == Third_True ? (v * v3) : (v * 2 * v3);
    return v;
}

int main()
{
    std::srand(1);
    int v = 0;

    auto begin = std::clock();

    for (int i = 0; i < 1'000; ++i)
    {
        v += oneflag(First_False);
        v += oneflag(First_True);

        v += twoflag(First_False, Second_False);
        v += twoflag(First_False, Second_True);
        v += twoflag(First_True, Second_False);
        v += twoflag(First_True, Second_True);

        v += threeflag(First_False, Second_False, Third_False);
        v += threeflag(First_False, Second_False, Third_True);
        v += threeflag(First_False, Second_True, Third_False);
        v += threeflag(First_False, Second_True, Third_True);
        v += threeflag(First_True, Second_False, Third_False);
        v += threeflag(First_True, Second_False, Third_True);
        v += threeflag(First_True, Second_True, Third_False);
        v += threeflag(First_True, Second_True, Third_True);
    }

    auto end = std::clock();
    auto time = 1000000.0 * (end - begin)/CLOCKS_PER_SEC;

    std::clog << fmt::format("{}\n", time);
    std::cout << fmt::format("v={} : Time={} us - enum-unscoped\n", v, time);
}
