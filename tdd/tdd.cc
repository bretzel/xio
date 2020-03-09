#include "tdd.h"
#include <xio/utils/xstr.h>
#include <xio/utils/notification.h>
#include <xio/logbook/book.h>
#include <xio/lexer/lexer.h>

#include <any>


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
    std::cout << "xio::logbook::document_element::tagname(xio::logbook::document_element::tag::div): " << xio::logbook::doc_element::tagname(xio::logbook::doc_element::tag::div)() << '\n';

    //xio::logbook::doc_element::shared e     = std::make_shared<xio::logbook::doc_element>(xio::logbook::doc_element::tag::head);
    xio::logbook::doc_element::shared e     = xio::logbook::book::create_element(nullptr,xio::logbook::doc_element::tag::head);
    //xio::logbook::doc_element::shared div   = std::make_shared<xio::logbook::doc_element>(e,xio::logbook::doc_element::tag::div);
    xio::logbook::doc_element::shared div   = xio::logbook::book::create_element(e,xio::logbook::doc_element::tag::div);
    xio::logbook::doc_element& head = div->parent();
    
    std::cout << "element e : " << e->text() << '\n';
    std::cout << "element head=div(e).parent(): " << head.text() << '\n';
    std::cout << "element div : " << div->text() << '\n';

    int v = 4040;
    using xio::utils::expect;

    expect<int> zyx = 45;
    expect<int> z2;

    std::cout << "xio::utils::expect::notice() (int v=4040; zyx = 45;):" <<  zyx.note()() << '\n';
    int& v2 = zyx.value();
    std::cout << "int& v2 = zyx::value() :" << v2 << '\n';
    std::cout << "z2.value()..." << z2.value() << "\n";

    xio::utils::notification::clear(
        [](xio::utils::notification& n) 
        {
            std::cout << n() << '\n';
        }
    );
    
    return 0;
}

