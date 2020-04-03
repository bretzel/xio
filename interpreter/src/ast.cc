#include <xio/interpreter/parsers/ast.h>



namespace teacc::ast
{


using utils::notification;

utils::result_code astbloc::build()
{
    return 
    {(
        notification::push(), notification::code::implement
    )};
}

}