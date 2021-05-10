#include <cstdlib>
#include <iostream>
#include <bitset>
#include <ctime>
#include <fmt/format.h>

using bitset1 = std::bitset<1>;
using bitset2 = std::bitset<2>;
using bitset3 = std::bitset<3>;

constexpr int F1Pos = 0;
constexpr int F2Pos = 1;
constexpr int F3Pos = 2;

int oneflag(bitset1 f)
{
    int v1 = rand() % 64;
    return f[F1Pos] ? v1 : -v1;
}

int twoflag(bitset2 f)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v = f[F1Pos] ? v1 : -v1;
    v = f[F2Pos] ? (v + v2) : (v - v2);
    return v;
}

int threeflag(bitset3 f)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v3 = rand() % 64;
    int v = f[F1Pos] ? v1 : -v1;
    v = f[F2Pos] ? (v + v2) : (v - v2);
    v = f[F3Pos] ? (v * v3) : (v * 2 * v3);
    return v;
}

int main()
{
    constexpr int F1_True = 0x1;
    constexpr int F2_True = 0x2;
    constexpr int F3_True = 0x4;

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
    std::cout << fmt::format("v={} : Time={} us - bitset-pos\n", v, time);
}
