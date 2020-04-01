

#include <xio/interpreter/var.h>



namespace teacc
{
    
var::var(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_acc):xio(a_parent, a_token, a_acc)
{}


var::shared var::make(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_acc)
{
    return std::make_shared<var>(a_parent, a_token, a_acc);
}


}