#include <fmt/format.h>
#include <iostream>
#include <string>

using namespace std;
using namespace fmt;

void vlog_error(int code, std::string_view fmt, format_args args)
{
    cout << "Error " << code << ": " << vformat(fmt, args) << "\n";
}

template<class... Args>
void log_error(int code, std::string_view fmt, const Args&... args)
{
    vlog_error(code, fmt, make_format_args(args...));
}

int main()
{
    int i = 10;
    double f = 1.234;
    string s = "Hello World!";

    log_error(1, "Bad input detected: {} is not an integer value", 10.1);
    log_error(10, "Oops - Type mismatch between {} and {}", "var1", 10); 
    log_error(255, "Something went wrong!");
}
