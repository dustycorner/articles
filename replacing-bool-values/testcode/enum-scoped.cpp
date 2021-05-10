#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fmt/format.h>

enum class First { False, True };
enum class Second { False, True };
enum class Third { False, True };

int oneflag(First f1)
{
    int v1 = rand() % 64;
    return f1 == First::True ? v1 : -v1;
}

int twoflag(First f1, Second f2)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v = f1 == First::True ? v1 : -v1;
    v = f2 == Second::True ? (v + v2) : (v - v2);
    return v;
}

int threeflag(First f1, Second f2, Third f3)
{
    int v1 = rand() % 64;
    int v2 = rand() % 64;
    int v3 = rand() % 64;
    int v = f1 == First::True ? v1 : -v1;
    v = f2 == Second::True ? (v + v2) : (v - v2);
    v = f3 == Third::True ? (v * v3) : (v * 2 * v3);
    return v;
}

int main()
{
    std::srand(1);
    int v = 0;

    auto begin = std::clock();

    for (int i = 0; i < 1'000; ++i)
    {
        v += oneflag(First::False);
        v += oneflag(First::True);

        v += twoflag(First::False, Second::False);
        v += twoflag(First::False, Second::True);
        v += twoflag(First::True, Second::False);
        v += twoflag(First::True, Second::True);

        v += threeflag(First::False, Second::False, Third::False);
        v += threeflag(First::False, Second::False, Third::True);
        v += threeflag(First::False, Second::True, Third::False);
        v += threeflag(First::False, Second::True, Third::True);
        v += threeflag(First::True, Second::False, Third::False);
        v += threeflag(First::True, Second::False, Third::True);
        v += threeflag(First::True, Second::True, Third::False);
        v += threeflag(First::True, Second::True, Third::True);
    }

    auto end = std::clock();
    auto time = 1000000.0 * (end - begin)/CLOCKS_PER_SEC;

    std::clog << fmt::format("{}\n", time);
    std::cout << fmt::format("v={} : Time={} us - enum-scoped\n", v, time);
}
