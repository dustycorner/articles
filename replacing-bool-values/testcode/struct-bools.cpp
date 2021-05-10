#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fmt/format.h>

struct OneFlag
{
    bool flag1;
};

struct TwoFlags
{
    bool flag1;
    bool flag2;
};

struct ThreeFlags
{
    bool flag1;
    bool flag2;
    bool flag3;
};

int oneflag(OneFlag f)
{
    int v1 = rand() % 64;
    return f.flag1 ? v1 : -v1;
}

int twoflag(TwoFlags f)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v = f.flag1 ? v1 : -v1;
    v = f.flag2 ? (v + v2) : (v - v2);
    return v;
}

int threeflag(ThreeFlags f)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v3 = rand() % 64;
    int v = f.flag1 ? v1 : -v1;
    v = f.flag2 ? (v + v2) : (v - v2);
    v = f.flag3 ? (v * v3) : (v * 2 * v3);
    return v;
}

int main()
{
    std::srand(1);
    int v = 0;

    auto begin = std::clock();

    for (int i = 0; i < 1'000; ++i)
    {
        v += oneflag({false});
        v += oneflag({true});

        v += twoflag({false,false});
        v += twoflag({false,true});
        v += twoflag({true,false});
        v += twoflag({true,true});

        v += threeflag({false,false,false});
        v += threeflag({false,false,true});
        v += threeflag({false,true,false});
        v += threeflag({false,true,true});
        v += threeflag({true,false,false});
        v += threeflag({true,false,true});
        v += threeflag({true,true,false});
        v += threeflag({true,true,true});
    }

    auto end = std::clock();
    auto time = 1000000.0 * (end - begin)/CLOCKS_PER_SEC;

    std::clog << fmt::format("{}\n", time);
    std::cout << fmt::format("v={} : Time={} us - struct-bools\n", v, time);
}

