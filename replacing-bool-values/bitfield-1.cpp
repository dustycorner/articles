#include <iostream>

struct A
{
    int a1 : 1;
    int a2 : 1;
    int a3 : 1;
};

struct B
{
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
};

int main()
{
    A a; a.a1 = 1; a.a2 = 1; a.a3 = 1;
    std::cout << a.a1 << " " << a.a2 << " " << a.a3 << "\n";
    B b; b.b1 = 1; b.b2 = 1; b.b3 = 1;
    std::cout << b.b1 << " " << b.b2 << " " << b.b3 << "\n";
}
