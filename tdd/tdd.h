#pragma once

#include <xio/utils/expect>


using teacc::utils::expect;

class tdd
{

    int     _argc = 0;
    char ** _argv =nullptr;
    std::string _logfile;
public:

    using result = expect<teacc::utils::notification::code>;

    tdd() = default;

    tdd(int argc, char** argv): _argc(argc),_argv(argv){}
    ~tdd();

    
    tdd::result run();

    tdd::result init();
    
    tdd::result lexer();
    tdd::result logbook();
    tdd::result alu();
    tdd::result interpreter();
    tdd::result test_cargs(const std::string& _arg);
    tdd::result set_logfile(const std::string &_arg);
    

};

