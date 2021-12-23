#include <fmt/format.h>
#include <iostream>

using namespace std;
using namespace fmt;

int main()
{
    double small = 123.4567;
    double nodps = 34567.;
    double large = 1e10+12.345;
    double huge = 1e20;

    cout << "Default precision:\n";
    cout << format("Default: {} {} {} {} {}\n", 0.0, small, nodps, large, huge);
    cout << format("Type f : {:f} {:f} {:f} {:f} {:f}\n", 0.0, small, nodps, large, huge);
    cout << format("Type e : {:e} {:e} {:e} {:e} {:e}\n", 0.0, small, nodps, large, huge);
    cout << format("Type g : {:g} {:g} {:g} {:g} {:g}\n", 0.0, small, nodps, large, huge);
    cout << format("Type a : {:a} {:a} {:a} {:a} {:a}\n", 0.0, small, nodps, large, huge);
    cout << "\nAlternate form:\n";
    cout << format("Default: {:#} {:#} {:#} {:#} {:#}\n", 0.0, small, nodps, large, huge);
    cout << format("Type f : {:#f} {:#f} {:#f} {:#f} {:#f}\n", 0.0, small, nodps, large, huge);
    cout << format("Type e : {:#e} {:#e} {:#e} {:#e} {:#e}\n", 0.0, small, nodps, large, huge);
    cout << format("Type g : {:#g} {:#g} {:#g} {:#g} {:#g}\n", 0.0, small, nodps, large, huge);
    cout << format("Type a : {:#a} {:#a} {:#a} {:#a} {:#a}\n", 0.0, small, nodps, large, huge);
    cout << "\nPrecision=3:\n";
    cout << format("Default: {:.3} {:.3} {:.3} {:.3} {:.3}\n", 0.0, small, nodps, large, huge);
    cout << format("Type f : {:.3f} {:.3f} {:.3f} {:.3f} {:.3f}\n", 0.0, small, nodps, large, huge);
    cout << format("Type e : {:.3e} {:.3e} {:.3e} {:.3e} {:.3e}\n", 0.0, small, nodps, large, huge);
    cout << format("Type g : {:.3g} {:.3g} {:.3g} {:.3g} {:.3g}\n", 0.0, small, nodps, large, huge);
    cout << format("Type a : {:.3a} {:.3a} {:.3a} {:.3a} {:.3a}\n", 0.0, small, nodps, large, huge);
    cout << "\nPrecision=3, alternate form:\n";
    cout << format("Default: {:#.3} {:#.3} {:#.3} {:#.3} {:#.3}\n", 0.0, small, nodps, large, huge);
    cout << format("Type f : {:#.3f} {:#.3f} {:#.3f} {:#.3f} {:#.3f}\n", 0.0, small, nodps, large, huge);
    cout << format("Type e : {:#.3e} {:#.3e} {:#.3e} {:#.3e} {:#.3e}\n", 0.0, small, nodps, large, huge);
    cout << format("Type g : {:#.3g} {:#.3g} {:#.3g} {:#.3g} {:#.3g}\n", 0.0, small, nodps, large, huge);
    cout << format("Type a : {:#.3a} {:#.3a} {:#.3a} {:#.3a} {:#.3a}\n", 0.0, small, nodps, large, huge);
}
