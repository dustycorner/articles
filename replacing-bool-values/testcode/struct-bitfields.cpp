#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fmt/format.h>

struct OneFlag
{
    unsigned flag1 : 1;
};

struct TwoFlags
{
    unsigned flag1 : 1;
    unsigned flag2 : 1;
};

struct ThreeFlags
{
    unsigned flag1 : 1;
    unsigned flag2 : 1;
    unsigned flag3 : 1;
};

int oneflag(OneFlag f)
{
    int v1 = rand() % 64;
    return f.flag1 == 1 ? v1 : -v1;
}

int twoflag(TwoFlags f)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v = f.flag1 == 1 ? v1 : -v1;
    v = f.flag2 == 1 ? (v + v2) : (v - v2);
    return v;
}

int threeflag(ThreeFlags f)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v3 = rand() % 64;
    int v = f.flag1 == 1 ? v1 : -v1;
    v = f.flag2 == 1 ? (v + v2) : (v - v2);
    v = f.flag3 == 1 ? (v * v3) : (v * 2 * v3);
    return v;
}

int main()
{
    std::srand(1);
    int v = 0;

    auto begin = std::clock();

    for (int i = 0; i < 1'000; ++i)
    {
        v += oneflag({0});
        v += oneflag({1});

        v += twoflag({0,0});
        v += twoflag({0,1});
        v += twoflag({1,0});
        v += twoflag({1,1});

        v += threeflag({0,0,0});
        v += threeflag({0,0,1});
        v += threeflag({0,1,0});
        v += threeflag({0,1,1});
        v += threeflag({1,0,0});
        v += threeflag({1,0,1});
        v += threeflag({1,1,0});
        v += threeflag({1,1,1});
    }

    auto end = std::clock();
    auto time = 1000000.0 * (end - begin)/CLOCKS_PER_SEC;

    std::clog << fmt::format("{}\n", time);
    std::cout << fmt::format("v={} : Time={} us - struct-bitfields\n", v, time);
}
