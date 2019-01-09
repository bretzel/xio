#include "interpreter.hpp"
#include "../journal/logger.hpp"
#include "compiler/grammar.hpp"

xio::interpreter::interpreter():xio_module()
{
}

xio::interpreter::~interpreter()
{
}


xio::alu xio::interpreter::jsr()
{
    xio_grammar grammar;
    xio::xio_grammar::result gr =  grammar.build();
    xio::token_t::list_t tokens;
    xio_t::result r = build();
    if(!r){
        logerrorfn << r.notice()() << Ends;
        return alu(false);
    }
    
    return xio_module::jsr();
}

