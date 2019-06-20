//
// Created by bretzelus on 19-06-19.
//

#pragma once

#include <xio++/interpreter/kernel/bloc.hpp>


/*!
    Implements a basic-to-intermediate Aritmetic Expression Evaluator.

    The primary motivation of this class is to test the mechanism/algorithm of the xio_t arithmetic expr tree input.

 */
namespace xio{
class calculus : public bloc_t{

    token_t::cursor _start;
    token_t::cursor _position;
    token_t::cursor _end;

    token_t::list_t tokens;
    // Children xio_t are implicitely link to this instance of bloc_t;
    xio_t*        _instruction = nullptr; ///< Result instruction. (Call to the root of the expression)

    const char* _text= nullptr;
public:
    calculus();
    ~calculus() override;

    xio_t::result operator [](const std::string& a_expr);

    xio_t::result build();

};

}

