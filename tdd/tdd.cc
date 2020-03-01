#include "tdd.h"
#include <xio/utils/xstr.h>
#include <xio/utils/notification.h>
#include <xio/logbook/logbook.h>



auto main() -> int {
    xio::utils::xstr str = "Hello, World!\n";
    std::cout << str();
    
    
    
    

    xio::utils::notification::clear(
        [](xio::utils::notification& n) 
        {
            std::cout << n() << '\n';
        }
    );

    return 0;
}