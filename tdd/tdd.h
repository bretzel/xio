#pragma once

#include <xio/utils/expect>


using teacc::utils::expect;

class tdd
{


public:

    using result = expect<teacc::utils::notification::code>;

    tdd() = default;

    ~tdd();

    
    tdd::result run();

    tdd::result init();
    
    tdd::result lexer();
    tdd::result logbook();
    tdd::result alu();
    

};

