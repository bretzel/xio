//
// Created by bretzelus on 19-03-14.
//

#pragma once


/*


 */

#include <xio++/interpreter/compiler/grammar.hpp>
#include <xio++/object.hpp>

namespace xio{

class xio_api ast : public object {

    token_t* m_token = nullptr;
    term_t*  m_term  = nullptr;
    friend class bloc_t;
    friend class compiler;
public:
    ast()=default;

    ast(ast* a_ast, token_t* a_token);
    ~ast();

    ast& operator =(ast&&) = delete;
    ast& operator =(const ast&) = delete;

};


}

