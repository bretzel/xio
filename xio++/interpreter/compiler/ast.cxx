#include "ast.hpp"

#include "../../journal/logger.hpp"

namespace xio {



#define INC_SRC                 {\
++m_cursor;\
if (eof()) return ar;\
}\


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


astnode::result xioast::build(token_t::list_t * a_tokens, const std::string & rule_id)
{
    m_tokens    = a_tokens;
    m_startrule = xio_grammar()[rule_id];
    m_cursor = m_tokens->begin() + 1;
    m_rootnode = new astnode(this, m_startrule->begin()->begin(), m_cursor);
    return enter_rule(m_rootnode, m_startrule);
}




astnode::result xioast::enter_rule(astnode * parent_node, const rule_t * a_rule)
{
	astnode::result ar;

    logdebugfn << " Enter rule: " << logger::HRed << a_rule->_id << logger::Reset << Ends;


    if (a_rule->empty())
    {
        logdebugfn << " " << logger::HBlue << a_rule->_id << logger::White << "::" << logger::Yellow << m_cursor->attribute() << logger::White
            << " Empty => Accept and leave: the Parser will further analyzes during the xio generation phase..." << logger::Reset << Ends;
        return parent_node;
    }
    
    seq_t::const_iterator seq_it    = a_rule->begin();
    term_t::const_iterator term_it  = seq_it->begin();
    while (!a_rule->end(seq_it))
    {
        bool rep = false;
        while (!seq_it->end(term_it)) 
        {
            ar.clear();
            logdebugfn << " " << logger::HBlue << (*term_it)() << logger::White << "<::>" << logger::Yellow << m_cursor->attribute() << logger::Reset << Ends;

            if (term_it->is_rule())
            {
                astnode* node = new astnode(parent_node, term_it, m_cursor);
                ar = enter_rule(node, term_it->mem.r);
                if (!ar)
                    delete node; // auto detach from parent_node. (see astnode::~astnode())
                else
                {
                    //INC_SRC
                        if (!term_it->a.is_repeat()) ++term_it;
                }
            }
            else
            {
                if (*term_it == *m_cursor)
                {
                    astnode* node = new astnode(parent_node, term_it, m_cursor);
                    rep = term_it->a.is_repeat();
                    logdebugfn << " " << logger::HBlue << (*term_it)() << logger::White << "==" << logger::Yellow << m_cursor->attribute() << logger::Reset << Ends;
                    INC_SRC
                    ar = node;
                    if (term_it->a.is_one_of())
                    {
                        logdebugfn << " " << logger::HBlue << (*term_it)() << logger::White << " is a one hit rule sequence: leaving, the cursor on [" << 
                            logger::Yellow << m_cursor->attribute() << logger::White << ']' <<  logger::Reset << Ends;
                        return ar;
                    }
                    if (!rep) 
                    {
                        logdebugfn << " " << logger::HBlue << (*term_it)() << logger::White << " is not repeat." << logger::Reset << Ends;
                        ++term_it;
                        if(!seq_it->end(term_it))
                            logdebugfn << " Next term: " << logger::HBlue << (*term_it)() << logger::White << " :" << logger::Reset << Ends;

                    }
                }
            }

            if (!ar)
            {
                logdebugfn << " " << logger::HBlue << (*term_it)() << logger::White << "!=" << logger::Yellow << m_cursor->attribute() << logger::Reset << Ends;
                if (rep & term_it->a.is_repeat())
                {
                    ++term_it;
                    rep = false; // maybe not necessary because of < bool rep = false on next iteration >... 
                    continue;
                }
                if (!term_it->a.is_strict())
                {
                    ++term_it;
                    continue;
                }
                else break;
            
            }
        }
        if (ar)
            return ar;
        ++seq_it;
    }
    if (!ar) {
        return {
            (message::push(message::xclass::error),
            m_cursor->location(),
            message::code::syntax,
            " error: expected ", (*term_it)(), "\033[0m, got :", type_t::name(m_cursor->type)," instead.\n", 
            m_cursor->mark())
        };
    }
    return ar;
}



astnode::~astnode()
{
    (void)detach();
}

bool astnode::is_rule() const
{
    return term_it->is_rule();
}

const rule_t * astnode::rule() const
{
    if (!is_rule()) return nullptr;
    return term_it->mem.r;
}

}

