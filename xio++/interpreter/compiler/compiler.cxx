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




compiler::result xio::compiler::__cc__(rule_t * r, std::function<compiler::result(const term_t&)> cc)
{
    auto start_token = ctx.cursor;
    auto seq_it = r->begin();
    bool nomatch = false;
    while( !r->end(seq_it) ) {
        const seq_t& seq = *seq_it;
        auto term_it = seq.begin();
        do {

            switch( term_it->_type ) {
                case term_t::type::rule:
                {
                    result res = (this->*parsers[term_it->mem.r->_id])(term_it->mem.r);
                    if( !res ) {

                    }
                }
                case term_t::type::code:
                    if( ctx.cursor->code != term_it->mem.c ) nomatch = true;
                    break;

                // ...
                default:break;
            }

            if( nomatch ) {
                if( term_it->a.z ) {
                    ++term_it;
                    continue;
                }
            }
            if( term_it->a.l ) {
                ++ctx.cursor;
                break;
            }


        } while( !seq.end(term_it) );
        if( !nomatch )
            return { &(*start_token) };
    }



    return { nullptr };

}


compiler::result xio::compiler::cc_expression(rule_t *r)
{
    token_t::cursor start_pos = ctx.cursor;

    return { nullptr };
}


compiler::result xio::compiler::cc_stmts(rule_t * rule)
{
    
    __cc__(rule, [ ](const term_t& t) -> result {
        return { nullptr };
    });
    return { nullptr };
}

compiler::result xio::compiler::cc_statement(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_assignstmt(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_declvar(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_funcsig(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_declfunc(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_paramseq(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_param(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_params(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_objcarg(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_arg(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_argseq(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_args(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_typename(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_instruction(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_kif(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_bloc(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_truebloc(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_elsebloc(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_ifbody(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_condexpr(rule_t *)
{
    return { nullptr };
}


compiler::result xio::compiler::cc_var_id(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_new_var(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_objectid(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_function_id(rule_t *)
{
    return { nullptr };
}

compiler::result xio::compiler::cc_objcfncall(rule_t *)
{
    return { nullptr };
}
