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

public:
    calculus();
    ~calculus() override;



};

}

