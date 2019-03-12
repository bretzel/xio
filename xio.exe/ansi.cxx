#include "ansi.hpp"

#include <iostream>
#include <xio++/journal/logger.hpp>
#include <xio++/xio++.hpp>
#include <xio++/interpreter/compiler/lexer.hpp>
#include <xio++/expect.hpp>
#include <xio++/interpreter/kernel/bloc.hpp>
//#include <xio++/interpreter/compiler/grammar.hpp>
#include <xio++/interpreter/compiler/compiler.hpp>


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


xio::message::code CAnsi::execute()
{
    //xio::xio_grammar gr;
    //gr.build();
    //gr.dump();
    xio::token_t::list_t tokens;
    xio::compiler cc;
    cc.config() = {
        "static u64 a;",
        &tokens
    };

    xio::xio_t::result r = cc.compile();
    return xio::message::code::accepted;
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

//long fibonacci(unsigned n)
//{
//    if (n < 2) return n;
//    return fibonacci(n - 1) + fibonacci(n - 2);
//}


void test_rtfc(Ansi::CAnsi& ansi)
{
    struct rt_class {
        
        rt_class(){}
        
        double call(int bb, const std::string& text){
            std::cout << Ansi::Color::HMagenta << "Let's say that the script is calling this runtime function/method:\n" 
                      << "\033[30mbb = "
                      << Ansi::Color::Yellow << bb 
                      << Ansi::Color::HCyan << "; text(pretending!)=" 
                      << Ansi::Color::Yellow << text << Ansi::Color::Reset << '\n';
            return 0.25;
        }

        void voidcall(const std::string& message) {
            logdebugpfn << " message: " << xio::logger::Yellow << message << Ends;
        }

    };
    
    rt_class rt;
    xio::bloc_t i;
    xio::alu al;
    xio::alu::list_t params = {23.0,std::string("hello, world")};
    xio::bloc_t::rt_function* rtf = i.bind("script-func", rt, &rt_class::call);

    logdebug << Ends;
    logdebug << Ends;
    logdebugfn << xio::logger::Yellow << " Now testing the stack call to that suposed runtime function:" << Ends;
    al = i.jsr_rtf("script-func", params);
    logdebug << Ends;
    logdebug << xio::logger::White << " ret: [" << xio::logger::Yellow << al.value<float>() << xio::logger::White << ']' <<  Ends;
    logdebug << Ends;


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
    test_rtfc(con);
    con.execute();
    xio::message::clear([](xio::message& msg){
        lognotice  << msg() << Ends;
    });


    return 0;
}
