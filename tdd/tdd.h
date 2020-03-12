#pragma once

#include <xio/utils/expect>


using xio::utils::expect;

class tdd
{


public:

    using result = expect<xio::utils::notification::code>;

    tdd();

    ~tdd();

    
    tdd::result run();


    tdd::result lexer();
    tdd::result logbook();

};

