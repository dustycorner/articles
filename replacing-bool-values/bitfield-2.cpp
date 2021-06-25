#include <iostream>

struct BitFlags
{
    unsigned int flag1 : 1;
    unsigned int flag2 : 1;
    unsigned int flag3 : 1;
};

struct BoolFlags
{
    bool flag1;
    bool flag2;
    bool flag3;
};

void fbit(BitFlags flags)
{
    std::cout << flags.flag1 << " " << flags.flag2 << " " << flags.flag3 << "\n";
}

void fbool(BoolFlags flags)
{
    std::cout << std::boolalpha << flags.flag1 << " " << flags.flag2 << " " << flags.flag3 << "\n";
}

int main()
{
    BitFlags bitflags;
    bitflags.flag1 = 0;
    bitflags.flag2 = 1;
    bitflags.flag3 = 0;
    fbit(bitflags);
    fbit({1, 0, 1});

    BoolFlags boolflags;
    boolflags.flag1 = false;
    boolflags.flag2 = true;
    boolflags.flag3 = false;
    fbool(boolflags);
    fbool({true, false, true});

}
