#include "ansi.hpp"

#include <iostream>
#include <xio++/journal/logger.hpp>
#include <xio++/xio++.hpp>
#include <xio++/interpreter/kernel/function_t.hpp>
#include <xio++/interpreter/compiler/lexer.hpp>
#include <xio++/expect.hpp>

#include <xio++/interpreter/kernel/xio.hpp>
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
    xio::xio_grammar grammar;
    xio::xio_grammar::result r =  grammar.build();
    if(!r){
        logerrorfn << " >" << r.notice()() << Ends;
    }
    
    grammar.dump();
    return r.value();
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

void test_xio(Ansi::CAnsi& ansi)
{

   using namespace xio;

   token_t::list_t tokens;
   lexer_t lexer;
   lexer.token_stream() = &tokens;
   lexer_t::result result = lexer["1.67 / (3+2^2));"];

   if(!result) {
       ansi << result.notice()() << '\n';
       tokens.clear();
       return;
   }

   xio_t* x = new xio_t(nullptr, &tokens[1]);
   xio_t::result r;
   int tt=0;
   xio_t::list_t xio_stream;
   xio_stream.push_back(x);
   for(token_t& token : tokens){
       if(tt <= 1){ ++tt; continue; }

       if(token.code == e_code::semicolon) break;

       std::cerr << " test_xio: x = " << x->token()->loc.b << " <- input token: " << token.loc.b << "\n";

       r = x->tree_input(&token, [&xio_stream](token_t* a_token)->xio_t*{

           std::cerr << " test_xio::lamba[] => a_token:" << a_token->loc.b << "\n";
           xio_t* newx = new xio_t(nullptr, a_token);
           xio_stream.push_back(newx);
           return newx;

       });

       if(!r){
           message m = r.notice();
           logdebugpfn << " Error: " << m() << Ends;
           break;
       }
       x = r.value();
       std::cerr << " test_xio: (r.value() -> x = " << x->token()->loc.b << ")\n";
   }

   if (r) {
       xio_t::result r = x->tree_close();

       if (r) {

           xio_t* xx_op = r.value();
           alu a = xx_op->jsr();
           std::cerr << lexer.text() << " = " << a() << '\n';
           logdebugfn << " Size of xiostream: " << logger::HBlue << xio_stream.size() << logger::Reset << Ends;
           loginfo << logger::HCyan << lexer.text() << " = " << logger::Yellow << a.number<float>() << Ends;

           for (auto* ix : xio_stream) {
               //if(xx)
               logdebugfn << ix->token()->informations() << Ends;
               delete ix;
           }
       }
       else {
           logerror << r.notice()() << Ends;
       }
   }

   tokens.clear();
   // leak: x........
}



void test_method(Ansi::CAnsi& ansi)
{
    struct met_tester {

        std::string function(int a1, const std::string& a2){
            std::cerr << "a1=" << a1 << ", a2='" << a2 << "'\n";
            return "test_tester::function...";
        }
    };
    met_tester m;
    try {
    //xio::method<met_tester,int,int, const std::string&> me = xio::method<met_tester,int,int, const std::string&>("function", &m, &met_tester::function);
    xio::method<met_tester,std::string,int, const std::string&> me("function", &m, &met_tester::function);
    std::string r = me(200,"Hey Salutations!");
    std::cerr << __FUNCTION__ << " r = " << r << '\n';
    xio::alu::list_t params(2);
    params = {xio::alu(200),"Hey Salut La Gang hjahaha!"};
    xio::alu rr = me(params);
    std::cerr << __FUNCTION__ << " rr = " << rr() << '\n';
    }
    catch(std::bad_alloc e){
        std::cerr << __FUNCTION__ << e.what() << "\n ";
    }
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
    //test_method(con);
    con.execute();
    xio::message::clear([](xio::message& msg){
        lognotice  << msg() << Ends;
    });


    return 0;
}
