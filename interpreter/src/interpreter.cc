#include <xio/interpreter/interpreter.h>
//#include <xio/interpreter/parsers/ast.h>
#include <xio/utils/journal.h>

using logger = teacc::utils::journal;
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
    _unit = new interpreter::tree_unit;
    
    
    // *_text is not corrupted in this stack, and left untouched after leave.
    _unit->_text = "a = 4ac(45 * pi  + b) + sin0.3;";
    _unit->compile();
    
    return {(
        utils::notification::push(), "not implemented"
    )};
}
interpreter::~interpreter()
{
    delete _unit;
}

utils::expect<ast::node *> interpreter::tree_unit::compile()
{
    
    return
    {(
        notification::push(), __PRETTY_FUNCTION__, '\n',
        '"',_text, '"', '\n',
        " - Not implemented, yet! :)"
    )};
}


interpreter::tree_unit::~tree_unit()
{
    logdebugpfn << " \\O/!" << ends;
    logdebugpfn << " \\O/!" << ends;
    const char *yeah = R"(
\O/
 |
- -
)";
    
    const char *warn = R"(
\O
 |\
- -
)";
    
    const char *err = R"(
/O\
 |
- -
)";
    
    logerror << logger::Red << err << ends;
    logwarning << logger::Yellow << warn << ends;
    logsuccess << logger:: Green << yeah << ends;
    
}


}
