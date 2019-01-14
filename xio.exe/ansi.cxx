#include "ansi.hpp"

#include <iostream>
#include <xio++/journal/logger.hpp>
#include <xio++/xio++.hpp>
#include <xio++/interpreter/kernel/function_t.hpp>
#include <xio++/interpreter/compiler/lexer.hpp>
#include <xio++/expect.hpp>

#include <xio++/interpreter/interpreter.hpp>
#include <xio++/interpreter/compiler/grammar.hpp>


namespace Ansi{


CAnsi::CAnsi()
{
}
CAnsi::CAnsi(int argc, char ** argv) :_argc(argc), _argv(argv){}


#ifdef _WIN32
DWORD CAnsi::Init()
{
    hStdIn = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hStdIn, &mode);
    if (!SetConsoleMode(hStdIn, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)){
        DWORD Err = GetLastError();
        std::cout << " Error #" << Err << " in:" __FUNCTION__ "\n";
        return Err;
    }
    F = false;
    return 0;
}

void CAnsi::End()
{
    SetConsoleMode(hStdIn, mode);
    F = true;
}
#endif


xio::message::xcode CAnsi::execute()
{
    xio::token_t::list_t tokens;
    xio::interpreter inter;
    inter.config() = {
        "test",
        "expr:\"a = .45 * sin 45;\"",
        &tokens
    };
    
    logdebugfn << " text:'" << xio::logger::Yellow << inter.name() << xio::logger::White << "';" << Ends;
    logdebugfn << " uri:'" << xio::logger::Yellow << inter.uri() << xio::logger::White << "';" << Ends;
    
    xio::alu a = inter.jsr();
    
    return xio::message::xcode::accepted;
}


CAnsi::~CAnsi()
{
//    flush_notices(*this);
    xio::logger::close();
    (*this) << Ansi::Color::HBlue << "Logger closed.\n";
#ifdef _WIN32
    if (!F)
        End();
#endif
    
}



}

long fibonacci(unsigned n)
{
    if (n < 2) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}


void test_methodology(Ansi::CAnsi& ansi)
{
    struct rt_class {
        
        rt_class(){}
        
        int call(int bb, const std::string& text){
            std::cout << Ansi::Color::HCyan << "bb=" 
                      << Ansi::Color::Yellow << bb 
                      << Ansi::Color::HCyan << "; text(pretending!)=" 
                      << Ansi::Color::Yellow << text << Ansi::Color::Reset << '\n';
            return 0;
        }
    };
    
    rt_class rt;
    xio::interpreter::rtfc_t<rt_class, int, int, const std::string&> m("call",rt,&rt_class::call);
    m.name() = "fibonacci";
    int ret = m(23,std::string("argument #1"));

    xio::alu::list_t params = {23,std::string("hello, world")};
    xio::alu al = m(params);
    
}



auto main(int argc, char **argv) -> int {
    
    Ansi::CAnsi con(argc,argv);
    #ifdef _WIN32
    con.Init();
    #endif
    xio::logger::setfile("xio.log");
    xio::logger::init(xio::logger::Mode::Ansi, "tea++ framework development.", true);
    xio::logger::resetstamp(), xio::logger::Hour24;

    string_t str = "Binaire de 10: [%04b]";
    str << 10;
    logsuccess << str() << Ends;
    loginfo << "test  _type = 16401: " << xio::type_t::name(16401) << ": " << Ends;
    //test_xio(con);
    test_methodology(con);
    con.execute();
    xio::message::clear([](xio::message& msg){
        lognotice  << msg() << Ends;
    });


    return 0;
}