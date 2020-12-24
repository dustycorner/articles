#include "common.ipp"
#include <memory>

int main()
{
    {
        std::cout << LINENO << "Creating with make_shared\n";
        std::shared_ptr<DataHolder> p2 = std::make_shared<DataHolder>();
        std::cout << LINENO << "Finished creating with make_shared, use_count=" << p2.use_count() << ", object @ " << std::hex << p2.get() << std::dec << "\n";
    }
    std::cout << LINENO << "Exiting program\n";
}
