//
// Created by slussier on 19-08-25.
//

#include <xio/interpreter/xio.h>
#include <cmath>
#include <xio/utils/journal.h>

namespace teacc{


using logger = utils::journal;

xio::shared xio::_global_root_bloc = nullptr;


xio::shared xio::get_global_root() { return xio::_global_root_bloc; }


#define deg2rad(_xio) ((_xio)->acc->number<double>()*3.1415) / 180
constexpr auto _PI_ = 3.14159265358979323846;
// M_PI ?
std::stack<xio::shared> xio::pars;
std::stack<xio::shared> xio::indexes;
std::stack<xio::shared> xio::curls;


// void xio::// discard()
// {
//     for (auto x : xio::xio_gc)
//         if (this == x) return;

//     xio::xio_gc.push_back(this);
//     lhs = rhs = op = nullptr; // Make sure we do not accidentally reuse this;
// }

using token_t = lexer::type::token_t;

// Arithmetic Binary Tree: associative building logic table:
xio::input_table_t      xio::tree_input_assoc_table = {

    {{lexer::type::binary,   lexer::type::leftpar},   &xio::tree_set_right},
    {{lexer::type::leftpar,  lexer::type::leaf},      &xio::tree_set_left},
    {{lexer::type::leftpar,  lexer::type::prefix},    &xio::tree_set_left},
    {{lexer::type::leftpar,  lexer::type::binary},    &xio::tree_set_left},
    {{lexer::type::prefix,   lexer::type::leftpar},   &xio::tree_set_right},
    {{lexer::type::closepar,  lexer::type::leaf},     &xio::tree_rpar_input_leaf},
    {{lexer::type::closepar,  lexer::type::binary},   &xio::tree_close_par},
    {{lexer::type::closepar,  lexer::type::postfix},  &xio::tree_close_par},
    {{lexer::type::closepar,  lexer::type::closepar}, &xio::tree_rpar_rpar},

    {{lexer::type::prefix,   lexer::type::closepar},  &xio::tree_input_rpar},
    {{lexer::type::leaf,     lexer::type::closepar},  &xio::tree_input_rpar},
    {{lexer::type::leaf,     lexer::type::postfix},   &xio::tree_set_right_to_op},
    {{lexer::type::leaf,     lexer::type::assign},    &xio::tree_input_binary},


    {{lexer::type::postfix,  lexer::type::closepar},  &xio::tree_input_rpar},
    {{lexer::type::leftpar,  lexer::type::binary},    &xio::tree_set_left},
    {{lexer::type::leaf,     lexer::type::binary},    &xio::tree_input_binary},
    {{lexer::type::binary,   lexer::type::binary},    &xio::tree_input_binary},
    {{lexer::type::binary,   lexer::type::leaf},      &xio::tree_set_right},
    {{lexer::type::prefix,   lexer::type::binary},    &xio::tree_input_binary},
    {{lexer::type::binary,   lexer::type::prefix},    &xio::tree_set_right},
    {{lexer::type::prefix,   lexer::type::leaf},      &xio::tree_set_right},
    {{lexer::type::prefix,   lexer::type::number},    &xio::tree_set_right},
    {{lexer::type::sign,     lexer::type::id},        &xio::tree_set_right},
    {{lexer::type::sign,     lexer::type::number},    &xio::tree_set_right},
    {{lexer::type::sign,     lexer::type::leaf},      &xio::tree_set_right},
    {{lexer::type::postfix,  lexer::type::binary},    &xio::tree_input_binary},

    {{lexer::type::assign,   lexer::type::binary},    &xio::tree_set_right},
    {{lexer::type::assign,   lexer::type::leaf},      &xio::tree_set_right},
    {{lexer::type::assign,   lexer::type::prefix},    &xio::tree_set_right},
    {{lexer::type::assign,   lexer::type::postfix},   &xio::tree_set_right}

};


// Primary arithmetic supported operators table:
xio::xio_opfn_table_t   xio::xio_operators_table{
    {lexer::lexem::mnemonic::lshift,          &xio::lshift},
    {lexer::lexem::mnemonic::radical,         &xio::radical},
    {lexer::lexem::mnemonic::exponent,        &xio::exponent},
    {lexer::lexem::mnemonic::rshift,          &xio::rshift},
    {lexer::lexem::mnemonic::decr,            &xio::decr},
    {lexer::lexem::mnemonic::incr,            &xio::incr},
    {lexer::lexem::mnemonic::assignadd,       &xio::assignadd},
    {lexer::lexem::mnemonic::assignsub,       &xio::assignsub},
    {lexer::lexem::mnemonic::assignmul,       &xio::assignmul},
    {lexer::lexem::mnemonic::assigndiv,       &xio::assigndiv},
    {lexer::lexem::mnemonic::assignmod,       &xio::assignmod},
    {lexer::lexem::mnemonic::assignand,       &xio::assignand},
    {lexer::lexem::mnemonic::assignor,        &xio::assignor},
    {lexer::lexem::mnemonic::assignxor,       &xio::assignxor},
    {lexer::lexem::mnemonic::assignx1,        &xio::assignx1},
    {lexer::lexem::mnemonic::assignlshift,    &xio::assignlshift},
    {lexer::lexem::mnemonic::assignrshift,    &xio::assignrshift},
    {lexer::lexem::mnemonic::leq,             &xio::leq},
    {lexer::lexem::mnemonic::geq,             &xio::geq},
    {lexer::lexem::mnemonic::eq,              &xio::eq},
    {lexer::lexem::mnemonic::neq,             &xio::neq},
    {lexer::lexem::mnemonic::add,             &xio::add},
    {lexer::lexem::mnemonic::sub,             &xio::sub},
    {lexer::lexem::mnemonic::mul,             &xio::mul},
    {lexer::lexem::mnemonic::modulo,          &xio::modulo},
    {lexer::lexem::mnemonic::lt,              &xio::lt},
    {lexer::lexem::mnemonic::gt,              &xio::gt},
    {lexer::lexem::mnemonic::assign,          &xio::assign},
    {lexer::lexem::mnemonic::binand,          &xio::binand},
    {lexer::lexem::mnemonic::binor,           &xio::binor},
    {lexer::lexem::mnemonic::bitxor,          &xio::bitxor},
    {lexer::lexem::mnemonic::x1,              &xio::x1},
    {lexer::lexem::mnemonic::x2,              &xio::x2},
    {lexer::lexem::mnemonic::bitnot,          &xio::bitnot},
    {lexer::lexem::mnemonic::booland,         &xio::booland},
    {lexer::lexem::mnemonic::boolor,          &xio::boolor},
    {lexer::lexem::mnemonic::division,        &xio::division},
    {lexer::lexem::mnemonic::factorial,       &xio::factorial},
    {lexer::lexem::mnemonic::positive,        &xio::positive},
    {lexer::lexem::mnemonic::negative,        &xio::negative},
    {lexer::lexem::mnemonic::kpi,             &xio::kpi},
    {lexer::lexem::mnemonic::kcos,            &xio::kcos},
    {lexer::lexem::mnemonic::kacos,           &xio::kacos},
    {lexer::lexem::mnemonic::ktan,            &xio::ktan},
    {lexer::lexem::mnemonic::katan,           &xio::katan},
    {lexer::lexem::mnemonic::ksin,            &xio::ksin},
    {lexer::lexem::mnemonic::kasin,           &xio::kasin},
    {lexer::lexem::mnemonic::knumber,         &xio::knumber},
    {lexer::lexem::mnemonic::ku8,             &xio::ku8   },
    {lexer::lexem::mnemonic::ku16,            &xio::ku16  },
    {lexer::lexem::mnemonic::ku32,            &xio::ku32  },
    {lexer::lexem::mnemonic::ku64,            &xio::ku64  },
    {lexer::lexem::mnemonic::ki8,             &xio::ki8   },
    {lexer::lexem::mnemonic::ki16,            &xio::ki16  },
    {lexer::lexem::mnemonic::ki32,            &xio::ki32  },
    {lexer::lexem::mnemonic::ki64,            &xio::ki64  },
    {lexer::lexem::mnemonic::kreal,           &xio::kreal },
    {lexer::lexem::mnemonic::kstring,         &xio::kstring}
};


// xio::xio()
// {
//     //acc = std::make_shared<alu>(0.0);
// }


xio::xio(xio::shared a_parent):_parent(a_parent)
{
    //t0 = new token_t;
    acc = std::make_shared<alu>(0.0);
}

xio::xio(xio::shared a_parent, token_t * a_token, alu::shared a_alu):_parent(a_parent)
{
    t0 = a_token;
    logdebugfn << "xio::xio this->t:[" << lexer::type::to_s(t0?t0->s:lexer::type::null) << "'" << (t0?t0->attribute():"no token") <<  "'] \n";

    if(acc != nullptr)
    {
        acc = a_alu;
        return;
    }

    if (!t0)
    {
        acc = std::make_shared<alu>(0.0);
        return;
    }

    switch(t0->t){
        case lexer::type::text:
            acc = std::make_shared<alu>(a_token->attribute());
            return;
        case lexer::type::number:
        {
            ///@todo Attention enlever le court-circuit du type-size!!!
            double d;
            (utils::xstr(a_token->attribute())) >> d;
            acc = std::make_shared<alu>(d);
        }
            return;
        case lexer::type::hex:
        {
            uint64_t d;
            utils::xstr(a_token->attribute()).hex(d);
            acc = std::make_shared<alu>(d);
            return;
        }
            return;
        case lexer::type::any:
        case lexer::type::voidptr:
            acc = std::make_shared<alu>((void*)nullptr);
            return;
        default:
            acc = std::make_shared<alu>(0.0f);
            break;
    }

    auto i = xio::xio_operators_table.find(a_token->c);
    xio_fn = i != xio::xio_operators_table.end() ? i->second : nullptr;

}

xio::shared xio::make(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_acc)
{
    return std::make_shared<xio>(a_parent, a_token, a_acc);
}

xio::shared xio::copy(xio::shared a_)
{
    return nullptr;
}


// xio::~xio()
// {

// }



// -------------- ARITHMETIC BINARY TREE INPUT LOGIC IMPLEMENTATION ------------------------------------
/*
        a = 32 * (2+3) / 4;

                    =           =               =              =              =            =              =               = <---+           =
                   / \         / \             / \            / \            / \          / \            / \             / \    |          / \
                  a  [/]      a   *  <- (     a   *          a   *          a   *        a   *          a   *           a   * <-|         a   /
                     /  \        /               / \            / \            / \          / \            / \             / \  |            / \
                    *    4 ;    32              32  ( <- 2     32  (          32  (        32 ( <--+      32  ) <- /      32  + /           *   4;
                   / \                                            /              /           /     |         /               / \           / \
                  32  +                                          2 <- +         + <- 3      +      |        +               2   3         32  +
                     / \                                                       /           / \     +       / \                               / \
                    2   3                                                     2           2   3 <- )      2   3                             2   3
*/
xio::expect xio::tree_input(token_t * a_token, xio::maker invoke_maker)
{
    inptr_fn_t pfn = nullptr;
    for(auto key : xio::tree_input_assoc_table)
    {
        if((key.first.first & t0->s) && (a_token->s & key.first.second))
        {
            logdebugfn << logger::White
                     << "'" << logger::Yellow
                     << t0->attribute()
                     << logger::White  << "' - Key:"
                     << logger::Yellow << lexer::type::to_s(key.first.first)
                     << logger::White  << " <=> "
                     << logger::Blue  << __FUNCTION__
                     << logger::White  << ":'"
                     << logger::Yellow << a_token->attribute()
                     << logger::White  << "' - Key:"
                     << logger::Yellow << lexer::type::to_s(key.first.second)
                     << logger::White  << ":"
                     << Ends;

            pfn = key.second;
            break;
        }
    }
    if (pfn)
        return (this->*pfn)(invoke_maker(a_token));

    return
        {(
             utils::notification::push(utils::notification::type::error),
                 ":(debug): syntax error, unexpected token:",
                 a_token->attribute(),
                 "\n",
                 a_token->mark()
         )};
}



void xio::push_lpar(xio::shared lpar)
{
    logdebugpfn << Ends;
    xio::pars.push(lpar);
}

xio::shared xio::pop_lpar()
{
    if (xio::pars.empty()) return nullptr;
    xio::shared x = xio::pars.top();
    xio::pars.pop();
    return x;
}


xio::expect xio::tree_input_binary(xio::shared x) {

    logdebugfn
        << logger::Yellow << t0->attribute()
        << logger::White << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << x->t0->attribute()
        << Ends;

    if (t0->is_binary()) {
        if (!rhs) {
            return { (
                         utils::notification::push(utils::notification::type::error),
                             " binary operator has no right hand side operand, then I cannot input this operator.",
                             t0->attribute(),t0->mark()
                     )};
        }
        if (t0->d >= x->t0->d) {
            return tree_set_right(x);
        }
    }

    if (op) {
        inptr_fn_t ptr = associate(op, x); // "has_assoc_pair(op,x); ??
        if (ptr)
            return (op.get()->*ptr)(x);

    }

    if(_parent)
    {
        x->tree_set_left(_parent->query_child(this));
    }
    return { x };
}


xio::expect xio::tree_input_leaf(xio::shared x) {

    logdebugfn
        << logger::Yellow << t0->attribute()
        << logger::White  << ":"
        << logger::HBlue  << __FUNCTION__
        << logger::White  << ":"
        << logger::Yellow << x->t0->attribute()
        << Ends;

    if(!t0->is_operator())
        return {(
                    utils::notification::push(utils::notification::type::error),
                        "Expected an operator after a value operand.",
                        x->t0->mark()
                )};

    if(t0->is_binary()){
        if(!lhs)
            return {(
                        utils::notification::push(utils::notification::type::error),
                            " binary operator has no left hand side operand.",
                            t0->attribute(),
                            t0->mark()
                    )};
    }

    ///@Todo Reach interpreter :: mark (token_t*, or xio::shared  );
    return tree_set_right(x);
}

/*!

    @note Attention: call tree_set_left only when isolating a sub-expressiom such as '('; '['; '{'.  
    -( 5! )


          -
           \
            (
           /
          ! <- +  
         /
        5  
*/
xio::expect xio::tree_set_left(xio::shared x)
{
    logdebugfn
        << logger::Yellow << t0->attribute()
        << logger::White  << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << x->t0->attribute()
        << Ends;

/*
          (;[;{   // Appliqué sur aucun autre type de token car l'appel de tree_set_left ne se fait qu'� partir de tree_input qui r�soud l'associativit�.
           /
          x <- next_token
         /
       lhs
 */

    if (lhs) {
        // here we are supposed to be the openning par/index/bracket. so the interior will become right hand side of the parent op of this.
        lhs->op = x;
        x->lhs = lhs;
    }
    if(_parent)
        x->op = _parent->query_child(this);

    lhs = x;
    return {x};
}


/*

    /             /        
     \             \       
      a < (         ( < 2  
                   /       
                  a        
*/
xio::expect xio::tree_set_right(xio::shared x) {
    logdebugfn
        << logger::Yellow << t0->attribute()
        << logger::White << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << x->t0->attribute()
        << Ends;

    // Temporary hack....
    if (x->t0->c == lexer::lexem::mnemonic::openpar)
        push_lpar(x);

    if (rhs) {
        /*
           this
              \
               x <- next_token
              /
            rhs
        */
        logdebugfn << t0->attribute() << " -> " << rhs->t0->attribute()
                 << logger::Black << "tree_set_right "
                 << logger::White << "<- "
                 << logger::Yellow << x->t0->attribute()
                 <<   Ends;
        rhs->op = x;
        x->lhs = rhs;
    }
    rhs = x;
    if(_parent)
        x->op = _parent->query_child(this);
    return {x};
}


xio::expect xio::tree_set_right_to_op(xio::shared x)
{
    logdebugfn << logger::Yellow << t0->attribute()
             << logger::White << ":"
             << logger::Black << __FUNCTION__
             << logger::White << ":"
             << logger::Yellow << x->t0->attribute()
             << Ends;

    if (!op) {
        x->lhs = _parent->query_child(this);
        op = x;
        return { x };
    }
    return op->tree_set_right(x);
}


xio::inptr_fn_t xio::associate(xio::shared a_lhs, xio::shared a_rhs)
{
    for(auto key : xio::tree_input_assoc_table){
        //if ((key.first.first == a_lhs->t0->t) && (a_rhs->t0->t & key.first.second)) {
        if ((key.first.first & a_lhs->t0->s) && (a_rhs->t0->s & key.first.second)) {
            return key.second;
        }
    }
    return nullptr;
}


xio::expect xio::tree_close()
{
    logdebugfn << Ends;
    if (t0->c == lexer::lexem::mnemonic::openpar)
        return {(
                    utils::notification::push(utils::notification::type::error),
                        " unexpected end of file."
                )};
    if (!xio::pars.empty()) {

        return { (
                     utils::notification::push(utils::notification::type::error),
                         " umatched closing parenthese."
                 ) };
    }

    if (t0->c == lexer::lexem::mnemonic::closepar) {
        logdebugfn << "Closing the tree on close parenthese:" << Ends;
        if (lhs) {
            xio::shared x = lhs;
            lhs->op = op;

            if (op) {
                op->rhs = lhs;
            }
            // discard();
            return { x->tree_root() };
        }
    }
    return { tree_root() };
}


xio::expect xio::tree_root()
{
    xio::shared x = _parent->query_child(this);
    xio::shared p = x;
    do{
        x = p;
        switch (x->t0->t) {
            case lexer::type::assign:
            case lexer::type::binary:
                if (!x->lhs)
                    return {(
                                utils::notification::push(utils::notification::type::error), "Syntax error: binary operator is missing its left operand.\n", x->t0->mark()
                            )};
                if (!x->rhs)
                    return {(
                                utils::notification::push(utils::notification::type::error), "Syntax error: binary operator is missing its right operand.\n", x->t0->mark()
                            )};
                break;
            case lexer::type::prefix:
                if (!x->rhs)
                    return { (
                                 utils::notification::push(utils::notification::type::error), "Syntax error: prefix unary operator is missing its (right) operand.\n", x->t0->mark()
                             )};
                break;
            case lexer::type::postfix:
                if (!x->lhs)
                    return { (
                                 utils::notification::push(utils::notification::type::error), "Syntax error: postfix unary operator is missing its (left) operand.\n", x->t0->mark()
                             )};
                break;
        }

        p = p->op;
    } while (p);
    return  x ;
}

xio::expect xio::tree_lpar_input_binary(xio::shared x)
{
    if (!lhs) {
        return {(
            utils::notification::push(utils::notification::type::error),
            " syntax error : \n", t0->mark()
        )};
    }
    return tree_set_left(_parent->query_child(this));
}


xio::expect xio::tree_input_rpar(xio::shared rpxio)
{
    logdebugfn
        << logger::Yellow << t0->attribute()
        << logger::White << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << rpxio->t0->attribute()
        << Ends;

    xio::shared x = xio::pop_lpar();
    if (!x)
        return {(
                    utils::notification::push(utils::notification::type::error),
                        "Unmatched left paranthese.",
                        rpxio->t0->mark()
                )};

    rpxio->op = x->op;
    rpxio->lhs = x->lhs;
    rpxio->lhs->op = rpxio;
    rpxio->op->rhs = rpxio; // oops!!
    logdebugfn << "new input vertex:[" << logger::Yellow <<  rpxio->t0->attribute() << logger::Reset << "]" << Ends;
    
    return { rpxio };
}


xio::expect xio::tree_input_lpar(xio::shared x)
{

    logdebugfn
        << logger::Yellow << t0->attribute()
        << logger::White << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << x->t0->attribute()
        << Ends;

    // Binary input left par -> tree_set_right is called directly.
    return tree_set_right(x);
}

xio::expect xio::tree_close_par(xio::shared x)
{
    logdebugfn
        << logger::Yellow << t0->attribute()
        << logger::White << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << x->t0->attribute()
        << Ends;    // Binary input left par -> tree_set_right is called directly.

    xio::shared v = lhs;

    // Collapse lhs

    v->op = op;
    if (op) op->rhs = v;

    // discard();

    if (v->op) {
        auto p_fn = associate(v->op, x);
        if (!p_fn) {
            utils::xstr str = t0->mark();
            return { (utils::notification::push(utils::notification::type::error), "syntax error on:", str()) };
        }
        return (v->op.get()->*p_fn)(x);
    }

    v->op = x;
    x->lhs = v;
    return {x};
}


xio::expect xio::tree_rpar_input_leaf(xio::shared x)
{
    logdebugfn << logger::Yellow << t0->attribute()
             << logger::White << ":"
             << logger::Black << __FUNCTION__
             << logger::White << ":"
             << logger::Yellow << x->t0->attribute()
             << Ends;    // Binary input left par -> tree_set_right is called directly.

    if (lhs) {
        logdebugfn << "lhs:" << logger::Yellow << lhs->t0->attribute()
                 << logger::White << Ends;

        if (lhs->t0->is_prefix()) {
            if (op) {
                logdebugfn << logger::Yellow  << lhs->t0->attribute() << logger::White << "'s operator:`" << logger::Yellow << lhs->op->t0->attribute() << logger::Reset << "':" << Ends;
                //op->tree_set_right(lhs);
                lhs->op = op;
                op->rhs = lhs;

            }
            xio::shared xx = lhs;
            // discard();

            return xx->tree_set_right(x);
        }
    }
    return {(utils::notification::push(utils::notification::type::error), "illegal rvalue token :", x->t0->mark())};
}


xio::expect xio::tree_rpar_rpar(xio::shared r)
{
    logdebugfn << logger::Yellow << t0->attribute()
             << logger::White << ":"
             << logger::Black << __FUNCTION__
             << logger::White << ":"
             << logger::Yellow << r->t0->attribute()
             << Ends;    // Binary input left par -> tree_set_right is called directly.

    // Collapse lhs

    r->op = op;
    r->lhs = lhs;
    r->lhs->op = r;
    if (op) op->rhs = r;

    // discard();

    return { r };
}


// ------------------ ARITHMETIC OPERATORS IMPLEMENTATION ---------------------------------------------

alu xio::jsr()
{
    //...

    if (lhs) *acc = lhs->jsr(); // Always catch the lhs value so we return that one if there is no rhs operand.
    if (rhs) *acc = rhs->jsr(); // Always catch the rhs value because it is the value to be returned after being applied to the lhs (if applicable).

    if (xio_fn)
        return (this->*xio_fn)();// All operators assign acc.
    else {
        if (t0->is_operator()) {
            logwarningfn << "xio [" << logger::Yellow << t0->attribute() << logger::Reset << "] has no rtfn (yet?).:\n" << t0->mark() << Ends;
        }
    }
    t0->s |= acc->_type;
    return *acc;
}


alu xio::lshift()
{
    *acc = lhs->acc->number<uint64_t>() << rhs->acc->number<uint64_t>();
    return *acc;
}

alu xio::radical()
{
    *acc = std::pow(lhs->acc->number<double>(), 1/rhs->acc->number<double>());
    return *acc;
}

alu xio::exponent()
{
    *acc = std::pow(lhs->acc->number<double>(), rhs->acc->number<double>());
    return *acc;
}

alu xio::rshift()
{
    *acc = lhs->acc->number<uint64_t>() >> rhs->acc->number<uint64_t>();
    return *acc;
}


alu xio::decr()
{
    *acc = t0->is_prefix() ? --(*rhs->acc) : (*lhs->acc)--;
    return *acc;
}


alu xio::incr()
{
    *acc = t0->is_prefix() ? ++(*rhs->acc) : (*lhs->acc)++;
    return *acc;
}


alu xio::assignadd()
{
    *acc = *(lhs->acc) += *(rhs->acc);
    return  *acc;
}


alu xio::assignsub()
{
    *acc = *(lhs->acc) -= *(rhs->acc);
    return  *acc;
}


alu xio::assignmul()
{
    *acc = *(lhs->acc) *= *(rhs->acc);
    return *acc;
}


alu xio::assigndiv()
{
    *acc = *(lhs->acc) /= *(rhs->acc);
    return *acc;
}
alu xio::assignmod()
{
    *acc = *(lhs->acc) %= *(rhs->acc);
    return *acc;

}
alu xio::assignand()
{
    *acc = *(lhs->acc) &= *(rhs->acc);
    return *acc;

}
alu xio::assignor()
{
    *acc = *(lhs->acc) |= *(rhs->acc);
    return *acc;

}
alu xio::assignxor()
{
    *acc = *(lhs->acc) ^= *(rhs->acc);
    return *acc;

}
alu xio::assignx1()
{
    *acc = ~(*rhs->acc);
    return *acc;
}
alu xio::assignlshift()
{
    *acc = *lhs->acc <<= *rhs->acc;
    return *acc;
}


alu xio::assignrshift()
{
    *acc = *lhs->acc >>= *rhs->acc;
    return *acc;
}


alu xio::leq()
{
    *acc = *lhs->acc <= *rhs->acc;
    return *acc;
}


alu xio::geq()
{
    *acc = *lhs->acc >= *rhs->acc;
    return *acc;
}


alu xio::eq()
{
    *acc = *lhs->acc == *rhs->acc;
    return *acc;
}


alu xio::neq()
{
    *acc = *lhs->acc != *rhs->acc;
    return *acc;
}

alu xio::add()
{
    *acc = *lhs->acc + *rhs->acc;
    return *acc;
}
alu xio::sub()
{
    *acc = *lhs->acc - *rhs->acc;
    return *acc;
}
alu xio::mul()
{
    *acc = *lhs->acc * *rhs->acc;
    return *acc;
}
alu xio::modulo()
{
    *acc = *lhs->acc % *rhs->acc;
    return *acc;
}
alu xio::lt()
{
    *acc = *lhs->acc < *rhs->acc;
    return *acc;
}
alu xio::gt()
{
    *acc = *lhs->acc > *rhs->acc;
    return *acc;
}
alu xio::assign()
{
    *acc = *lhs->acc = *rhs->acc;
    return *acc;
}
alu xio::binand()
{
    *acc = *lhs->acc & *rhs->acc;
    return *acc;
}
alu xio::binor()
{
    *acc = *lhs->acc | *rhs->acc;
    return *acc;
}
alu xio::bitxor()
{
    *acc = *lhs->acc ^ *rhs->acc;
    return *acc;
}
alu xio::x1()
{
    *acc = ~(*lhs->acc);
    return *acc;
}
alu xio::x2()
{
    *acc = ~(*lhs->acc) + alu(1);
    return *acc;
}


alu xio::bitnot()
{
    *acc = !rhs->acc->number<double>();
    return *acc;
}


alu xio::booland()
{
    *acc = *lhs->acc && *rhs->acc;
    return *acc;
}
alu xio::boolor()
{
    *acc = *lhs->acc || *rhs->acc;
    return *acc;

}
alu xio::division()
{
    *acc = *lhs->acc  / *rhs->acc;
    return *acc;
}
alu xio::factorial()
{
    *acc = lhs->factorial();
    return *acc;
}
alu xio::positive()
{
    if(rhs->acc->number<double>() < 0)
        *rhs->acc *= -1;

    *acc = *rhs->acc;
    return *acc;
}


alu xio::negative()
{
    if(rhs->acc->number<double>() > 0) // ==> a = -1;  -a = ?
        *rhs->acc *= -1;

    *acc = *rhs->acc;
    return *acc;
}


alu xio::kpi()
{
    *acc = alu(_PI_);
    return *acc;
}


alu xio::kcos()
{
    *acc = std::cos(deg2rad(rhs));
    return *acc;
}


alu xio::kacos()
{
    *acc = std::acos(deg2rad(rhs));
    return *acc;
}
alu xio::ktan()
{
    *acc = std::tan(deg2rad(rhs));
    return *acc;
}


alu xio::katan()
{
    *acc = std::atan(deg2rad(rhs));

    return *acc;
}

alu xio::ksin()
{
    *acc = std::sin(deg2rad(rhs));
    return *acc;
}

alu xio::kasin()
{
    *acc = std::asin(deg2rad(rhs));
    return *acc;
}

alu xio::knumber()
{

    return *acc;
}

alu xio::ku8()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFF;
    return alu();
}

alu xio::ku16()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFFFF;
    return *acc;
}

alu xio::ku32()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFFFFFFFF;
    return *acc;
}

alu xio::ku64()
{
    *acc = (uint64_t)(rhs->acc->number<uint64_t>());// & 0xFFFFFFFFFFFFFFFF);
    return *acc;
}

alu xio::ki8()
{
    *acc = (char)(rhs->acc->number<int64_t>() & 0xFF);
    return *acc;
}

alu xio::ki16()
{
    logdebugpfn << rhs->t0->attribute() << Ends;
    *acc = (int16_t)(rhs->acc->number<uint64_t>() & 0xFFFF);
    return *acc;
}

alu xio::ki32()
{
    *acc = (int32_t)(rhs->acc->number<uint64_t>() & 0xFFFFFFFF);
    return alu();
}

alu xio::ki64()
{
    *acc = (int64_t)(rhs->acc->number<int64_t>() & 0xFFFFFFFFFFFFFFFF);
    return *acc;
}

alu xio::kreal()
{
    *acc = rhs->acc->number<double>();
    return alu();
}

alu xio::kstring()
{
    *acc = std::string(*rhs->acc);
    return *acc;
}


std::string xio::type_name()
{
    return lexer::type::to_s(t0->t);
}

xio::expect xio::tree_rpar_input_postfix(xio::shared) {
    return {};
}



void xio::trace_tree_start(utils::xstr &a_out, const utils::xstr &Title) 
{
    a_out << "digraph teacc_club_expr_tree {\n";
    a_out << "ratio=compress; ranksep=.55; size = \"6.5,6.5\";\n";
    a_out << "    node [fontname=\"Source Code Pro\", fontsize=12];\n";
    a_out << "    label=\"Expression CST:[" << Title << "]\"; fontsize = 10;\n";
}

void xio::trace_node(xio::shared A, utils::xstr &a_out) 
{
    //static int nullcount = 0;
    if(!A) return;
    if((!A->lhs) && (!A->rhs)) return;

    if (A->lhs){
        a_out <<  "    A" << A << " -> A" <<  A->lhs << ";\n";
        xio::trace_node(A->lhs,a_out);
    }
    //     else
    //         t_node::trace_null_node(A, nullcount++, a_stream);

    if (A->rhs){
        a_out <<  "    A" << A << " -> A" <<  A->rhs << ";\n";
        xio::trace_node(A->rhs,a_out);
    }
    //     else
    //         trace_null_node(A, nullcount++, a_stream);
}

void xio::trace_null_node(xio::shared, int, utils::xstr &) 
{

}

void xio::trace_tree(xio::shared a_root, utils::xstr &a_out) 
{
    a_root->tree_attr(a_out);
    xio::trace_node(a_root, a_out);
}

void xio::trace_tree_close(utils::xstr &a_out) 
{
    a_out << "}\n";
}

void xio::tree_attr(utils::xstr &a_out) 
{
    utils::xstr attr ;
    attr << t0->attribute();
    if(t0->t & lexer::type::assign)
        attr << ": [" << (*acc)() << "]";
    utils::xstr Shape;
    if(t0->t & lexer::type::text)
        Shape << "none";
    else
    if(t0->t & lexer::type::assign)
        Shape << "none";
    else
        Shape << "none";

    a_out <<  "    A" << this << " [shape=%s, label=\"%s\"]\n";
    a_out << Shape << attr;

    if(lhs)
        lhs->tree_attr(a_out);
    if(rhs)
        rhs->tree_attr(a_out);
}

std::string xio::attribute() {
    return t0? t0->attribute() : "[]";
}


xio::shared xio::query_child(xio* cThis)
{
    for(auto i : _children) if(cThis == i.get()) return i;
    return nullptr;
}


}