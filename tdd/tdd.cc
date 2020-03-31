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
    logdebugpfn << "Interrupted by user signal " << teacc::utils::log::color::HCyan << "INT" <<ends;
    teacc::utils::journal::close();
    exit(s);
}


void signal_segfault(int)
{
    LogFatalPFn << "Interrupted by segmentation fault signal" << Ends;
    teacc::utils::journal::close();
    exit(127);
}


void sig_abort( int s)
{
    teacc::utils::journal::close();
    
    LogCriticalPFn << "Interrupted by " << teacc::utils::log::color::Yellow << "ABORT" << teacc::utils::log::color::White<< " signal" << Ends;
    teacc::utils::journal::close();
    exit(s);
}

void install_signals()
{
    
    LogNoticeFn << teacc::utils::log::color::Yellow << " SIGINT" << Ends;
    ::signal(SIGINT, signal_int);
    LogNoticeFn  << teacc::utils::log::color::HRed << " SIGSEGV" << Ends;
    ::signal(SIGSEGV, signal_segfault);
    LogNoticeFn << teacc::utils::log::color::HRed << "SIGABRT" << Ends;
    ::signal(SIGABRT, sig_abort);
}



using teacc::utils::expect;
using teacc::utils::notification;
using teacc::logbook::doc_element;
using teacc::logbook::book;
using teacc::utils::xstr;





tdd::result tdd::run()
{
    
    init();
    install_signals();
    loginfopfn << Ends;
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
    teacc::utils::journal::close();
}

auto main() -> int {
   // tdd _tdd;
   
   tdd().run();
    
    teacc::utils::notification::clear(
        [](teacc::utils::notification& n) 
        {
            std::cout << n() << '\n';
        }
    );
    
    return 0;
}

tdd::result tdd::lexer()
{
    teacc::lexer::lexscanners lexer;
    teacc::lexer::lexscanners::code code = lexer["a = 4ac(45 * pi  + b) + sin0.3;"];
    if (!code) return code.note();

//    for (auto t : lexer.tokens())
//    {
//        std::cout << t.attribute() << " : " << teacc::lexer::type::to_s(t.s) << '\n';
//    }
//    //...
    lexer.debug([](teacc::lexer::type::token_t& token){
        std::cout << '[' << token.attribute() << "] :" << teacc::lexer::type::to_s(token.s) << '\n';
    });

    teacc::lexer::type::token_t::collection tokens = lexer.tokens(); // deep copy.
    lexer.tokens().clear();

    //  Bypass auto scan.
    //  Manual scan  ( step ).

    return notification::code::ok;
}

tdd::result tdd::logbook()
{
    std::cout << "teacc::logbook::document_element::tagname(teacc::logbook::document_element::tag::div): " << doc_element::tagname(doc_element::tag::div)() << '\n';

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
    using  teacc::utils::journal;
    journal::setfile("xioproject.log");
    journal::init(journal::Ansi, "xio project API tests (MyTDD)", true);
    journal::resetstamp(), journal::Hour24;
    logdebugpfn << " mark" << Ends;
    
    return notification::code::ok;
}
