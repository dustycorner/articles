#include "common.ipp"
#include <memory>

int main()
{
    {
        std::cout << LINENO << "Creating from pointer\n";
        auto p1 = std::shared_ptr<DataHolder>{new DataHolder};
        std::cout << LINENO << "Finished creating from pointer, use_count=" << p1.use_count() << ", object @ " << std::hex << p1.get() << std::dec << "\n";
    }
    std::cout << LINENO << "Exiting program\n";
}
