#include <iostream>
#include <string_view>

void PrintArgs(const std::string& s, bool is_blue = false)
{
    std::cout << "Called PrintArgs(string, bool) with (" << s << ", " << is_blue << ")\n";
}

void PrintArgs(const std::string& s, short len, bool is_blue = false)
{
    std::cout << "Called PrintArgs(string, short, bool) with (" << s << ", " << len << ", " << is_blue << ")\n";
}

int main()
{
    std::cout << std::boolalpha;
    PrintArgs("Abc");          // 1
    PrintArgs("Abc", true);    // 2
    PrintArgs("Abc", 2);       // 3
    PrintArgs("Abc", 2, true); // 4
}
