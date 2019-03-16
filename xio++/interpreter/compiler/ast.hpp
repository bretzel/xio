//
// Created by bretzelus on 19-03-14.
//

#pragma once


/*
if statement (rule):

if condexpr           ifbloc            *elsebloc
      |                  |                  |
      ( expression )   *'then' { stmts }  'else'  {  stmts }
            |                     |                   |           
          +value               +statment           +statement
             ...                  ...                 ...
                                            
 */

#include <xio++/interpreter/compiler/grammar.hpp>
#include <xio++/object.hpp>

namespace xio{





class astnode: public object {


    token_t::cursor m_cursor;
    term_t::const_iterator m_term;
   

    friend class bloc_t;
    friend class compiler;
    friend class xioast;


public:
    using result = xio::expect<astnode*>;
    astnode(astnode* a_node, term_t::const_iterator i1, token_t::cursor i2) noexcept;
    ~astnode() override;



};

class xio_api xioast : public object{


    //ast* m_parent = nullptr;
    token_t::list_t* m_tokens = nullptr;
    token_t::cursor  m_cursor;
    using result = xio::expect<astnode*>;

    astnode* m_rootnode = nullptr;

    xioast()=default;
    xioast();

    ~xioast() override;

    xioast& operator =(xioast&& a) = delete;
    xioast& operator =(const xioast& a) = delete;
   
    xioast::result build(token_t::list_t* a_tokens);

    astnode::result enter_rule(astnode* a_node, term_t::const_iterator a_term, token_t::cursor a_cursor );
    bool end(token_t::cursor cc) { return cc == m_tokens->end(); }

    //void append(ast* a )
    
};


}

