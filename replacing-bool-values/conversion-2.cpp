#include <iostream>
#include <string_view>

void PrintArgs(const std::string& s, bool is_blue = false)
{
    std::cout << "Called PrintArgs(string, bool) with (" << s << ", " << is_blue << ")\n";
}

void PrintArgs(const std::string& s, bool is_blue, short len)
{
    std::cout << "Called PrintArgs(string, bool, short) with (" << s << ", " << is_blue << ", " << len << ")\n";
}

int main()
{
    std::cout << std::boolalpha;
    PrintArgs("Abc");          // 1
    PrintArgs("Abc", true);    // 2
    PrintArgs("Abc", 2);       // 3
    PrintArgs("Abc", 2, true); // 4
}
