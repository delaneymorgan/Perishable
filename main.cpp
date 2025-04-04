
#include "Perishable.h"

int main()
{
    Perishable<float> val( boost::chrono::milliseconds( 500 ) , 1000 );
    std::cout << val.Get() << std::endl;
    sleep( 1 );
    std::cout << val.Get() << std::endl;
    val.Set( 2000 );
    std::cout << val.Get() << std::endl;
    sleep( 1 );
    std::cout << val.Get() << std::endl;
    return 0;
}

