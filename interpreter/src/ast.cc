#include <xio/interpreter/parsers/ast.h>
#include <xio/interpreter/parsers/grammar.h>




namespace teacc::ast
{


using utils::notification;


utils::result_code astbloc::build()
{
    utils::result_code r  = _rules.build();
    
    return
        {(
             notification::push(), __PRETTY_FUNCTION__ ,
             notification::code::implement
             ," wtf? I'm only testing!!"
         )};
    
}
}