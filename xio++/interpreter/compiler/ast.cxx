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

astnode::astnode(astnode* a_node): object(a_node),
m_term(a_node->m_term),
m_cursor(a_node->m_cursor)
{

}

xioast::~xioast() {

    detach();
    for (auto a : m_children)  delete a;
}

xioast::result xioast::build(token_t::list_t* a_tokens)
{

	m_tokens = a_tokens;

    return {};
}


astnode::result xioast::enter_rule(astnode* parent_node) // , term_t::const_iterator a_term, token_t::cursor a_cursor)
{
    astnode::result r;
    ///@todo validate rule...
    rule_t* rule = parent_node->m_term->mem.r;
    auto seq_it = rule->begin();
    astnode::result ar;
    bool rep = false;
    
    while (!rule->end(seq_it))
    {
        auto term_it = seq_it->begin();
		ar.clear();
        while (!seq_it->end(term_it)) 
        {
            if (term_it->is_rule())
                ar = enter_rule(new astnode(parent_node, term_it, m_cursor));
            else
                if (*term_it == *m_cursor) 
					ar = new astnode(parent_node, term_it, m_cursor);
            if (!ar) 
            {
				if (term_it->a.is_strict() && !rep)
					break;
				++term_it;
				continue;
            }
			
			rep = term_it->a.is_repeat();

			if (term_it->a.is_one_of())
			{
				++m_cursor;
				return ar;
			}

			if (!rep)
				++term_it;
			++m_cursor;
			
			if (end(m_cursor)) return ar;
        }
    }

    return {};
}



astnode::~astnode()
{
    detach();
}


}