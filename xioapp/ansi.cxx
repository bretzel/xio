#include "ansi.hpp"

#include <iostream>
#include <tea++/journal/logger.hpp>
#include <tea++/core/queu.hpp>
#include <tea++/xio/core/xio.hpp>
#include <tea++/xio/core/function_t.hpp>

//#include <tea++/core/tea_types.hpp>
#include <tea++/lexer/tea_lexer.hpp>
#include <tea++/core/tea_notify.hpp>

#include <tea++/xio/core/alu.hpp>

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


tea::message::type CAnsi::execute()
{
    logdebugfn << "testing xio_module through the derived class: interpreter_t:" << Ends;
    logdebug << " argc:" << _argc << Ends;
    logdebug << " argv[0]: " << _argv[0] << Ends;
    if (_argc >= 2) {
        for (int c = 1; c < _argc; c++)
            logdebug << "arg #" << c << ":" << _argv[c] << Ends;
    }
    interpret.settings() = { 
        "xio::test expression",
        //"xor +=.14159;"
        //"test=cos 1 * .5a;"
        //"a=1.67 / 4(2+3+2^2);"
        "hex = (i16) 0xffd2;"
        //"'Un essaie sur une constante litterale.. Ca va planter...';"
    };

    

    tea::alu A;
    uint16_t xx = 0xffd2;
    std::cerr << " xx = " << xx << '\n';
    int16_t xy = xx; std::cerr << " xy = " << xy << '\n';


    tea::xio::result r = interpret.run();
    if (r) {
        tea::xio* i = r.value();
        if (i) {
            A = i->jsr();
            loginfo << tea::logger::White << interpret.settings().src << tea::logger::Black << " = " << tea::logger::Yellow << A()  << Ends; // A[tea::alu::DEG]() << " deg;" << Ends;
            tea::xio* v = interpret["hex"];
            if (v) {
                tea::alu var_a = *v->unit();
                
                loginfo << tea::logger::White <<'\'' 
                    << tea::logger::Yellow << v->token()->attribute()
                    << tea::logger::White << "' = " 
                    << tea::logger::Yellow << var_a() 
                    << Ends;

                //loginfo << tea::logger::White << "'a' = " << tea::logger::Yellow <<  << Ends;
            }
            else
                loginfo << tea::logger::White << "'hex' has no storage?" << Ends;
        }
    }
    else {
        tea::message M = r.notice();
        logerrorfn << "Interpreter returned error:" << M() << Ends;
    }
    return tea::message::type::accepted;
}


CAnsi::~CAnsi()
{
//    flush_notices(*this);
    tea::logger::close();
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

//void test_xio(Ansi::CAnsi& ansi)
//{
//
//    using tea::xio;
//
//    tea::token_t::list tokens;
//    tea::tea_lexer lexer;
//    lexer.token_stream() = &tokens;
//    tea::tea_lexer::result result = lexer["1.67 / (3+2^2));"];
//
//    if(!result) {
//        ansi << result.notice()() << '\n';
//        tokens.clear();
//        return;
//    }
//
//    xio* x = new xio(nullptr, &tokens[1]);
//    xio::result r;
//    int tt=0;
//    xio::list_t xio_stream;
//    xio_stream.push_back(x);
//    for(tea::token_t& token : tokens){
//        if(tt <= 1){ ++tt; continue; }
//
//        if(token.code == tea::tea_code::semicolon) break;
//
//        std::cerr << " test_xio: x = " << x->token()->loc.b << " <- input token: " << token.loc.b << "\n";
//
//        r = x->tree_input(&token, [&xio_stream](tea::token_t* a_token)->xio*{
//
//            std::cerr << " test_xio::lamba[] => a_token:" << a_token->loc.b << "\n";
//            xio* newx = new xio(nullptr, a_token);
//            xio_stream.push_back(newx);
//            return newx;
//
//        });
//
//        if(!r){
//            tea::message m = r.notice();
//            logdebugpfn << " Error: " << m() << Ends;
//            break;
//        }
//        x = r.value();
//        std::cerr << " test_xio: (r.value() -> x = " << x->token()->loc.b << ")\n";
//    }
//
//    if (r) {
//        xio::result r = x->tree_close();
//
//        if (r) {
//
//            xio* xx_op = r.value();
//            tea::alu a = xx_op->jsr();
//            std::cerr << lexer.text() << " = " << a() << '\n';
//            logdebugfn << " Size of xiostream: " << tea::logger::HBlue << xio_stream.size() << tea::logger::Reset << Ends;
//            loginfo << tea::logger::HCyan << lexer.text() << " = " << tea::logger::Yellow << a.number<float>() << Ends;
//
//            for (auto* ix : xio_stream) {
//                //if(xx)
//                logdebugfn << ix->token()->informations() << Ends;
//                delete ix;
//            }
//        }
//        else {
//            logerror << r.notice()() << Ends;
//        }
//    }
//
//    tokens.clear();
//    // leak: x........
//}



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
    //tea::method<met_tester,int,int, const std::string&> me = tea::method<met_tester,int,int, const std::string&>("function", &m, &met_tester::function);
    tea::method<met_tester,std::string,int, const std::string&> me("function", &m, &met_tester::function);
    std::string r = me(200,"Hey Salutations!");
    std::cerr << __FUNCTION__ << " r = " << r << '\n';
    tea::alu::list_t params(2);
    params = {tea::alu(200),"Hey Salut La Gang hjahaha!"};
    tea::alu rr = me(params);
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
    tea::logger::setfile("tea++.log");
    tea::logger::init(tea::logger::Mode::Ansi, "tea++ framework development.", true);
    tea::logger::resetstamp(), tea::logger::Hour24;

    string_t str = "Binaire de 10: [%04b]";
    str << 10;
    logsuccess << str() << Ends;
    loginfo << "test  _type = 16401: " << tea::type_t::name(16401) << ": " << Ends;
    con.execute(); // Test xio_module/interpreter_t.
    //test_method(con);
    tea::message::clear([](tea::message& msg){
        lognotice  << msg() << Ends;
    });


    return 0;
}
