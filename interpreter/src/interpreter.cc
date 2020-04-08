#include <xio/interpreter/interpreter.h>
//#include <xio/interpreter/parsers/ast.h>



namespace teacc
{

using utils::notification;


alu interpreter::jsr()
{
    return bloc::jsr();
}


utils::result_code interpreter::run()
{
    return build();
//    return {(
//        notification::push(),
//        " Tests will grow as the interpreter advances..."
//        )};
}


utils::result_code interpreter::build()
{
    lexer::lexscanners lex;
    
//    utils::result_code r = pstree.build();
//    if(!r)
//    {
//        return {(
//            utils::notification::push(), __PRETTY_FUNCTION__, ": build aborted because of previous errors (",
//            r.note()(),
//            ")."
//        )};
//    }
    
    return {(
        utils::notification::push(), "not implemented"
    )};
}

}
