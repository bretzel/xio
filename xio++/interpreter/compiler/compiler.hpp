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

#pragma once

#include <xio++/interpreter/compiler/grammar.hpp>
#include <xio++/interpreter/kernel/stack.hpp>
#include <utility>

namespace xio {

/**
 * @todo write docs
 */
class compiler
{
    
    using aeb_t = std::pair<type_t::T, type_t::T>;
    using result = expect<token_t::cursor>;
    using parser_t = result(compiler::*)(rule_t*);
    using parsers_t = std::map<std::string, compiler::parser_t>;

    struct context_t{
        token_t::list_t* tokens;    /// ref
        token_t::cursor  cursor;    /// local instance
        xio_stack*       bloc       = nullptr; /// local instance
        xio_t*           instruction= nullptr; /// local instance
        xio_t::list_t*   i_seq      = nullptr; /// local seq,

        //...
        context_t();
        context_t(context_t&& /* ... */);
        context_t(const context_t& );
        ~context_t();

        
    }ctx;
    
    static std::vector<aeb_t> aeb_table;
    static bool validate(const compiler::aeb_t& ab);

    parsers_t parsers;
public:
    
    /**
     * Default constructor
     */
    compiler();
    compiler(const compiler& other) = delete;
    ~compiler();

    compiler& operator=(const compiler& other) = delete;
    bool operator==(const compiler& other) const = delete;
    bool operator!=(const compiler& other) const = delete;
   

private:
    result __cc__(rule_t* r, std::function<compiler::result(const term_t&)> cc);

    result cc_stmts      (rule_t*);
    result cc_statement  (rule_t*);
    result cc_assignstmt (rule_t*);
    result cc_declvar    (rule_t*);
    result cc_funcsig    (rule_t*);
    result cc_declfunc   (rule_t*);
    result cc_paramseq   (rule_t*);
    result cc_param      (rule_t*);
    result cc_params     (rule_t*);
    result cc_objcarg    (rule_t*);
    result cc_arg        (rule_t*);
    result cc_argseq     (rule_t*);
    result cc_args       (rule_t*);
    result cc_typename   (rule_t*);
    result cc_instruction(rule_t*);
    result cc_kif        (rule_t*);
    result cc_bloc       (rule_t*);
    result cc_truebloc   (rule_t*);
    result cc_elsebloc   (rule_t*);
    result cc_ifbody     (rule_t*);
    result cc_condexpr   (rule_t*);
    result cc_expression (rule_t*);
    result cc_var_id     (rule_t*);
    result cc_new_var    (rule_t*);
    result cc_objectid   (rule_t*);
    result cc_function_id(rule_t*);
    result cc_objcfncall (rule_t*);
};

}
