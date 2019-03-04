
#include <xio++/xio++.hpp>
#include <cmath>
#include <xio++/journal/logger.hpp>
#include "xio.hpp"

namespace xio {


#define deg2rad(_xio) ((_xio)->acc->number<double>()*3.1415) / 180
constexpr auto _PI_ = 3.14159265358979323846;
// M_PI ?
std::stack<xio_t*> xio_t::pars;
std::stack<xio_t*> xio_t::indexes;
std::stack<xio_t*> xio_t::curls;
xio_t::xio_t::list_t xio_t::xio_gc;

void xio_t::discard()
{
    for (auto x : xio_t::xio_gc)
        if (this == x) return;

    xio_t::xio_gc.push_back(this);
    lhs = rhs = op = nullptr; // Make sure we do not accidentally reuse this;
}


// Arithmetic Binary Tree: associative building logic table:
xio_t::input_table_t      xio_t::tree_input_assoc_table = {

    {{type_t::binary,   type_t::leftpar},   &xio_t::tree_set_right},
    {{type_t::leftpar,  type_t::leaf},      &xio_t::tree_set_left},
    {{type_t::leftpar,  type_t::prefix},    &xio_t::tree_set_left},
    {{type_t::leftpar,  type_t::binary},    &xio_t::tree_set_left},
    {{type_t::prefix,   type_t::leftpar},   &xio_t::tree_set_right},
    {{type_t::closepar,  type_t::leaf},     &xio_t::tree_rpar_input_leaf},
    {{type_t::closepar,  type_t::binary},   &xio_t::tree_close_par},
    {{type_t::closepar,  type_t::postfix},  &xio_t::tree_close_par},
    {{type_t::closepar,  type_t::closepar}, &xio_t::tree_rpar_rpar},
   
    {{type_t::prefix,   type_t::closepar},  &xio_t::tree_input_rpar},
    {{type_t::leaf,     type_t::closepar},  &xio_t::tree_input_rpar},
    {{type_t::leaf,     type_t::postfix},   &xio_t::tree_set_right_to_op},
    {{type_t::leaf,     type_t::assign},    &xio_t::tree_input_binary},

    
    {{type_t::postfix,  type_t::closepar},  &xio_t::tree_input_rpar},
    {{type_t::leftpar,  type_t::binary},    &xio_t::tree_set_left},
    {{type_t::leaf,     type_t::binary},    &xio_t::tree_input_binary},
    {{type_t::binary,   type_t::binary},    &xio_t::tree_input_binary},
    {{type_t::binary,   type_t::leaf},      &xio_t::tree_set_right},
    {{type_t::prefix,   type_t::binary},    &xio_t::tree_input_binary},
    {{type_t::binary,   type_t::prefix},    &xio_t::tree_set_right},
    {{type_t::prefix,   type_t::leaf},      &xio_t::tree_set_right},
    {{type_t::prefix,   type_t::number},    &xio_t::tree_set_right},
    {{type_t::sign,     type_t::id},        &xio_t::tree_set_right},
    {{type_t::sign,     type_t::number},    &xio_t::tree_set_right},
    {{type_t::sign,     type_t::leaf},      &xio_t::tree_set_right},
    {{type_t::postfix,  type_t::binary},    &xio_t::tree_input_binary},

    {{type_t::assign,   type_t::binary},    &xio_t::tree_set_right},
    {{type_t::assign,   type_t::leaf},      &xio_t::tree_set_right},
    {{type_t::assign,   type_t::prefix},    &xio_t::tree_set_right},
    {{type_t::assign,   type_t::postfix},   &xio_t::tree_set_right}

};


// Primary arithmetic supported operators table:
xio_t::xio_opfn_table_t   xio_t::xio_operators_table{
    {e_code::lshift,          &xio_t::lshift},
    {e_code::radical,         &xio_t::radical},
    {e_code::exponent,        &xio_t::exponent},
    {e_code::rshift,          &xio_t::rshift},
    {e_code::decr,            &xio_t::decr},
    {e_code::incr,            &xio_t::incr},
    {e_code::assignadd,       &xio_t::assignadd},
    {e_code::assignsub,       &xio_t::assignsub},
    {e_code::assignmul,       &xio_t::assignmul},
    {e_code::assigndiv,       &xio_t::assigndiv},
    {e_code::assignmod,       &xio_t::assignmod},
    {e_code::assignand,       &xio_t::assignand},
    {e_code::assignor,        &xio_t::assignor},
    {e_code::assignxor,       &xio_t::assignxor},
    {e_code::assignx1,        &xio_t::assignx1},
    {e_code::assignlshift,    &xio_t::assignlshift},
    {e_code::assignrshift,    &xio_t::assignrshift},
    {e_code::leq,             &xio_t::leq},
    {e_code::geq,             &xio_t::geq},
    {e_code::eq,              &xio_t::eq},
    {e_code::neq,             &xio_t::neq},
    {e_code::add,             &xio_t::add},
    {e_code::sub,             &xio_t::sub},
    {e_code::mul,             &xio_t::mul},
    {e_code::modulo,          &xio_t::modulo},
    {e_code::lt,              &xio_t::lt},
    {e_code::gt,              &xio_t::gt},
    {e_code::assign,          &xio_t::assign},
    {e_code::binand,          &xio_t::binand},
    {e_code::binor,           &xio_t::binor},
    {e_code::bitxor,          &xio_t::bitxor},
    {e_code::x1,              &xio_t::x1},
    {e_code::x2,              &xio_t::x2},
    {e_code::bitnot,          &xio_t::bitnot},
    {e_code::booland,         &xio_t::booland},
    {e_code::boolor,          &xio_t::boolor},
    {e_code::division,        &xio_t::division},
    {e_code::factorial,       &xio_t::factorial},
    {e_code::positive,        &xio_t::positive},
    {e_code::negative,        &xio_t::negative},
    {e_code::kpi,             &xio_t::kpi},
    {e_code::kcos,            &xio_t::kcos},
    {e_code::kacos,           &xio_t::kacos},
    {e_code::ktan,            &xio_t::ktan},
    {e_code::katan,           &xio_t::katan},
    {e_code::ksin,            &xio_t::ksin},
    {e_code::kasin,           &xio_t::kasin},
    {e_code::knumber,         &xio_t::knumber},
    {e_code::ku8,             &xio_t::ku8   },
    {e_code::ku16,            &xio_t::ku16  },
    {e_code::ku32,            &xio_t::ku32  },
    {e_code::ku64,            &xio_t::ku64  },
    {e_code::ki8,             &xio_t::ki8   },
    {e_code::ki16,            &xio_t::ki16  },
    {e_code::ki32,            &xio_t::ki32  },
    {e_code::ki64,            &xio_t::ki64  },
    {e_code::kreal,           &xio_t::kreal },
    {e_code::kstring,         &xio_t::kstring}
};


xio_t::xio_t()
{
    //t0 = new token_t;
    acc = new alu(0.0);
}


xio_t::xio_t(object * a_parent):object(a_parent)
{
    //t0 = new token_t;
    acc = new alu(0.0);
}

xio_t::xio_t(object * a_parent, token_t * a_token, alu * a_alu):object(a_parent)
{
    t0 = a_token;
    logdebugpfn << "xio_t::xio_t this->type:[" << type_t::name(t0?t0->sem:type_t::null) << "'" << (t0?t0->attribute():"no token") <<  "'] \n";

    acc = a_alu;
    if (acc) return;
    mem.own = 1;
    
    if (!t0){
        acc = new alu(0.0);
        return;
    }

    switch(t0->type){
        case type_t::text:
            acc = new alu(a_token->attribute());
            return;
        case type_t::number:
        {
            double d;
            (string_t(a_token->attribute())) >> d;
            acc = new alu(d);
        }
            return;
        case type_t::hex:
        {
            uint64_t d;
            string_t(a_token->attribute()).hex(d);
            acc = new alu(d);
            return;
        }   
        return;
        case type_t::any:
        case type_t::voidptr:
            acc = new alu((void*)nullptr);
            return;
        default:
            acc = new alu(0.0f);
            break;
    }

    auto i = xio_t::xio_operators_table.find(a_token->code);
    xio_fn = i != xio_t::xio_operators_table.end() ? i->second : nullptr;

}


xio_t::~xio_t()
{
    //First thing first: delete acc if it is owned by this xio_t:
    if (mem.own) delete acc;
    
    // that's it for now, folks.
}



// -------------- ARITHMETIC BINARY TREE INPUT LOGIC IMPLEMENTATION ------------------------------------

xio_t::result xio_t::tree_input(token_t * a_token, xio_t::allocator_t a_allocator)
{
    inptr_fn_t pfn = nullptr;

    //std::cerr << "[tree_input] : [" << type_t::name(t0->type) << "'" << t0->loc.b <<  "'] <- [" << type_t::name(a_token->type) << "'" << a_token->loc.b << "']:\n";

    for(auto key : xio_t::tree_input_assoc_table){
        

        if((key.first.first & t0->sem) && (a_token->sem & key.first.second)){
            
            logdebug << logger::White 
                << "'" << logger::Yellow 
                << t0->attribute() 
                << logger::White  << "' - Key:" 
                << logger::Yellow << type_t::name(key.first.first) 
                << logger::White  << " <=> "
                << logger::Blue  << __FUNCTION__ 
                << logger::White  << ":'" 
                << logger::Yellow << a_token->attribute() 
                << logger::White  << "' - Key:" 
                << logger::Yellow << type_t::name(key.first.second) 
                << logger::White  << ":" 
                << Ends;
                
            pfn = key.second;
            break;
        }
    }
    if (pfn) {
        xio_t * x = a_allocator(a_token);
        return (this->*pfn)(x);
    }
    return {(
        message::push(message::xclass::error),
        ":(debug): syntax error, unexpected token:",
        a_token->informations(),
        "\n",
        a_token->mark()
    )};
}



void xio_t::push_lpar(xio_t * lpar)
{
    logdebugpfn << Ends;
    xio_t::pars.push(lpar);
}

xio_t * xio_t::pop_lpar()
{
    if (xio_t::pars.empty()) return nullptr;
    xio_t* x = xio_t::pars.top();
    xio_t::pars.pop();
    return x;
}


xio_t::result xio_t::tree_input_binary(xio_t* x) {

    logdebug 
        << logger::Yellow << t0->attribute() 
        << logger::White << ":" 
        << logger::Black << __FUNCTION__ 
        << logger::White << ":" 
        << logger::Yellow << x->t0->attribute() 
        << Ends;

    if (t0->is_binary()) {
        if (!rhs) {
            return { (
                message::push(message::xclass::error),
                " binary operator has no right hand side operand, then I cannot input this operator.",
                t0->informations(),t0->mark()
            )};
        }
        if (t0->delta >= x->t0->delta) {
            return tree_set_right(x);
        }
    }

    if (op) {
        inptr_fn_t ptr = associate(op, x); // "has_assoc_pair(op,x); ??
        if (ptr)
            return (op->*ptr)(x);
        
    }

    x->tree_set_left(this);
    return { x };
}


xio_t::result xio_t::tree_input_leaf(xio_t* x) {

    logdebug 
        << logger::Yellow << t0->attribute() 
        << logger::White  << ":" 
        << logger::HBlue  << __FUNCTION__ 
        << logger::White  << ":" 
        << logger::Yellow << x->t0->attribute() 
        << Ends;

    if(!t0->is_operator())
        return {(
            message::push(message::xclass::error), 
            "Expected an operator after a value operand.",
            x->t0->mark()
        )};

    if(t0->is_binary()){
        if(!lhs)
            return {(
                message::push(message::xclass::error), 
                " binary operator has no left hand side operand.", 
                t0->informations(),
                t0->mark()
            )};
    }

    ///@Todo Reach interpreter :: mark (token_t*, or xio_t*  );
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
xio_t::result xio_t::tree_set_left(xio_t* x)
{
    logdebug 
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
    x->op = this;
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
xio_t::result xio_t::tree_set_right(xio_t* x) {
    logdebug 
        << logger::Yellow << t0->attribute() 
        << logger::White << ":" 
        << logger::Black << __FUNCTION__ 
        << logger::White << ":" 
        << logger::Yellow << x->t0->attribute() 
        << Ends;

    // Temporary hack....
    if (x->t0->code == e_code::openpar)
        push_lpar(x);

    if (rhs) {
        /*
           this
              \
               x <- next_token
              /
            rhs
        */
        logdebug << t0->attribute() << " -> " << rhs->t0->attribute() 
            << logger::Black << "tree_set_right " 
            << logger::White << "<- " 
            << logger::Yellow << x->t0->attribute() 
            <<   Ends;
        rhs->op = x;
        x->lhs = rhs;
    }
    rhs = x;
    x->op = this;
    return {x};
}


xio_t::result xio_t::tree_set_right_to_op(xio_t * x)
{
    logdebug << logger::Yellow << t0->attribute() 
        << logger::White << ":" 
        << logger::Black << __FUNCTION__ 
        << logger::White << ":" 
        << logger::Yellow << x->t0->attribute() 
        << Ends;

    if (!op) {
        x->lhs = this;
        op = x;
        return { x };
    }
    return op->tree_set_right(x);
}


xio_t::inptr_fn_t xio_t::associate(xio_t * a_lhs, xio_t * a_rhs)
{
    for(auto key : xio_t::tree_input_assoc_table){
        //if ((key.first.first == a_lhs->t0->type) && (a_rhs->t0->type & key.first.second)) {
        if ((key.first.first & a_lhs->t0->sem) && (a_rhs->t0->sem & key.first.second)) {
            return key.second;
        }
    }
    return nullptr;
}


xio_t::result xio_t::tree_close()
{
    if (t0->code == e_code::openpar)
        return {(
            message::push(message::xclass::error),
            " unexpected end of file."
        )};
    if (!xio_t::pars.empty()) {
        
        return { (
            message::push(message::xclass::error),
            " umatched closing parenthese."
        ) };
    }
    
    if (t0->code == e_code::closepar) {
        if (lhs) {
            xio_t* x = lhs;
            lhs->op = op;

            if (op) {
                op->rhs = lhs;
            }
            discard();
            return { x->tree_root() };
        }
    }
    return { tree_root() };
}


xio_t::result xio_t::tree_root()
{
    xio_t* x = this;
    xio_t* p = x;
    do{
        x = p;
        switch (x->t0->type) {
        case type_t::assign:
        case type_t::binary:
            if (!x->lhs)
                return {(
                    message::push(message::xclass::error), "Syntax error: binary operator is missing its left operand.\n", x->t0->mark()
                )};
            if (!x->rhs)
                return {(
                    message::push(message::xclass::error), "Syntax error: binary operator is missing its right operand.\n", x->t0->mark()
                )};
            break;
        case type_t::prefix:
            if (!x->rhs)
                return { (
                    message::push(message::xclass::error), "Syntax error: prefix unary operator is missing its (right) operand.\n", x->t0->mark()
                )};
            break;
        case type_t::postfix:
            if (!x->lhs)
                return { (
                    message::push(message::xclass::error), "Syntax error: postfix unary operator is missing its (left) operand.\n", x->t0->mark()
                )};
            break;
        }
        
        p = p->op;
    } while (p);
    return  x ;
}

xio_t::result xio_t::tree_lpar_input_binary(xio_t * x)
{
    if (!lhs) {
        return {(message::push(message::xclass::error))};
    }
    return xio_t::result();
}

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
xio_t::result xio_t::tree_input_rpar(xio_t * rpxio)
{
    logdebug 
        << logger::Yellow << t0->attribute() 
        << logger::White << ":" 
        << logger::Black << __FUNCTION__ 
        << logger::White << ":" 
        << logger::Yellow << rpxio->t0->attribute() 
        << Ends;

    // Leaf; postfix input right par.: 
    // Okay - On es rendu là...   À PLUS ! Merci! hehehehehehehehhehhehhehehehhhehheheh!
    xio_t* x = xio_t::pop_lpar();
    if (!x)
        return {(
            message::push(message::xclass::error), 
            "Unmatched left paranthese.",
            rpxio->t0->mark()
        )};

    rpxio->op = x->op;
    rpxio->lhs = x->lhs;
    rpxio->lhs->op = rpxio;
    rpxio->op->rhs = rpxio; // oops!!

    x->discard();
    return { rpxio };
}


xio_t::result xio_t::tree_input_lpar(xio_t * x)
{

    logdebug 
        << logger::Yellow << t0->attribute() 
        << logger::White << ":" 
        << logger::Black << __FUNCTION__ 
        << logger::White << ":" 
        << logger::Yellow << x->t0->attribute() 
        << Ends;

    // Binary input left par -> tree_set_right is called directly.
    return tree_set_right(x);
}

xio_t::result xio_t::tree_close_par(xio_t * x)
{
    logdebug 
        << logger::Yellow << t0->attribute() 
        << logger::White << ":" 
        << logger::Black << __FUNCTION__ 
        << logger::White << ":" 
        << logger::Yellow << x->t0->attribute() 
        << Ends;    // Binary input left par -> tree_set_right is called directly.

    xio_t* v = lhs;

    // Collapse lhs

    v->op = op;
    if (op) op->rhs = v;
    
    discard();
    
    if (v->op) {
        auto p_fn = associate(v->op, x);
        if (!p_fn) {
            string_t str = t0->mark();
            return { (message::push(message::xclass::error), "syntax error on:", str()) };
        }
        return (v->op->*p_fn)(x);
    }
    
    v->op = x;
    x->lhs = v;
    return {x};
}


xio_t::result xio_t::tree_rpar_input_leaf(xio_t *x)
{
    logdebug << logger::Yellow << t0->attribute()
        << logger::White << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << x->t0->attribute()
        << Ends;    // Binary input left par -> tree_set_right is called directly.

    if (lhs) {
        logdebug << "lhs:" << logger::Yellow << lhs->t0->attribute()
            << logger::White << Ends;
            
        if (lhs->t0->is_prefix()) {
            if (op) {
                logdebug << logger::Yellow  << lhs->t0->attribute() << logger::White << "'s operator:`" << logger::Yellow << lhs->op->t0->attribute() << logger::Reset << "':" << Ends;
                //op->tree_set_right(lhs);
                lhs->op = op;
                op->rhs = lhs;

            }
            xio_t* xx = lhs;
            discard();

            return xx->tree_set_right(x);
        }
    }
    return {(message::push(message::xclass::error), "illegal rvalue token :", x->t0->mark())};
}


xio_t::result xio_t::tree_rpar_rpar(xio_t * r)
{
    logdebug << logger::Yellow << t0->attribute() 
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

    discard();

    return { r }; 
}


// ------------------ ARITHMETIC OPERATORS IMPLEMENTATION ---------------------------------------------

alu xio_t::jsr()
{
    //...
    
    if (lhs) *acc = lhs->jsr(); // Always catch the lhs value so we return that one if there is no rhs operand.
    if (rhs) *acc = rhs->jsr(); // Always catch the rhs value because it is the value to be returned after being applied to the lhs (if applicable).
    
    if (xio_fn)
        return (this->*xio_fn)(); // All operators assign acc.
    else {
        if (t0->is_operator()) {
            logwarningfn << "xio_t [" << logger::Yellow << t0->informations() << logger::Reset << "] has no function (yet?).:\n" << t0->mark() << Ends;
        }
    }
    return *acc;
}


alu xio_t::lshift()
{
    *acc = lhs->acc->number<uint64_t>() << rhs->acc->number<uint64_t>();
    return *acc;
}

alu xio_t::radical()
{
    *acc = std::pow(lhs->acc->number<double>(), 1/rhs->acc->number<double>());
    return *acc;
}

alu xio_t::exponent()
{
    *acc = std::pow(lhs->acc->number<double>(), rhs->acc->number<double>());
    return *acc;
}

alu xio_t::rshift()
{
    *acc = lhs->acc->number<uint64_t>() >> rhs->acc->number<uint64_t>();
    return *acc;
}


alu xio_t::decr()
{
    *acc = t0->is_prefix() ? --(*rhs->acc) : *(lhs->acc)--;
    return *acc;
}


alu xio_t::incr()
{
    *acc = t0->is_prefix() ? ++(*rhs->acc) : *(lhs->acc)++;
    return *acc;
}


alu xio_t::assignadd()
{
    *acc = *(lhs->acc) += *(rhs->acc);
    return  *acc;
}


alu xio_t::assignsub()
{
    *acc = *(lhs->acc) -= *(rhs->acc);
    return  *acc;
}


alu xio_t::assignmul()
{
    *acc = *(lhs->acc) *= *(rhs->acc);
    return *acc;
}


alu xio_t::assigndiv()
{
    *acc = *(lhs->acc) /= *(rhs->acc);
    return *acc;
}
alu xio_t::assignmod()
{
    *acc = *(lhs->acc) %= *(rhs->acc);
    return *acc;

}
alu xio_t::assignand()
{
    *acc = *(lhs->acc) &= *(rhs->acc);
    return *acc;

}
alu xio_t::assignor()
{
    *acc = *(lhs->acc) |= *(rhs->acc);
    return *acc;

}
alu xio_t::assignxor()
{
    *acc = *(lhs->acc) ^= *(rhs->acc);
    return *acc;

}
alu xio_t::assignx1()
{
    *acc = ~(*rhs->acc);
    return *acc;
}
alu xio_t::assignlshift()
{
    *acc = *lhs->acc <<= *rhs->acc;
    return *acc;
}


alu xio_t::assignrshift()
{
    *acc = *lhs->acc >>= *rhs->acc;
    return *acc;
}


alu xio_t::leq()
{
    *acc = *lhs->acc <= *rhs->acc;
    return *acc;
}


alu xio_t::geq()
{
    *acc = *lhs->acc >= *rhs->acc;
    return *acc;
}


alu xio_t::eq()
{
    *acc = *lhs->acc == *rhs->acc;
    return *acc;
}


alu xio_t::neq()
{
    *acc = *lhs->acc != *rhs->acc;
    return *acc;
}

alu xio_t::add()
{
    *acc = *lhs->acc + *rhs->acc;
    return *acc;
}
alu xio_t::sub()
{
    *acc = *lhs->acc - *rhs->acc;
    return *acc;
}
alu xio_t::mul()
{
    *acc = *lhs->acc * *rhs->acc;
    return *acc;
}
alu xio_t::modulo()
{
    *acc = *lhs->acc % *rhs->acc;
    return *acc;
}
alu xio_t::lt()
{
    *acc = *lhs->acc < *rhs->acc;
    return *acc;
}
alu xio_t::gt()
{
    *acc = *lhs->acc > *rhs->acc;
    return *acc;
}
alu xio_t::assign()
{
    *acc = *lhs->acc = *rhs->acc;
    return *acc;
}
alu xio_t::binand()
{
    *acc = *lhs->acc & *rhs->acc;
    return *acc;
}
alu xio_t::binor()
{
    *acc = *lhs->acc | *rhs->acc;
    return *acc;
}
alu xio_t::bitxor()
{
    *acc = *lhs->acc ^ *rhs->acc;
    return *acc;
}
alu xio_t::x1()
{
    *acc = ~(*lhs->acc);
    return *acc;
}
alu xio_t::x2()
{
    *acc = ~(*lhs->acc) + alu(1);
    return *acc;
}


alu xio_t::bitnot()
{
    *acc = !rhs->acc->number<double>();
    return *acc;
}


alu xio_t::booland()
{
    *acc = *lhs->acc && *rhs->acc;
    return *acc;
}
alu xio_t::boolor()
{
    *acc = *lhs->acc || *rhs->acc;
    return *acc;

}
alu xio_t::division()
{
    *acc = *lhs->acc  / *rhs->acc;
    return *acc;
}
alu xio_t::factorial()
{
    *acc = lhs->factorial();
    return *acc;
}
alu xio_t::positive()
{
    if(rhs->acc->number<double>() < 0)
        *rhs->acc *= -1;

    *acc = *rhs->acc;
    return *acc;
}


alu xio_t::negative()
{
    if(rhs->acc->number<double>() > 0) // ==> a = -1;  -a = ?
        *rhs->acc *= -1;

    *acc = *rhs->acc;
    return *acc;
}


alu xio_t::kpi()
{
    *acc = alu(_PI_);
    return *acc;
}


alu xio_t::kcos()
{
    *acc = std::cos(deg2rad(rhs));
    return *acc;
}


alu xio_t::kacos()
{
    *acc = std::acos(deg2rad(rhs));
    return *acc;
}
alu xio_t::ktan()
{
    *acc = std::tan(deg2rad(rhs));
    return *acc;
}


alu xio_t::katan()
{
    *acc = std::atan(deg2rad(rhs));
    
    return *acc;
}

alu xio_t::ksin()
{
    *acc = std::sin(deg2rad(rhs));
    return *acc;
}

alu xio_t::kasin()
{
    *acc = std::asin(deg2rad(rhs));
    return *acc;
}

alu xio_t::knumber()
{
    
    return *acc;
}

alu xio_t::ku8()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFF;
    return alu();
}

alu xio_t::ku16()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFFFF;
    return *acc;
}

alu xio_t::ku32()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFFFFFFFF;
    return *acc;
}

alu xio_t::ku64()
{
    *acc = (uint64_t)(rhs->acc->number<uint64_t>());// & 0xFFFFFFFFFFFFFFFF);
    return *acc;
}

alu xio_t::ki8()
{
    *acc = (char)(rhs->acc->number<int64_t>() & 0xFF);
    return *acc;
}

alu xio_t::ki16()
{
    logdebugpfn << rhs->t0->attribute() << Ends;
    *acc = (int16_t)(rhs->acc->number<uint64_t>() & 0xFFFF);
    return *acc;
}

alu xio_t::ki32()
{
    *acc = (int32_t)(rhs->acc->number<uint64_t>() & 0xFFFFFFFF);
    return alu();
}

alu xio_t::ki64()
{
    *acc = (int64_t)(rhs->acc->number<int64_t>() & 0xFFFFFFFFFFFFFFFF);
    return *acc;
}

alu xio_t::kreal()
{
    *acc = rhs->acc->number<double>();
    return alu();
}

alu xio_t::kstring()
{
    *acc = std::string(*rhs->acc);
    return *acc;
}


std::string xio_t::type_name()
{
    return type_t::name(t0->type);
}


}



