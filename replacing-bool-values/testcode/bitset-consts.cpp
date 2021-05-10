#include <cstdlib>
#include <iostream>
#include <bitset>
#include <ctime>
#include <fmt/format.h>

using bitset1 = std::bitset<1>;
using bitset2 = std::bitset<2>;
using bitset3 = std::bitset<3>;

constexpr bitset1 B1F1_True = 0x1;
constexpr bitset2 B2F1_True = 0x1;
constexpr bitset2 B2F2_True = 0x2;
constexpr bitset3 B3F1_True = 0x1;
constexpr bitset3 B3F2_True = 0x2;
constexpr bitset3 B3F3_True = 0x4;

int oneflag(bitset1 f)
{
    int v = rand() % 64;
    return (f & B1F1_True) == B1F1_True ? v : -v;
}

int twoflag(bitset2 f)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v = (f & B2F1_True) == B2F1_True ? v1 : -v1;
    v = (f & B2F2_True) == B2F2_True ? (v + v2) : (v - v2);
    return v;
}

int threeflag(bitset3 f)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v3 = rand() % 64;
    int v = (f & B3F1_True) == B3F1_True ? v1 : -v1;
    v = (f & B3F2_True) == B3F2_True ? (v + v2) : (v - v2);
    v = (f & B3F3_True) == B3F3_True ? (v * v3) : (v * 2 * v3);
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
        v += oneflag(B1F1_True);

        v += twoflag(0);
        v += twoflag(B2F2_True);
        v += twoflag(B2F1_True);
        v += twoflag(B2F1_True | B2F2_True);

        v += threeflag(0);
        v += threeflag(B3F3_True);
        v += threeflag(B3F2_True);
        v += threeflag(B3F2_True | B3F3_True);
        v += threeflag(B3F1_True);
        v += threeflag(B3F1_True | B3F3_True);
        v += threeflag(B3F1_True | B3F2_True);
        v += threeflag(B3F1_True | B3F2_True | B3F3_True);
    }

    auto end = std::clock();
    auto time = 1000000.0 * (end - begin)/CLOCKS_PER_SEC;

    std::clog << fmt::format("{}\n", time);
    std::cout << fmt::format("v={} : Time={} us - bitset-consts\n", v, time);
}
