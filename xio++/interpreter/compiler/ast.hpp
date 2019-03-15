//
// Created by bretzelus on 19-03-14.
//

#pragma once


/*


 */

#include <xio++/interpreter/compiler/grammar.hpp>
#include <xio++/object.hpp>

namespace xio{

class xio_api ast {


    ast* m_parent = nullptr;
    token_t* m_token = nullptr;
    term_t*  m_term  = nullptr;
    
    
    friend class bloc_t;
    friend class compiler;
    using array_t = std::vector<ast*>;

    array_t m_children;

    ast()=default;
    ast(ast* a_ast, term_t* a_term, token_t* a_token);

    ~ast();

    ast& operator =(ast&& a) = delete;
    ast& operator =(const ast& a) = delete;

    void append(ast* a );



};


}

