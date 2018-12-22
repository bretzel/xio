#include "ansi.hpp"

#include <iostream>
#include <xio++/journal/logger.hpp>
#include <xio++/xio++.hpp>
#include <xio++/interpreter/kernel/function_t.hpp>
#include <xio++/interpreter/compiler/lexer.hpp>
#include <xio++/expect.hpp>

#include <xio++/interpreter/kernel/alu.hpp>

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


// xio::message::type CAnsi::execute()
// {
//     logdebugfn << "testing xio_module through the derived class: interpreter_t:" << Ends;
//     logdebug << " argc:" << _argc << Ends;
//     logdebug << " argv[0]: " << _argv[0] << Ends;
//     if (_argc >= 2) {
//         for (int c = 1; c < _argc; c++)
//             logdebug << "arg #" << c << ":" << _argv[c] << Ends;
//     }
//     interpret.settings() = { 
//         "xio::test expression",
//         //"xor +=.14159;"
//         //"test=cos 1 * .5a;"
//         //"a=1.67 / 4(2+3+2^2);"
//         "hex = (i16) 0xffd2;"
//         //"'Un essaie sur une constante litterale.. Ca va planter...';"
//     };

    

//     xio::alu A;
//     uint16_t xx = 0xffd2;
//     std::cerr << " xx = " << xx << '\n';
//     int16_t xy = xx; std::cerr << " xy = " << xy << '\n';


//     xio::xio::result r = interpret.run();
//     if (r) {
//         xio::xio* i = r.value();
//         if (i) {
//             A = i->jsr();
//             loginfo << xio::logger::White << interpret.settings().src << xio::logger::Black << " = " << xio::logger::Yellow << A()  << Ends; // A[xio::alu::DEG]() << " deg;" << Ends;
//             xio::xio* v = interpret["hex"];
//             if (v) {
//                 xio::alu var_a = *v->unit();
                
//                 loginfo << xio::logger::White <<'\'' 
//                     << xio::logger::Yellow << v->token()->attribute()
//                     << xio::logger::White << "' = " 
//                     << xio::logger::Yellow << var_a() 
//                     << Ends;

//                 //loginfo << xio::logger::White << "'a' = " << xio::logger::Yellow <<  << Ends;
//             }
//             else
//                 loginfo << xio::logger::White << "'hex' has no storage?" << Ends;
//         }
//     }
//     else {
//         xio::message M = r.notice();
//         logerrorfn << "Interpreter returned error:" << M() << Ends;
//     }
//     return xio::message::type::accepted;
// }


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

//void test_xio(Ansi::CAnsi& ansi)
//{
//
//    using xio::xio;
//
//    xio::token_t::list tokens;
//    xio::tea_lexer lexer;
//    lexer.token_stream() = &tokens;
//    xio::tea_lexer::result result = lexer["1.67 / (3+2^2));"];
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
//    for(xio::token_t& token : tokens){
//        if(tt <= 1){ ++tt; continue; }
//
//        if(token.code == xio::tea_code::semicolon) break;
//
//        std::cerr << " test_xio: x = " << x->token()->loc.b << " <- input token: " << token.loc.b << "\n";
//
//        r = x->tree_input(&token, [&xio_stream](xio::token_t* a_token)->xio*{
//
//            std::cerr << " test_xio::lamba[] => a_token:" << a_token->loc.b << "\n";
//            xio* newx = new xio(nullptr, a_token);
//            xio_stream.push_back(newx);
//            return newx;
//
//        });
//
//        if(!r){
//            xio::message m = r.notice();
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
//            xio::alu a = xx_op->jsr();
//            std::cerr << lexer.text() << " = " << a() << '\n';
//            logdebugfn << " Size of xiostream: " << xio::logger::HBlue << xio_stream.size() << xio::logger::Reset << Ends;
//            loginfo << xio::logger::HCyan << lexer.text() << " = " << xio::logger::Yellow << a.number<float>() << Ends;
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
    xio::logger::setfile("tea++.log");
    xio::logger::init(xio::logger::Mode::Ansi, "tea++ framework development.", true);
    xio::logger::resetstamp(), xio::logger::Hour24;

    string_t str = "Binaire de 10: [%04b]";
    str << 10;
    logsuccess << str() << Ends;
    loginfo << "test  _type = 16401: " << xio::type_t::name(16401) << ": " << Ends;
    //con.execute(); // Test xio_module/interpreter_t.
    test_method(con);
    xio::message::clear([](xio::message& msg){
        lognotice  << msg() << Ends;
    });


    return 0;
}