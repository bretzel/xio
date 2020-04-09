//
// Created by bretzel on 20-04-08.
//

#include <xio/interpreter/parsers/parsers.h>
#include <xio/lexer/lexer.h>
#include <xio/utils/notification.h>
#include <xio/utils/journal.h>


using teacc::utils::notification;
using logger = teacc::utils::journal;


namespace teacc::parsers
{

parse_tree::~parse_tree()
{
    logdebugpfn << " \\O/!" << ends;
}

/*!
 * @brief Entry point of this parse-tree parsers.
 * @param a_input_node
 * @return ast::node* root node of the parse tree.
 */
parse_tree::result parse_tree::parse(ast::node *a_input_node)
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
parse_tree::result parse_tree::parse_rule(const rule_t *rule, ast::node *input_location)
{
    
    return {(notification::push(), " Implement")};
    
}

utils::result_code parse_tree::parse_rule_elements(seq_t::const_iterator seq, ast::node *input_location)
{
    return {(notification::push(), " Implement")};
    
}
parse_tree::result parse_tree::parse_expression(ast::node *input_location)
{
    return {(notification::push(), " Implement")};
    
}
std::string parse_tree::make_dotgraph()
{
    return "not implemented yet";
}

void parse_tree::discard_nodes(ast::node* input_location)
{
    
}

}
