#include <xio/interpreter/interpreter.h>



namespace teacc
{

using utils::notification;


alu interpreter::jsr()
{
    return bloc::jsr();
}


utils::result_code interpreter::run()
{
    (void)_grammar.build();
    
    return {(
        notification::push(),
        " Tests will grow as the interpreter advances..."
        )};
}


}
