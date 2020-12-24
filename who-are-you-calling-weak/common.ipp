#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <new>

int lineno=0;
#define LINENO std::setw(2) << ++lineno << ": "

void* operator new(std::size_t sz)
{
    auto ptr = std::malloc(sz + sizeof(std::size_t));
    std::size_t* iptr = static_cast<std::size_t*>(ptr);
    *iptr = sz;
    ++iptr;
    ptr = static_cast<void*>(iptr);
    std::cout << LINENO << "Allocated " << sz << " bytes at address 0x" << std::hex << ptr << std::dec << "\n";
    return ptr;
}

void operator delete(void *ptr) noexcept
{
    std::size_t* iptr = static_cast<std::size_t*>(ptr);
    --iptr;
    std::cout << LINENO << "Deallocating " << *iptr << " bytes at address 0x" << std::hex << ptr << std::dec << "\n";
    ptr = static_cast<void*>(iptr);
    std::free(ptr);
}

struct DataHolder
{
    DataHolder()
    : num(++dh)
    {
        std::cout << LINENO << "Constructing DataHolder " << num << "\n";
    }
    ~DataHolder()
    {
        std::cout << LINENO << "Destroying DataHolder " << num << "\n";
    }

    int num;
    int i[20];
    static int dh;
};
int DataHolder::dh=0;

