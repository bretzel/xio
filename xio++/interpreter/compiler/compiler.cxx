/*
 * Copyright (c) 2018 Serge Lussier <lussier.serge@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "compiler.hpp"

using namespace xio;


std::vector<compiler::aeb_t> compiler::aeb_table = {
    {type_t::binary,   type_t::leftpar},
    {type_t::leftpar,  type_t::leaf},
    {type_t::leftpar,  type_t::prefix},
    {type_t::leftpar,  type_t::binary},
    {type_t::prefix,   type_t::leftpar},
    {type_t::closepar,  type_t::leaf},
    {type_t::closepar,  type_t::binary},
    {type_t::closepar,  type_t::postfix},
    {type_t::closepar,  type_t::closepar},

    {type_t::prefix,   type_t::closepar},
    {type_t::leaf,     type_t::closepar},
    {type_t::leaf,     type_t::postfix},
    {type_t::leaf,     type_t::assign},


    {type_t::postfix,  type_t::closepar},
    {type_t::leftpar,  type_t::binary},
    {type_t::leaf,     type_t::binary},
    {type_t::binary,   type_t::binary},
    {type_t::binary,   type_t::leaf},
    {type_t::prefix,   type_t::binary},
    {type_t::binary,   type_t::prefix},
    {type_t::prefix,   type_t::leaf},
    {type_t::prefix,   type_t::number},
    {type_t::sign,     type_t::id},
    {type_t::sign,     type_t::number},
    {type_t::sign,     type_t::leaf},
    {type_t::postfix,  type_t::binary},

    {type_t::assign,   type_t::binary},
    {type_t::assign,   type_t::leaf},
    {type_t::assign,   type_t::prefix},
    {type_t::assign,   type_t::postfix}
};

bool xio::compiler::validate(const compiler::aeb_t& ab)
{
    for( auto t : compiler::aeb_table )
        if( t == compiler::aeb_t{ ab.first, t.second & ab.second} ) return true;
    return false;
}

// -3-3; sign number bin number
compiler::compiler()
{

}


compiler::~compiler()
{

}

compiler::context_t::context_t() {

}

compiler::context_t::context_t(compiler::context_t &&) {

}

compiler::context_t::context_t(const compiler::context_t &) {

}

compiler::context_t::~context_t() {

}
