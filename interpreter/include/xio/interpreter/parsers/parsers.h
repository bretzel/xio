//
// Created by bretzel on 20-04-08.
//

//#ifndef LIBXIO_PARSERS_H
//#define LIBXIO_PARSERS_H

#pragma once

#include <xio/interpreter/interpreterdll.h>
#include <xio/interpreter/parsers/ast.h>
#include <utility>

//#include <xio/lexer/lexer.h>

namespace teacc::parsers
{


class INTERPRETERAPI parse_tree
{
    ast::node*                      _root = nullptr;
    ast::node*                      _node = nullptr;
    ast::node*                      _input_node = nullptr;
    
    
    
    lexer::type::token_t::collection*    _tokens_stream = nullptr; ///< Mandatory external working tokens stream storage.
    const char*                         _source = nullptr;
    
public:
    using result = utils::expect<ast::node*>;
    
    struct input_tokens
    {
        lexer::type::token_t::iterator _start_token;
        lexer::type::token_t::iterator _stop_token;
    };
    
    lexer::type::token_t::collection*& tokens_stream() { return _tokens_stream; }
    const char*& source() { return _source; }
    ast::node*   ast_root() { return _root; }
    
    parse_tree::input_tokens& tokens_sequence() { return _tokens; }
    
    
    parse_tree() = default;
    ~parse_tree();
    
    
    parse_tree::result parse(ast::node* a_input_node = nullptr);
    parse_tree::result parse_rule(const rule_t* rule, ast::node *input_location);
    utils::result_code parse_rule_elements(seq_t::const_iterator seq, ast::node* input_location);
    
    /*!
     * @brief -- THE -- reason of teacc grammar and parse tree.
     * @param input_location
     * @return expected ast::node* expression ast root(entry-point) into the parse_tree::result.
     */
    parse_tree::result parse_expression(ast::node* input_location);
    
    
    std::string make_dotgraph();
    
    
private:
    input_tokens _tokens;
    
};

}

//#endif //LIBXIO_PARSERS_H
