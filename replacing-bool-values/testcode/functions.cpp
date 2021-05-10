#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fmt/format.h>

template<bool b1>
int oneflag()
{
    int v1 = rand() % 64;
    return b1 ? v1 : -v1;
}

template<bool b1, bool b2>
int twoflags()
{
    int v1 = oneflag<b1>();
    int v2 = rand() % 64;
    return b2 ? v1 + v2 : v1 - v2;
}

template<bool b1, bool b2, bool b3>
int threeflags()
{
    int v1 = twoflags<b1, b2>();
    int v2 = rand() % 64;
    return b3 ? v1 * v2 : v1 * (2*v2);
}

int main()
{
    std::srand(1);
    int v = 0;

    auto begin = std::clock();

    for (int i = 0; i < 1'000; ++i)
    {
        v += oneflag<false>();
        v += oneflag<true>();

        v += twoflags<false,false>();
        v += twoflags<false,true>();
        v += twoflags<true,false>();
        v += twoflags<true,true>();

        v += threeflags<false,false,false>();
        v += threeflags<false,false,true>();
        v += threeflags<false,true,false>();
        v += threeflags<false,true,true>();
        v += threeflags<true,false,false>();
        v += threeflags<true,false,true>();
        v += threeflags<true,true,false>();
        v += threeflags<true,true,true>();
    }

    auto end = std::clock();
    auto time = 1000000.0 * (end - begin)/CLOCKS_PER_SEC;

    std::clog << fmt::format("{}\n", time);
    std::cout << fmt::format("v={} : Time={} us - functions\n", v, time);
}
