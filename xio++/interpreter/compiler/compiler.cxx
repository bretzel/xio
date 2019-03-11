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

compiler::context_t::context_t(compiler::context_t && ct) noexcept {
    std::swap(bloc, ct.bloc);
    std::swap(cursor, ct.cursor);
    std::swap(i_seq, ct.i_seq);
    std::swap(instruction, ct.instruction);
}


compiler::context_t::context_t(const compiler::context_t & ct) {
    bloc = ct.bloc;
    cursor = ct.cursor;
    i_seq = ct.i_seq;
    instruction = ct.instruction;
}

xio::compiler::context_t::context_t(bloc_t* a_bloc, token_t::cursor a_cursor)
{
    bloc = a_bloc;
    cursor = a_cursor;
}


compiler::context_t::~context_t() {
    i_seq.clear();
}

compiler::context_t & xio::compiler::context_t::operator=(context_t && ct) noexcept
{
    std::swap(bloc, ct.bloc);
    std::swap(cursor, ct.cursor);
    std::swap(i_seq, ct.i_seq);
    std::swap(instruction, ct.instruction);

    return *this;
    
}

compiler::context_t & xio::compiler::context_t::operator=(const context_t & ct) noexcept
{
    bloc= ct.bloc;
    cursor= ct.cursor;
    i_seq= ct.i_seq;
    instruction= ct.instruction;
    return *this;
}

compiler::context_t& xio::compiler::context_t::operator++()
{
    ++cursor;
    return *this;
    // TODO: insérer une instruction return ici
}

compiler::context_t& xio::compiler::context_t::operator++(int)
{
    ++cursor;
    return *this;
    // TODO: insérer une instruction return ici
}

void xio::compiler::context_t::accepted()
{
}

void xio::compiler::context_t::rejected()
{
    for (auto x : i_seq) x->discard();
}




message::code xio::compiler::push_context(bloc_t* a_newbloc)
{
    if (a_newbloc)
        ctx_stack.push({ a_newbloc, ctx.cursor });
    else 
        ctx_stack.push(ctx);
    
    ctx = ctx_stack.top();
    return message::code::accepted;
}

message::code xio::compiler::pop_context()
{
    if (ctx_stack.empty())
        return message::code::empty;

    ctx = ctx_stack.top();
    ctx_stack.pop();
    return message::code::accepted;
}

/*!
    @brief "Compiler" entry.
 */
compiler::result xio::compiler::__cc__(rule_t * r, std::function<compiler::result(const term_t&, bool)> cb)
{
    auto start_token = ctx.cursor;
    auto seq_it = r->begin();
    compiler::result cr;
    // Enter rule's sequences iteration:
    auto tit = seq_it->begin();
    while (!r->end(seq_it)) {

        while(!seq_it->end(tit)){
            
            if (tit->_type == term_t::type::rule) {
                ///@todo check that we have a valid delegate ptr then enter rule 
                cr = (this->*parsers[tit->mem.r->_id])(tit->mem.r);
                // ...
                if (cr) {
                    ctx++;
                    if (tit->a.is_one_of())
                        return cr;
                    ++tit;
                    continue;
                }
                // Rule is rejected, then *tit is rejected:
                if (tit->a.is_opt() || tit->a.is_one_of()) {
                    ++tit;
                    continue;
                }
            }
            ///@todo ...
            if (*tit != *ctx.cursor) {
                if (tit->a.is_opt()) continue;
                return cb(*tit, false);
            }

            cr = cb(*tit, true);
            if (!cr) 
                return cb(*tit, false);
        }
    }
    return cb(*tit, cr);
}


compiler::result xio::compiler::cc_expression(rule_t *r)
{
    token_t::cursor start_pos = ctx.cursor;

    return {  };
}


compiler::result xio::compiler::cc_declvar(rule_t *rule)
{
    compiler::result cr = __cc__(rule, [this](const term_t & t, bool accrej) -> result {
        return {};

        });
        // not yet finished!
        return { (message::push(message::xclass::internal), message::code::implement) };
        //...

        // ...
        

}


compiler::result xio::compiler::cc_stmts(rule_t * rule)
{
    
    (void)__cc__(rule, [this] (const term_t& t, bool accrej) -> result {
        
        return { (message::push(message::xclass::internal), message::code::implement) };
    });
    return {  };
}

compiler::result xio::compiler::cc_statement(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
    });
    return {  };
}

compiler::result xio::compiler::cc_assignstmt(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
    });
    return {  };
}


compiler::result xio::compiler::cc_funcsig(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_declfunc(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_paramseq(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_param(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_params(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_objcarg(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return { };
}

compiler::result xio::compiler::cc_arg(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_argseq(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_args(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_typename(rule_t * rule)
{
    compiler::result cr = __cc__(rule, [this] (const term_t & t, bool acc) -> result {

        // ------- If we get explicitly declared the storage class and/or type:
        
        if (acc) {
            if (t._type == term_t::type::code) {
                switch (ctx.cursor->code) {
                case e_code::kstatic:ctx.st.sstatic = 1; break; // Static storage - no matter where.
                case e_code::ki8:ctx._type = type_t::i8; break;
                case e_code::ki16:ctx._type = type_t::i16; break;
                case e_code::ki32:ctx._type = type_t::i32; break;
                case e_code::ki64:ctx._type = type_t::i64; break;
                case e_code::ku8:ctx._type = type_t::u8; break;
                case e_code::ku16:ctx._type = type_t::u16; break;
                case e_code::ku32:ctx._type = type_t::u32; break;
                case e_code::ku64:ctx._type = type_t::u64; break;
                case e_code::kreal:ctx._type = type_t::real; break;
                }
            }
            // ---------------------------------------------------------------------------
            return { ctx.cursor };
        }
    });
    
    return cr;
}


compiler::result xio::compiler::cc_instruction(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_kif(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_bloc(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return { };
}

compiler::result xio::compiler::cc_truebloc(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_elsebloc(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_ifbody(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return { };
}

compiler::result xio::compiler::cc_condexpr(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}


compiler::result xio::compiler::cc_var_id(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_new_var(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_objectid(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_function_id(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {   };
}

compiler::result xio::compiler::cc_objcfncall(rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t, bool accrej) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {   };
}


