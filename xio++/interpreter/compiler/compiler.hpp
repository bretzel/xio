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

//#include <xio++/interpreter/compiler/grammar.hpp>
#include <xio++/interpreter/kernel/bloc.hpp>
#include <utility>
#include <stack>
#include <xio++/interpreter/compiler/ast.hpp>

namespace xio {

/**
 * @todo write docs
 */
class xio_api compiler
{


    xioast      m_ast;
    astnode*    m_ast_node = nullptr;

    using aeb_t     = std::pair<type_t::T, type_t::T>;
    using result    = xio_t::result;
    using parser_t  = result(compiler::*)(astnode*);
    using parsers_t = std::map<std::string, compiler::parser_t>;

    token_t::list_t* tokens = nullptr; ///< Master Stream;


public:

    struct xio_api config_t {
        const char*         src=nullptr;
        token_t::list_t* tokens = nullptr;
        bloc_t* bloc = nullptr;
    };


    struct xio_api context_t{
        object::iterator ast_node;

        //token_t::cursor cursor;      /// local instance
        bloc_t*         bloc         = nullptr; /// local instance        
        xio_t*          instruction  = nullptr; /// bloc entry instruction
        xio_t::list_t   i_seq;

        xio_t::storage_attr st = { 0,0,0,0 };
        type_t::T _type = type_t::null;
        bloc_t* _object = nullptr;

        //...
        context_t();
        context_t(context_t&& /* ... */) noexcept;
        context_t(const context_t& );
        context_t(bloc_t* a_bloc, object::iterator a_node);
        
        ~context_t();

        context_t& operator = (context_t&&) noexcept;
        context_t& operator = (const context_t&) noexcept;
        
        context_t& operator ++();
        context_t& operator ++(int);

        bloc_t* query_object(const std::string& oid);

        bool empty() { return i_seq.empty(); }

        xio_t* asm_expr();

        //token_t* token() { return &(*cursor); }
        void accepted();
        void rejected();

        using stack = std::stack<compiler::context_t>;
        astnode* node() { return (*ast_node)->me<astnode>(); }
    };
    
    
private:
    context_t ctx;
    config_t cfg;

    static std::vector<aeb_t> aeb_table;
    static bool validate(const compiler::aeb_t& ab);

    //parsers_t parsers;
    bool _eof();

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
   

    compiler::context_t& context_config() { return ctx; }
    compiler::config_t& config() { return cfg; }
    xio_t::result compile(const std::string& rname="");
private:
    
    
    compiler::context_t::stack ctx_stack;
    static parsers_t parsers;
    message::code push_context(bloc_t* a_newbloc=nullptr);
    message::code pop_context();

    result __cc__(astnode* a_node, std::function<compiler::result(astnode*)> cc);

    type_t::T get_type(mnemonic a_code);

    void cleanup_ctx();


    //result cc_stmts      (astnode*);
    //result cc_statement  (astnode*);
    result cc_assignstmt (astnode*);
    result cc_declvar    (astnode*);
    //result cc_funcsig    (astnode*);
    //result cc_declfunc   (astnode*);
    //result cc_paramseq   (astnode*);
    //result cc_param      (astnode*);
    //result cc_params     (astnode*);
    //result cc_objcarg    (astnode*);
    //result cc_arg        (astnode*);
    //result cc_argseq     (astnode*);
    //result cc_args       (astnode*);
    result cc_typename   (astnode*);
    //result cc_instruction(astnode*);
    //result cc_kif        (astnode*);
    //result cc_bloc       (astnode*);
    //result cc_truebloc   (astnode*);
    //result cc_elsebloc   (astnode*);
    //result cc_ifbody     (astnode*);
    //result cc_condexpr   (astnode*);
    result cc_expression (astnode*);
    result cc_var_id     (astnode*);
    result cc_new_var    (astnode*);
    result cc_value      (astnode*);
    //result cc_objectid   (astnode*);
    //result cc_function_id(astnode*);
    //result cc_objcfncall (astnode*);

};


}
