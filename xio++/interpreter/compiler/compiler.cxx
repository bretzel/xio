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
#include "../../journal/logger.hpp"

using namespace xio;

//
//std::vector<compiler::aeb_t> compiler::aeb_table = {
//    {type_t::binary,   type_t::leftpar},
//    {type_t::leftpar,  type_t::leaf},
//    {type_t::leftpar,  type_t::prefix},
//    {type_t::leftpar,  type_t::binary},
//    {type_t::prefix,   type_t::leftpar},
//    {type_t::closepar,  type_t::leaf},
//    {type_t::closepar,  type_t::binary},
//    {type_t::closepar,  type_t::postfix},
//    {type_t::closepar,  type_t::closepar},
//
//    {type_t::prefix,   type_t::closepar},
//    {type_t::leaf,     type_t::closepar},
//    {type_t::leaf,     type_t::postfix},
//    {type_t::leaf,     type_t::assign},
//
//
//    {type_t::postfix,  type_t::closepar},
//    {type_t::leftpar,  type_t::binary},
//    {type_t::leaf,     type_t::binary},
//    {type_t::binary,   type_t::binary},
//    {type_t::binary,   type_t::leaf},
//    {type_t::prefix,   type_t::binary},
//    {type_t::binary,   type_t::prefix},
//    {type_t::prefix,   type_t::leaf},
//    {type_t::prefix,   type_t::number},
//    {type_t::sign,     type_t::id},
//    {type_t::sign,     type_t::number},
//    {type_t::sign,     type_t::leaf},
//    {type_t::postfix,  type_t::binary},
//
//    {type_t::assign,   type_t::binary},
//    {type_t::assign,   type_t::leaf},
//    {type_t::assign,   type_t::prefix},
//    {type_t::assign,   type_t::postfix}
//};

//bool xio::compiler::validate(const compiler::aeb_t& ab)
//{
//    for( auto t : compiler::aeb_table )
//        if( t == compiler::aeb_t{ ab.first, t.second & ab.second} ) return true;
//    return false;
//}

compiler::parsers_t compiler::parsers = {
    //{"stmts"         , &compiler::cc_stmts      },
    //{"statement"     , &compiler::cc_statement  },
    {"expr_token"    , &compiler::cc_value      },
    {"assignstmt"    , &compiler::cc_assignstmt },
    {"declvar"       , &compiler::cc_declvar    },
    //{"funcsig"       , &compiler::cc_funcsig    },
    //{"declfunc"      , &compiler::cc_declfunc   },
    //{"paramseq"      , &compiler::cc_paramseq   },
    //{"param"         , &compiler::cc_param      },
    //{"params"        , &compiler::cc_params     },
    //{"objcarg"       , &compiler::cc_objcarg    },
    //{"arg"           , &compiler::cc_arg        },
    //{"argseq"        , &compiler::cc_argseq     },
    //{"args"          , &compiler::cc_args       },
    {"typename"      , &compiler::cc_typename   },
    //{"instruction"   , &compiler::cc_instruction},
    //{"kif"           , &compiler::cc_kif        },
    //{"bloc"          , &compiler::cc_bloc       },
    //{"truebloc"      , &compiler::cc_truebloc   },
    //{"elsebloc"      , &compiler::cc_elsebloc   },
    //{"ifbody"        , &compiler::cc_ifbody     },
    //{"condexpr"      , &compiler::cc_condexpr   },
    {"expression"    , &compiler::cc_expression },
    {"var_id"        , &compiler::cc_var_id     },
    {"new_var"       , &compiler::cc_new_var    },
    //{"objectid"      , &compiler::cc_objectid   },
    //{"function_id"   , &compiler::cc_function_id},
    //{"objcfncall"    , &compiler::cc_objcfncall }
};

bool xio::compiler::_eof()
{
    astnode* node = (*ctx.ast_node)->parent<astnode>();
    return node ? node->end(ctx.ast_node) : m_ast.end(ctx.ast_node);
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
    std::swap(ast_node, ct.ast_node);
    std::swap(i_seq, ct.i_seq);
    std::swap(instruction, ct.instruction);
    std::swap(_object, ct._object);
}

compiler::context_t::context_t(const compiler::context_t & ct) {
    bloc = ct.bloc;
    ast_node = ct.ast_node;
    i_seq = ct.i_seq;
    instruction = ct.instruction;
    _object = ct._object;
}

xio::compiler::context_t::context_t(bloc_t* a_bloc, object::iterator a_astnode)
{
    bloc = a_bloc;
    ast_node = a_astnode;
}

compiler::context_t::~context_t() {
    i_seq.clear();
}

compiler::context_t & xio::compiler::context_t::operator=(context_t && ct) noexcept
{
    std::swap(bloc, ct.bloc);
    std::swap(ast_node, ct.ast_node);
    std::swap(i_seq, ct.i_seq);
    std::swap(instruction, ct.instruction);
    std::swap(_object, ct._object);
    return *this;
}

compiler::context_t & xio::compiler::context_t::operator=(const context_t & ct) noexcept
{
    bloc = ct.bloc;
    ast_node = ct.ast_node;
    i_seq = ct.i_seq;
    instruction = ct.instruction;
    _object = ct._object;
    return *this;
}

compiler::context_t& xio::compiler::context_t::operator++()
{
    //++ast_node;
    return *this;
    // TODO: ins�rer une instruction return ici
}

compiler::context_t& xio::compiler::context_t::operator++(int)
{
    //++ast_node;
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
    if (!cfg.src) return { (
        message::push(message::xclass::error),
        message::code::empty,
        " compiler config is empty! DUH!"
    ) };

    lexer_t lexer;
    lexer_t::result l = lexer(cfg.tokens)[cfg.src];

    if (!l) return { l.notice() };

    tokens = cfg.tokens;
    ctx.bloc = cfg.bloc;

    xio_grammar gr;
    xio_grammar::result t = gr.build();
    if (!t) return { t.notice() };
    token_t::cursor c = tokens->begin();
    ++c;
    std::string start_rule = "stmts";
    if (!rname.empty()) start_rule = rname;

    astnode::result ar = m_ast.build(tokens, rname);

    if (!ar)
        return { ar.notice() };
    m_ast_node = ar.value();
    //
    (void)__cc__((*m_ast.begin())->me<astnode>(), nullptr);
    ////  (void)__cc__(ar.value(),nullptr);
    //
    //  //...
    //
    return { (
        message::push(message::xclass::internal),
        message::code::implement,
        " Ben ouaip! Tout s'est bien passe, mais non! C'est pas encore pret: Analyseurs arretes sur:\n",
        m_ast_node->m_cursor->mark()
    ) };
    //
}

message::code xio::compiler::push_context(bloc_t* a_newbloc)
{
    ctx_stack.push({ a_newbloc ? a_newbloc : ctx.bloc, ctx.ast_node });
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
compiler::result xio::compiler::__cc__(astnode* a_node, std::function<compiler::result(astnode*)> cb)
{
    object::iterator it = a_node->begin();
    xio_t::result xr;
    logdebugfn << logger::White << " entering :" << (*a_node->term_it)() << logger::White << "==>" << logger::Yellow << a_node->m_cursor->attribute() << logger::Reset << Ends;
    while (!a_node->end(it))
    {
        astnode* node = (*it)->me<astnode>();
        logdebugfn << (*node->term_it)() << logger::White << "==>" << logger::Yellow << node->m_cursor->attribute() << logger::Reset << Ends;

        xr = node->is_rule() ? (this->*parsers[node->rule()->_id])(node) :
            cb ? cb(node) :
            (
                message::push(message::xclass::internal),
                (message::code::unexpected),
                " - no allocator(s)/builder(s) for: \n",
                node->m_cursor->mark()
                );
        if (!xr)
            return xr;
        ++it;
    }
    return xr;
}

type_t::T xio::compiler::get_type(mnemonic a_code)
{
    //token_t t = token_t::query(a_code);
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

compiler::result compiler::cc_expression(astnode *node)
{
    auto cr = __cc__(node, [this](astnode* a)->result {
        return { (message::push(message::xclass::internal), message::code::implement) };
    });

    return cr;
}

compiler::result xio::compiler::cc_value(astnode * node)
{
    if (!node->m_cursor->f.v)
    {
        return { (
            message::push(message::xclass::error),
            message::code::syntax,
            "expected a value token, got:\n",
            node->m_cursor->mark()
        ) };
    }

    result xr;
    variable* v = nullptr;

    if (node->m_cursor->type == type_t::id)
    {
        v = ctx.bloc->query_local_variable(node->m_cursor->attribute());
        if (!v)
            v = ctx.bloc->push_variable(node->m_cursor->me(), ctx.st.sstatic, ctx._type)->me<variable>();
    }
    xr = { new xio_t(ctx.bloc, node->m_cursor->me(), v ? v->acc : nullptr) };
    ctx.i_seq.push_back(xr.value());
    ctx._type = type_t::any; //type_t::number ?
    ctx.st.sstatic = 0;
    return xr;

    //return __cc__(node, [this](astnode* a)->result {
    //    // - Default producer call-back from the parent astnode <node>: {a = operator, number, string...}
    //    xio_t* x = new xio_t(ctx.bloc, a->m_cursor->me());
    //    ctx.i_seq.push_back(x);
    //    return { x };
    //});
}

bool xio::compiler::directive_token(const std::string & d_id, const token_t::cursor & d_token)
{
    return false;
}

compiler::result xio::compiler::cc_var_id(astnode *node)
{
    if (compiler::result xr; (xr = ctx.bloc->query_variable(node->m_cursor->attribute())))
    {
        ctx.i_seq.push_back(xr.value());
        return xr;
    }

    return { (
        message::push(message::xclass::error),
        "undefined variable '",
        node->m_cursor->attribute(), "'\n", ///@todo Get the fucking rid of "\n";
        node->m_cursor->mark()
    ) };
}

/*
Failed to locate entry file in A:\Dev\Node\Vue\xio.
Valid entry file should be one of: main.js, index.js, App.vue or app.vue.
*/
//
compiler::result xio::compiler::cc_declvar(astnode *node)
{
    return __cc__(node, [this](astnode*) -> result {
        return { nullptr };
    });
}
//
//
////compiler::result xio::compiler::build_ast(ast* a_ast)
////{
////
////    return {};
////}
//
//compiler::result xio::compiler::cc_stmts(astnode *node)
//{
//
//    auto cr =__cc__(rule, [this] (const term_t& t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//    });
//    return {  };
//}
//
//compiler::result xio::compiler::cc_statement(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//    });
//    return {  };
//}
//
compiler::result xio::compiler::cc_assignstmt(astnode *node)
{
    return __cc__(node, [this](astnode* a) -> result {
        return { (message::push(message::xclass::internal), message::code::implement) };
    });
}

//
//compiler::result xio::compiler::cc_funcsig(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return {  };
//}
//
//compiler::result xio::compiler::cc_declfunc(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return {  };
//}
//
//compiler::result xio::compiler::cc_paramseq(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return {  };
//}
//
//compiler::result xio::compiler::cc_param(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//
//    return {  };
//}
//
//compiler::result xio::compiler::cc_params(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//
//    return {  };
//}
//
//compiler::result xio::compiler::cc_objcarg(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//
//    return { };
//}
//
//compiler::result xio::compiler::cc_arg(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//
//    return {  };
//}
//
//compiler::result xio::compiler::cc_argseq(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//    return {  };
//}
//
//compiler::result xio::compiler::cc_args(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//
//}
//
//
//// static i16 a;
//
//
static bool _static = false;
compiler::result xio::compiler::cc_typename(astnode *node)
{
    compiler::result cr = __cc__(node, [this](astnode* a) -> result {
        if (a->term_it->_type == term_t::type::code) {
            if (a->m_cursor->code == mnemonic::kstatic)
            {
                if (_static) return { (
                    message::push(message::xclass::error),
                    message::code::unexpected,"\n",
                    a->m_cursor->mark()
                ) };
                _static = true;
                ctx.st.sstatic = 1;// Static storage - no matter where.
                return { nullptr };
            }
            else
            {
                ctx._type = get_type(a->term_it->mem.c);
                return { nullptr };
            }
        }
        // ---------------------------------------------------------------------------
        ///@todo
        return { nullptr };
    });

    _static = false;
    return { nullptr };
}

//
//
//compiler::result xio::compiler::cc_instruction(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//    return {  };
//}
//
//compiler::result xio::compiler::cc_kif(astnode *node)
//{
//    //xio_if kif = new xio_if(ctx.bloc, *ctx.cursor, nullptr);
//    //push_context(kif, ctx.cursor);
//
//    auto cr = __cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//    return {  };
//}
//
//compiler::result xio::compiler::cc_bloc(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//    return { };
//}
//
//compiler::result xio::compiler::cc_truebloc(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//}
//
//compiler::result xio::compiler::cc_elsebloc(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//}
//
//compiler::result xio::compiler::cc_ifbody(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//}
//
//compiler::result xio::compiler::cc_condexpr(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//}
//

//
compiler::result xio::compiler::cc_new_var(astnode *node)
{
    if (!node->m_cursor->is_identifier())
        return{ (
            message::push(message::xclass::error),
            message::code::syntax,
            ": ",
            node->m_cursor->informations(),
            " is not an identifier.\n",
            node->m_cursor->mark()
        ) };

    // .. Create new variable
    variable* v = ctx.bloc->query_local_variable(node->m_cursor->attribute());
    if (v) {
        return {
            (message::push(message::xclass::error) , "identifier already exists.", node->m_cursor->mark())
        };
    }
    result xr = { ctx.bloc->push_variable(node->m_cursor->me(),ctx.st.sstatic,ctx._type) };
    ctx._type = type_t::any; //type_t::number ?
    ctx.st.sstatic = 0;
    return xr;
}

//
//
//
//compiler::result xio::compiler::cc_objectid(astnode *node)
//{
//    auto cr = __cc__(rule, [this] (const term_t & t) -> result {
//
//
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//    });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//}
//
//compiler::result xio::compiler::cc_function_id(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//}
//
//compiler::result xio::compiler::cc_objcfncall(astnode *node)
//{
//    (void)__cc__(rule, [this] (const term_t & t) -> result {
//
//        return { (message::push(message::xclass::internal), message::code::implement) };
//        });
//    return { (message::push(message::xclass::internal) , message::code::implement, ctx.cursor->mark()) };
//}
//
//