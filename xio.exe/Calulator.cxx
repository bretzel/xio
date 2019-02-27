#include "Calulator.hpp"
#include <xio++/interpreter/compiler/grammar.hpp>



Calulator::Calulator()
{
}


Calulator::~Calulator()
{
}

xio::xio_t::result Calulator::build()
{

    
    xio::xio_t::result res = xio::xio_module::build();
    // ---------------------------


    return parse();
}

xio::xio_t::result Calulator::parse()
{
    if( !(aebtree = rules["expression"]) )
        return { (xio::message::push(xio::message::xclass::error), xio::message::code::empty, "grammar gave no rule for arithmetic expression.") };

    xio::compiler cc;
    cc.context_config() = {
        this,
        &m_tokens
    };


    return { nullptr };
}
