//
// Created by bretzel on 20-04-08.
//

//#ifndef LIBXIO_PARSERS_H
//#define LIBXIO_PARSERS_H

#pragma once

#include <xio/interpreter/interpreterdll.h>
#include <xio/interpreter/parsers/ast.h>
#include <utility>



namespace teacc::parsers
{


/*!
 * @brief Class parser produces the AST from the|sequence of tokens stream.
 *
 * @author &copy; 2020, Serge Lussier / Lonesomecoder (lussier.serge@gmail.com)
 *
 */
class INTERPRETERAPI parser
{
    ast::node* _root = nullptr; ///< Entry node of the AST.
    ast::node* _node = nullptr; ///< Current node;
    ast::node* _input_node = nullptr; ///< ...Input node...
    ast::node* _bloc = nullptr; ///< Current Bloc/?
    // bloc* _bloc = nullptr;
    

    lexer::type::token_t::collection* _tokens_stream = nullptr; ///< Does not owns the tokens stream.
    const char* _source = nullptr; ///< Source code...

public:
    using result = utils::expect<ast::node*>; ///< Holds the _root node (ptr-to).
    
    /*!
        @brief For Future tokens  (sub-)sequence parse
    */
    struct input_tokens
    {
        lexer::type::token_t::iterator _start_token;
        lexer::type::token_t::iterator _stop_token;
    };

    lexer::type::token_t::collection*& tokens_stream() { return _tokens_stream; }
    const char*& source() { return _source; }
    ast::node* ast_root() { return _root; }

    parser::input_tokens& tokens_sequence() { return _tokens_seq; }

    parser() = default;
    parser(ast::node* a_bloc);
    ~parser();

    parser::result parse(ast::node* a_input_node = nullptr);
    parser::result parse_rule(const rule_t* rule, ast::node* input_location);
    utils::result_code parse_rule_elements(seq_t::const_iterator seq, ast::node* input_location);

    /*!
     * @brief -- THE -- reason of teacc grammar and parse tree.
     * @param input_location
     * @return expected ast::node* expression ast root(entry-point) into the parse_tree::result.
     */
    parser::result parse_expression(ast::node* input_location);

    std::string make_dotgraph();

private:
    input_tokens _tokens_seq;
    void discard_nodes(ast::node* input_location);
};
}

//#endif //LIBXIO_PARSERS_H
