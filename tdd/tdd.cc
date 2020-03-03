#include "tdd.h"
#include <xio/utils/xstr.h>
#include <xio/utils/notification.h>
#include <xio/logbook/book.h>
#include <xio/lexer/lexer.h>




/*!

    @code

    int funtion()
    {
         logdebug dbug{};
    }

*/

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