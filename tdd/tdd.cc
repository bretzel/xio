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
    std::cout << "xio::logbook::document_element::tagname(xio::logbook::document_element::tag::div): " << xio::logbook::document_element::tagname(xio::logbook::document_element::tag::div)() << '\n';

    xio::logbook::document_element::shared e     = std::make_shared<xio::logbook::document_element>(xio::logbook::document_element::tag::head);
    xio::logbook::document_element::shared div   = std::make_shared<xio::logbook::document_element>(e,xio::logbook::document_element::tag::div);
    xio::logbook::document_element& head = div->parent();
    
    std::cout << "element e : " << e->text() << '\n';
    std::cout << "element head=div(e).parent(): " << head.text() << '\n';
    std::cout << "element div : " << div->text() << '\n';

    xio::utils::notification::clear(
        [](xio::utils::notification& n) 
        {
            std::cout << n() << '\n';
        }
    );

    return 0;
}