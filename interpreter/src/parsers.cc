//
// Created by bretzel on 20-04-08.
//

#include <xio/interpreter/parsers/parsers.h>
#include <xio/lexer/lexer.h>
#include <xio/utils/notification.h>
#include <xio/utils/journal.h>
#include <xio/interpreter/interpreter.h>

using teacc::utils::notification;
using logger = teacc::utils::journal;


namespace teacc::parsers
{

    
    
parser::parser(bloc* _bloc):_bloc(_bloc)
{

}


    
parser::~parser()
{
    logdebugpfn << " \\O/!" << ends;
}

/*!
 * @brief Entry point of this parse-tree parsers.
 * @param a_input_node
 * @return ast::node* root node of the parse tree.
 */
parser::result parser::parse(ast::node *a_input_node)
{

    if(!_tokens_stream || !_source)
    {
        return
            {(
                notification::push(),
                notification::type::error, " empty source or null tokens stream (internal?)"
            )};
    }
    
    _input_node = a_input_node;
    
    return {(
       notification::push(), " Implement"
    )};
}
/*!
 * @brief Enters rule parse node.
 * @param rule
 * @param input_location
 * @return ast::node*
 */
parser::result parser::parse_rule(const rule_t *rule, ast::node *input_location)
{
    
    return {(notification::push(), " Implement")};
    
}

utils::result_code parser::parse_rule_elements(seq_t::const_iterator seq, ast::node *input_location)
{
    return {(notification::push(), " Implement")};
    
}
parser::result parser::parse_expression(ast::node *input_location)
{
    return {(notification::push(), " Implement")};
    
}
std::string parser::make_dotgraph()
{
    return "not implemented yet";
}

void parser::discard_nodes(ast::node* input_location)
{
    
}

}
