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

    // Implicit lexical analyze:
    xio::xio_t::result res = xio::xio_module::build();
    // ---------------------------

    if( !(aebtree = rules["expression"]) )
        return { (xio::message::push(xio::message::xclass::error), xio::message::code::empty, "grammar gave no rule for arithmetic expression.") };


    return xio_t::result();
}
