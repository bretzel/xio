//
// Created by bretzelus on 19-03-14.
//

#include "ast.hpp"


namespace xio{



astnode::astnode(astnode* a_node, term_t::const_iterator i1, token_t::cursor i2)noexcept :object(a_node),
m_term(i1), 
m_cursor(i2)
{
}

xioast::~xioast() {

    detach();
    for (auto a : m_children)  delete a;
}

xioast::result xioast::build(token_t::list_t* a_tokens)
{


    return {};
}


astnode::result xioast::enter_rule(astnode* parent_node, term_t::const_iterator a_term, token_t::cursor a_cursor)
{
    astnode::result r;
    ///@todo validate rule...
    auto term_it = a_term;
    
    rule_t* rule = a_term->mem.r;
    auto seq_it = rule->begin();

    while (!rule->end(seq_it))
    {
        auto term_it = seq_it->begin();
        while (!seq_it->end(term_it)) 
        {

        }
    }

    return {};
}



astnode::~astnode()
{
    detach();
}

astnode astnode::enter_rule(term_t::const_iterator a_term)
{

    return astnode();
}

}