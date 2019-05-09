//
// Created by bretzelus on 19-03-14.
//

#pragma once

#include <xio++/interpreter/compiler/grammar.hpp>
#include <xio++/interpreter/kernel/xio.hpp>

namespace xio {
/*
if statement rule:

if condexpr           ifbloc            *elsebloc
      |                  |                  |
      ( expression )   *'then' { stmts }  'else'  {  stmts }
            |                     |                   |
          +value               +statement           +statement
             ...                  |                    ...
                              assignstmt   ';', declvar ';', expression ';', instruction ';', var_id ';', ';'.
                                  |
                                  |
                                  |
                                *typename      new_var      '='   expression
                                   |              |                  |
                                [types enum]    cc_new_var         +value
 */

struct astnode : public object
{
    term_t::const_iterator term_it;
    token_t::cursor m_cursor;

    using result = expect<astnode*>;

    astnode() = default;
    astnode(object* a_parent, term_t::const_iterator a_term_it, token_t::cursor a_cursor);

    ~astnode() override;

    bool is_rule()const;
    const rule_t* rule() const;
};

class xioast : public object
{
    friend class compiler;

    token_t::list_t* m_tokens = nullptr;
    token_t::cursor  m_cursor;
    astnode*         m_rootnode = nullptr;
    const rule_t*    m_startrule = nullptr;

    xioast() = default;
    xioast(token_t::list_t* a_tokens, const std::string& rule_id);

    ~xioast() override;

    astnode* new_node(term_t::const_iterator a_term_it, token_t::cursor a_cursor);
    bool eof() { return m_cursor == m_tokens->end(); }

    astnode::result enter_rule(astnode* parent_node, const rule_t* a_rule);

    astnode::result build(token_t::list_t* a_tokens, const std::string& rule_id);
    astnode::result directive(astnode* a_node, const rule_t* a_rule);
};
}