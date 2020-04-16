#include "tdd.h"
#include <xio/utils/xstr.h>
#include <xio/utils/notification.h>
#include <xio/logbook/book.h>
#include <xio/lexer/lexer.h>
#include <xio/utils/journal.h>
//#include <any>
#include <xio/interpreter/alu.h>
//#include <xio/interpreter/bloc.h>
#include <xio/interpreter/interpreter.h>

#include <xio/utils/cargs.h>

#include <csignal>

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
    std::signal(SIGINT, signal_int);
    LogNoticeFn  << teacc::utils::log::color::HRed << " SIGSEGV" << Ends;
    std::signal(SIGSEGV, signal_segfault);
    LogNoticeFn << teacc::utils::log::color::HRed << "SIGABRT" << Ends;
    std::signal(SIGABRT, sig_abort);
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
    //logbook();
    //lexer();
    //alu();
    //interpreter();
    return {
        (
         notification::push(), __PRETTY_FUNCTION__ , ": not implemented, yet..."
        )
    };
}


tdd::~tdd()
{
    loginfopfn << __PRETTY_FUNCTION__ << " \\O/\n";
    teacc::utils::journal::close();
}

auto main(int arc, char** argv) -> int {
    tdd _tdd = tdd(arc,argv);
    
    tdd::result r = _tdd.run();
    loginfopfn << " Notifications:" << ends; //?????????????
    notification::clear(
        [](notification& n) 
        {
            lognotice << n() << ends;
            //std::cout << n() << '\n';
        }
    );
    
    return 0;
}


tdd::result tdd::alu(const std::string &src_code)
{
    teacc::alu a = 4.02;
    teacc::alu b = 0;
    teacc::alu r;
    std::any an;
    an = 0;
    logdebugpfn << " std::any 'an' has_value: "  << an.has_value() << '\n';
    an.reset();
    logdebugpfn << " std::any 'an' has_value after reset: "  << an.has_value() << '\n';

    LogDebug << ": a/b[" << b.number<double>() << "]:" << ends;
    r = a/b;
    if(r.is_set())
    ///@todo DO NOT FORGET to Implement exceptions in the interperter;
        logdebug << ": a/b=" << r() << ends;
    else 
        logdebug << ": a/b= Failed; UNSET." << ends;
    
    b = 4.01;
    logdebug << ": a/b[" << b.number<double>() << "]:" << ends;
    

    r = a/b;
    if(r.is_set())
    ///@todo DO NOT FORGET to Implement exceptions in the interperter;
        logdebug << ": a/b=" << r() << ends;
    else 
        logdebug << ": a/b= Failed; UNSET." << ends;
    
    notification::push(), __PRETTY_FUNCTION__, notification::code::ok;

    return notification::code::ok;
}



tdd::result tdd::lexer(const std::string &src_code)
{
    teacc::lexer::lexscanners lexer;
    teacc::lexer::lexscanners::code code = lexer[src_code.c_str()]; //lexer["a = 4ac(45 * pi  + b) + sin0.3;"];
    if (!code) return code.note();

//    for (auto t : lexer.tokens())
//    {
//        std::cout << t.attribute() << " : " << teacc::lexer::type::to_s(t.s) << '\n';
//    }
//    //...
    lexer.debug([](teacc::lexer::type::token_t& token){
        loginfo << '[' << token.attribute() << "] :" << teacc::lexer::type::to_s(token.s) << ends;
    });

    teacc::lexer::type::token_t::collection tokens = lexer.tokens(); // deep copy.
    lexer.tokens().clear();

    //  Bypass auto scan.
    //  Manual scan  ( step ).

    return notification::code::ok;
}

tdd::result tdd::logbook(const std::string &src_code)
{
    std::cout << "teacc::logbook::document_element::tagname(teacc::logbook::document_element::tag::div): " << doc_element::tagname(doc_element::tag::div)() << '\n';

    doc_element::shared e = book::create_element(nullptr, doc_element::tag::head);
    doc_element::shared div = book::create_element(e, doc_element::tag::div);
    auto head = div->parent();

    logdebug << "element e : " << e->text() << '\n';
    logdebug << "element head=div(e).parent(): " << head->text() << ends;
    logdebug << "element div : " << div->text() << ends;


    return notification::code::ok;
}


tdd::result tdd::init()
{
    using  teacc::utils::journal;
    journal::setfile("xioproject.log");
    journal::init(journal::Ansi, "xio project API dev & tests (MyTDD)", true);
    journal::resetstamp(), journal::Hour24;
    logdebugpfn << " mark" << Ends;
    
    teacc::utils::cargs<tdd> cmd;
    
    for(const auto& arg : teacc::utils::cargs<tdd>::arg::collection
        {
            {"help", 'h', true, 1,&tdd::help, "print out the usage on the console."},
            {"test", 't', true, 1,&tdd::test_cargs, "This is a description text..."},
            {"logfile",'l',true,1,&tdd::set_logfile, "Give the log file/path/name"}
        }
        )
        cmd  << arg;
    
    
    cmd.process(_argc, _argv);
    
    return notification::code::ok;
}


tdd::result tdd::interpreter(const std::string &src_code)
{
    teacc::interpreter interpreter;
    teacc::utils::result_code r = interpreter.run();
    if(!r)
        logdebugpfnx << " - TEST!!!! :) \\O/" << ends;
    
    return {(
        notification::push(), __PRETTY_FUNCTION__, " complete."
    )};
}
tdd::result tdd::test_cargs(const std::string &_arg)
{
    logdebugpfn << ends;
    
    return teacc::utils::notification::code::implement;
}
tdd::result tdd::set_logfile(const std::string &_arg)
{
    return teacc::utils::notification::code::implement;
}
tdd::result tdd::help(const std::string &)
{
    return teacc::utils::notification::code::implement;
}






