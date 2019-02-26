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
    
    return xio_module::jsr();
}

