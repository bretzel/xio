#include "tdd.h"
#include <xio/utils/xstr.h>
#include <xio/utils/notification.h>
#include <xio/logbook/book.h>
#include <xio/lexer/lexer.h>
#include <xio/utils/journal.h>
//#include <any>

#include <signal.h>

void signal_int(int s)
{
    logdebugpfn << "Interrupted by user signal " << xio::utils::log::color::HCyan << "INT" <<ends;
    xio::utils::journal::close();
    exit(s);
}


void signal_segfault(int)
{
    LogFatalPFn << "Interrupted by segmentation fault signal" << Ends;
    xio::utils::journal::close();
    exit(127);
}


void sig_abort( int s)
{
    xio::utils::journal::close();
    
    LogCriticalPFn << "Interrupted by " << xio::utils::log::color::Yellow << "ABORT" << xio::utils::log::color::White<< " signal" << Ends;
    xio::utils::journal::close();
    exit(s);
}

void install_signals()
{
    
    LogNoticeFn << xio::utils::log::color::Yellow << " SIGINT" << Ends;
    ::signal(SIGINT, signal_int);
    LogNoticeFn  << xio::utils::log::color::HRed << " SIGSEGV" << Ends;
    ::signal(SIGSEGV, signal_segfault);
    LogNoticeFn << xio::utils::log::color::HRed << "SIGABRT" << Ends;
    ::signal(SIGABRT, sig_abort);
}



using xio::utils::expect;
using xio::utils::notification;
using xio::logbook::doc_element;
using xio::logbook::book;
using xio::utils::xstr;





tdd::result tdd::run()
{
    init();
    install_signals();
    logbook();
    lexer();
    return {
        (
         notification::push(), __PRETTY_FUNCTION__ , ": not implemented, yet..."
        )
    };
}


tdd::~tdd()
{
    std::cout << __PRETTY_FUNCTION__ << " \\O/\n";
    xio::utils::journal::close();
}

auto main() -> int {
   // tdd _tdd;
   
   tdd().run();
    
    xio::utils::notification::clear(
        [](xio::utils::notification& n) 
        {
            std::cout << n() << '\n';
        }
    );
    
    return 0;
}

tdd::result tdd::lexer()
{
    xio::lexer::lexscanners lexer;
    xio::lexer::lexscanners::code code = lexer["a = 4ac(45 * pi  + b) + sin0.3;"];
    if (!code) return code.note();

//    for (auto t : lexer.tokens())
//    {
//        std::cout << t.attribute() << " : " << xio::lexer::type::to_s(t.s) << '\n';
//    }
//    //...
    lexer.debug([](xio::lexer::type::token_t& token){
        std::cout << '[' << token.attribute() << "] :" << xio::lexer::type::to_s(token.s) << '\n';
    });

    xio::lexer::type::token_t::collection tokens = lexer.tokens(); // deep copy.
    lexer.tokens().clear();

    //  Bypass auto scan.
    //  Manual scan  ( step ).

    return notification::code::ok;
}

tdd::result tdd::logbook()
{
    std::cout << "xio::logbook::document_element::tagname(xio::logbook::document_element::tag::div): " << doc_element::tagname(doc_element::tag::div)() << '\n';

    doc_element::shared e = book::create_element(nullptr, doc_element::tag::head);
    doc_element::shared div = book::create_element(e, doc_element::tag::div);
    auto head = div->parent();

    std::cout << "element e : " << e->text() << '\n';
    std::cout << "element head=div(e).parent(): " << head->text() << '\n';
    std::cout << "element div : " << div->text() << '\n';


    return notification::code::ok;
}


tdd::result tdd::init()
{
    using  xio::utils::journal;
    journal::setfile("xioproject.log");
    journal::init(journal::Ansi, "xio project API tests (MyTDD)", true);
    journal::resetstamp(), journal::Hour24;
    logdebugpfn << " mark" << Ends;
    
    return notification::code::ok;
}
