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


template<class T> class xyz
{
    std::any _a;
    bool _f = false;
public:

    xyz() = default;

    xyz(T& _t)
    {
        
        _a = _t;
        _f = true;
    }

    xyz(const T& _t)
    {
        _a = _t;
        _f = true;
    }
    xyz(T&& _t)
    {
        _a = std::move(_t);
        _f = true;
    }

    xyz& operator=(T& _t)
    {
        if (_a.has_value())
            _a.reset();
        _a = _t;
        _f = true;
        return *this;
    }

    xyz& operator=(T&& _t)
    {
        if (_a.has_value())
            _a.reset();
        _a = std::move(_t);
        _f = true;
        return *this;
    }

    xyz& operator=(const T& _t)
    {
        if (_a.has_value())
            _a.reset();
        _a = _t;
        _f = true;
        return *this;
    }


    auto& value()
    {
        if (!_f)
            _a = T();
        
        return std::any_cast<T&>(_a);
    }

    auto& notice()
    {
        if (_f)
            return xio::utils::notification::push(), " xyz::notice requires a false xyz state.";
        return std::any_cast<xio::utils::notification&>(_a);
    }
    void reset() { _a.reset(); }

};


auto main() -> int {
    xio::utils::xstr str = "Hello, World!\n";
    std::cout << str();
    std::cout << "xio::logbook::document_element::tagname(xio::logbook::document_element::tag::div): " << xio::logbook::doc_element::tagname(xio::logbook::doc_element::tag::div)() << '\n';

    xio::logbook::doc_element::shared e     = std::make_shared<xio::logbook::doc_element>(xio::logbook::doc_element::tag::head);
    xio::logbook::doc_element::shared div   = std::make_shared<xio::logbook::doc_element>(e,xio::logbook::doc_element::tag::div);
    xio::logbook::doc_element& head = div->parent();
    
    std::cout << "element e : " << e->text() << '\n';
    std::cout << "element head=div(e).parent(): " << head.text() << '\n';
    std::cout << "element div : " << div->text() << '\n';

    int v = 4040;
    xyz<int> zyx = 45;
    std::cout << "zyx::notice() (int v=4040; zyx = 45;):" <<  zyx.notice()() << '\n';
    int& v2 = zyx.value();
    std::cout << "int& v2 = zyx::value() :" << v2 << '\n';


    xio::utils::notification::clear(
        [](xio::utils::notification& n) 
        {
            std::cout << n() << '\n';
        }
    );

    return 0;
}