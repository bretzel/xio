#include "tdd.h"
#include <xio/utils/xstr.h>
#include <xio/utils/notification.h>
#include <xio/logbook/book.h>
#include <xio/lexer/lexer.h>

//#include <any>



using xio::utils::expect;
using xio::utils::notification;

using xio::logbook::doc_element;


expect<notification::code> tdd_lexer(const std::string& a_src)
{
    xio::lexer::lexscanners lexer;
    xio::lexer::lexscanners::code code = lexer[a_src.c_str()];
    if(!code) return code.note();
    //...
    return notification::code::ok;
}



auto main() -> int {
    xio::utils::xstr str = "Hello, World!\n";
    std::cout << str();
    std::cout << "xio::logbook::document_element::tagname(xio::logbook::document_element::tag::div): " << xio::logbook::doc_element::tagname(xio::logbook::doc_element::tag::div)() << '\n';

    doc_element::shared e     = xio::logbook::book::create_element(nullptr,xio::logbook::doc_element::tag::head);
    doc_element::shared div   = xio::logbook::book::create_element(e,xio::logbook::doc_element::tag::div);
    doc_element& head         = div->parent();
    
    std::cout << "element e : " << e->text() << '\n';
    std::cout << "element head=div(e).parent(): " << head.text() << '\n';
    std::cout << "element div : " << div->text() << '\n';

    int v = 4040;

    expect<int> zyx = 45;
    expect<int> z2;

    std::cout << "xio::utils::expect::notice() (int v=4040; zyx = 45;):" <<  zyx.note()() << '\n';
    int& v2 = zyx.value();
    std::cout << "int& v2 = zyx::value() :" << v2 << '\n';
    std::cout << " Call z2.value() - Uninitialized :..." << z2.value() << "\n";

    expect<notification::code> cc = tdd_lexer("a=0;");
    if(cc)
    {
        std::cout << "lexer test returned ok\n";
        //...
    }
    xio::utils::notification::clear(
        [](xio::utils::notification& n) 
        {
            std::cout << n() << '\n';
        }
    );
    
    return 0;
}

