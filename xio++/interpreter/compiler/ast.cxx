#include "ast.hpp"
namespace xio {





astnode::astnode(object * a_parent, term_t::const_iterator a_term_it, token_t::cursor a_cursor) :object(a_parent),
term_it(a_term_it),
m_cursor(a_cursor) 
{}

xioast::xioast(token_t::list_t * a_tokens, const std::string & rule_id):object(),
m_tokens(a_tokens)
{
	m_startrule = xio_grammar()[rule_id];
}

xioast::~xioast(){}


astnode * xio::xioast::new_node(term_t::const_iterator a_term_it, token_t::cursor a_cursor)
{
	astnode *n = new astnode(this, a_term_it, a_cursor);
	append_child(n);
	return n;
}

astnode::result xioast::enter_rule(astnode * parent_node, const rule_t * a_rule)
{
	astnode::result ar;
    
    seq_t::const_iterator seq_it    = a_rule->begin();
    term_t::const_iterator term_it  = seq_it->begin();


}

astnode::~astnode(){}

}

