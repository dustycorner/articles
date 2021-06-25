#include <iostream>
#include <string_view>

enum class RedBlue { Red, Blue };
std::ostream& operator<<(std::ostream& ostr, const RedBlue conv)
{
    ostr << (conv == RedBlue::Red ? "Red" : "Blue");
    return ostr;
}

void PrintArgs(const std::string& s, RedBlue to_uc = RedBlue::Red)
{
    std::cout << "Called PrintArgs(string, RedBlue) with (" << s << ", " << to_uc << ")\n";
}

void PrintArgs(const std::string& s, short len, RedBlue to_uc = RedBlue::Red)
{
    std::cout << "Called PrintArgs(string, short, RedBlue) with (" << s << ", " << len << ", " << to_uc << ")\n";
}

int main()
{
    std::cout << std::boolalpha;
    PrintArgs("Abc");                           // 1
    PrintArgs("Abc", RedBlue::Blue);    // 2
    PrintArgs("Abc", 2);                        // 3
    PrintArgs("Abc", 2, RedBlue::Blue); // 4
}
