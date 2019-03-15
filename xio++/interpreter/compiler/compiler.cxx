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
#include "lexer.hpp"

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




compiler::parsers_t compiler::parsers = {
    {"stmts"         , &compiler::cc_stmts      },
    {"statement"     , &compiler::cc_statement  },
    {"assignstmt"    , &compiler::cc_assignstmt },
    {"declvar"       , &compiler::cc_declvar    },
    {"funcsig"       , &compiler::cc_funcsig    },
    {"declfunc"      , &compiler::cc_declfunc   },
    {"paramseq"      , &compiler::cc_paramseq   },
    {"param"         , &compiler::cc_param      },
    {"params"        , &compiler::cc_params     },
    {"objcarg"       , &compiler::cc_objcarg    },
    {"arg"           , &compiler::cc_arg        },
    {"argseq"        , &compiler::cc_argseq     },
    {"args"          , &compiler::cc_args       },
    {"typename"      , &compiler::cc_typename   },
    {"instruction"   , &compiler::cc_instruction},
    {"kif"           , &compiler::cc_kif        },
    {"bloc"          , &compiler::cc_bloc       },
    {"truebloc"      , &compiler::cc_truebloc   },
    {"elsebloc"      , &compiler::cc_elsebloc   },
    {"ifbody"        , &compiler::cc_ifbody     },
    {"condexpr"      , &compiler::cc_condexpr   },
    {"expression"    , &compiler::cc_expression },
    {"var_id"        , &compiler::cc_var_id     },
    {"new_var"       , &compiler::cc_new_var    },
    {"objectid"      , &compiler::cc_objectid   },
    {"function_id"   , &compiler::cc_function_id},
    {"objcfncall"    , &compiler::cc_objcfncall }
};










bool xio::compiler::_eof()
{
    return ctx.cursor == tokens->end();
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
    std::swap(_object, ct._object);
}


compiler::context_t::context_t(const compiler::context_t & ct) {
    bloc = ct.bloc;
    cursor = ct.cursor;
    i_seq = ct.i_seq;
    instruction = ct.instruction;
    _object = ct._object;
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
    std::swap(_object, ct._object);
    return *this;
    
}

compiler::context_t & xio::compiler::context_t::operator=(const context_t & ct) noexcept
{
    bloc= ct.bloc;
    cursor= ct.cursor;
    i_seq= ct.i_seq;
    instruction= ct.instruction;
    _object = ct._object;
    return *this;
}

compiler::context_t& xio::compiler::context_t::operator++()
{
    ++cursor;
    return *this;
    // TODO: ins�rer une instruction return ici
}

compiler::context_t& xio::compiler::context_t::operator++(int)
{
    ++cursor;
    return *this;
    // TODO: ins�rer une instruction return ici
}


bloc_t* xio::compiler::context_t::query_object(const std::string& oid)
{
    // If the bloc is null then crash! It cannot be null!

    return bloc->query_object(oid);
}

xio_t* xio::compiler::context_t::asm_expr()
{
    
    return nullptr;
}

void xio::compiler::context_t::accepted()
{
}

void xio::compiler::context_t::rejected()
{
    for (auto x : i_seq) {

        x->discard();
    }
}

xio_t::result xio::compiler::compile(const std::string& rname)
{

    if (!cfg.src) return {(
        message::push(message::xclass::error),
        message::code::empty,
        " compiler config is empty! DUH!"
    )};

    lexer_t lexer;
    lexer_t::result l = lexer(cfg.tokens)[cfg.src];
    
    if (!l) return { l.notice() };

    tokens = cfg.tokens;
    ctx.cursor = tokens->begin();
    ctx.bloc = cfg.bloc;
    ++ctx;

    xio_grammar gr;
    xio_grammar::result t = gr.build();
    if (!t) return { t.notice() };
    token_t::cursor c = tokens->begin();
    ++c;
    std::string start_rule = "stmts";
    if (!rname.empty()) start_rule = rname;
    
    result cr = (this->*parsers[start_rule])(gr[start_rule]);
    //...

    return { (
        message::push(message::xclass::internal),
        message::code::implement,
        " Ben ouaip! c'est pas encore pret!\n",
        c->mark()
    ) };

}




message::code xio::compiler::push_context(bloc_t* a_newbloc)
{
    
    ctx_stack.push({ a_newbloc ? a_newbloc : ctx.bloc, ctx.cursor });
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
compiler::result xio::compiler::__cc__(const rule_t * r, std::function<compiler::result(const term_t&)> cb)
{
    auto start_token = ctx.cursor;
    auto seq_it = r->begin();
    compiler::result cr;
    // Enter rule's sequences iteration:
    auto tit = seq_it->begin();
    while (!r->end(seq_it)) {
        bool rep_ok = false;
        while (!seq_it->end(tit)) {
            
            cr.clear();
            if (tit->_type == term_t::type::rule)
                cr = (this->*parsers[tit->mem.r->_id])(tit->mem.r);
            else
                if (*tit == *ctx.cursor)
                    cr = cb(*tit);

            if (!cr) {
                cleanup_ctx();
                if (rep_ok && tit->a.is_repeat()) {
                    ++tit;
                    rep_ok = false;
                }
                else
                    if (!tit->a.is_strict())
                    {
                        ++tit;
                        rep_ok = false;
                        continue;
                    }
                // End repeat:
                // reject the sequence:
                // Cleanup ...

                break;
            }
            // Accepted:

            if ((rep_ok = tit->a.is_repeat())) continue;
            if (tit->a.is_one_of()) // Accept whole rule on (first) hit:
                return cr;
            ++tit;
        }
        ++seq_it;
    }
    return cr;
}

type_t::T xio::compiler::get_type(mnemonic a_code)
{
    token_t t = token_t::query(a_code);
    return
        a_code == mnemonic::ku8 ? type_t::u8 :
        a_code == mnemonic::ku16 ? type_t::u16 :
        a_code == mnemonic::ku32 ? type_t::u32 :
        a_code == mnemonic::ku64 ? type_t::u64 :
        a_code == mnemonic::ki8 ? type_t::i8 :
        a_code == mnemonic::ki16 ? type_t::i16 :
        a_code == mnemonic::ki32 ? type_t::i32 :
        a_code == mnemonic::ki64 ? type_t::i64 :
        a_code == mnemonic::kreal ? type_t::real :
        a_code == mnemonic::knumber ? type_t::number :
        a_code == mnemonic::kstring ? type_t::text : type_t::null;
}

void xio::compiler::cleanup_ctx()
{
    ctx.rejected();
}


compiler::result xio::compiler::cc_expression(const rule_t *r)
{
    auto cr = __cc__(r, [this](const term_t & t)->result {
        return {};
    });

    return {  };
}


compiler::result xio::compiler::cc_declvar(const rule_t *rule)
{
    compiler::result cr = 
    __cc__(rule, [this](const term_t & t) -> result {
               
        return {ctx.cursor};
    });

    if (!cr)
        return cr;


    return { ctx.cursor };
}


compiler::result xio::compiler::build_ast()
{

    return {};
}

compiler::result xio::compiler::cc_stmts(const rule_t * rule)
{
    
    auto cr =__cc__(rule, [this] (const term_t& t) -> result {
        
        return { (message::push(message::xclass::internal), message::code::implement) };
    });
    return {  };
}

compiler::result xio::compiler::cc_statement(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
    });
    return {  };
}

compiler::result xio::compiler::cc_assignstmt(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
    });
    return {  };
}


compiler::result xio::compiler::cc_funcsig(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_declfunc(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_paramseq(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_param(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_params(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_objcarg(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return { };
}

compiler::result xio::compiler::cc_arg(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_argseq(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_args(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}


// static i16 a;


static bool _static = false;
compiler::result xio::compiler::cc_typename(const rule_t * rule)
{
    compiler::result cr = __cc__(rule, [this] (const term_t & t) -> result {
        if (t._type == term_t::type::code) {
            if (ctx.cursor->code == mnemonic::kstatic )
            {
                if (_static) return {/* put descriptive message here */ };
                _static = true;
                ctx.st.sstatic = 1;// Static storage - no matter where.
                ++ctx;
                return { ctx.cursor };
            }
            else
            {
                ctx._type = get_type(t.mem.c);
                ++ctx;
                return { ctx.cursor };
            }
        }
        // ---------------------------------------------------------------------------
        ///@todo 
        return { /* put descriptive message here */ };
    });

    _static = false;
    return cr;
}


compiler::result xio::compiler::cc_instruction(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_kif(const rule_t * rule)
{
    //xio_if kif = new xio_if(ctx.bloc, *ctx.cursor, nullptr);
    //push_context(kif, ctx.cursor);

    auto cr = __cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_bloc(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return { };
}

compiler::result xio::compiler::cc_truebloc(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_elsebloc(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}

compiler::result xio::compiler::cc_ifbody(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return { };
}

compiler::result xio::compiler::cc_condexpr(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {  };
}


compiler::result xio::compiler::cc_var_id(const rule_t * rule)
{
    variable* v = ctx.bloc->query_variable(ctx.cursor->attribute());
    if ( v ) {
        ctx.push_token();
        return { ctx.cursor };
    }
    
    return {(
        message::push(message::xclass::error),
        "undefined variable '",
        ctx.cursor->attribute(), "'",
        ctx.cursor->mark()
    )};

}


compiler::result xio::compiler::cc_new_var(const rule_t * rule)
{
    if (!ctx.cursor->is_identifier())
        return{ (
            message::push(message::xclass::error), 
            message::code::syntax, 
            ": ", 
            ctx.cursor->informations(),
            " is not an identifier.\n",
            ctx.cursor->mark()
        )};
       
    // .. Create new variable
    variable* v = ctx.bloc->query_local_variable(ctx.cursor->attribute());
    if (v) {
        return { 
            (message::push(message::xclass::error) , "identifier already exists.", ctx.cursor->mark())
        };
    }

    (void)ctx.bloc->push_variable(ctx.cursor->me(),ctx.st.sstatic,ctx._type);
    ++ctx;
    ctx.push_token();

    return { ctx.cursor }; 
}



compiler::result xio::compiler::cc_objectid(const rule_t * rule)
{
    auto cr = __cc__(rule, [this] (const term_t & t) -> result {

           
        
        return { (message::push(message::xclass::internal), message::code::implement) };
    });
    return {  };
}

compiler::result xio::compiler::cc_function_id(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {   };
}

compiler::result xio::compiler::cc_objcfncall(const rule_t * rule)
{
    (void)__cc__(rule, [this] (const term_t & t) -> result {

        return { (message::push(message::xclass::internal), message::code::implement) };
        });
    return {   };
}


