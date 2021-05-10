#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fmt/format.h>

constexpr int F1_True = 0x1;
constexpr int F2_True = 0x2;
constexpr int F3_True = 0x4;

int oneflag(int f)
{
    int v1 = rand() % 64;
    return (f & F1_True) == F1_True ? v1 : -v1;
}

int twoflag(int f)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v = (f & F1_True) == F1_True ? v1 : -v1;
    v = (f & F2_True) == F2_True ? (v + v2) : (v - v2);
    return v;
}

int threeflag(int f)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v3 = rand() % 64;
    int v = (f & F1_True) == F1_True ? v1 : -v1;
    v = (f & F2_True) == F2_True ? (v + v2) : (v - v2);
    v = (f & F3_True) == F3_True ? (v * v3) : (v * 2 * v3);
    return v;
}

int main()
{
    std::srand(1);
    int v = 0;

    auto begin = std::clock();

    for (int i = 0; i < 1'000; ++i)
    {
        v += oneflag(0);
        v += oneflag(F1_True);

        v += twoflag(0);
        v += twoflag(F2_True);
        v += twoflag(F1_True);
        v += twoflag(F1_True | F2_True);

        v += threeflag(0);
        v += threeflag(F3_True);
        v += threeflag(F2_True);
        v += threeflag(F2_True | F3_True);
        v += threeflag(F1_True);
        v += threeflag(F1_True | F3_True);
        v += threeflag(F1_True | F2_True);
        v += threeflag(F1_True | F2_True | F3_True);
    }

    auto end = std::clock();
    auto time = 1000000.0 * (end - begin)/CLOCKS_PER_SEC;

    std::clog << fmt::format("{}\n", time);
    std::cout << fmt::format("v={} : Time={} us - ints\n", v, time);
}
