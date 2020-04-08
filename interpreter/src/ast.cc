#include <xio/interpreter/parsers/ast.h>
#include <xio/interpreter/parsers/grammar.h>
#include <xio/utils/journal.h>

using logger = teacc::utils::journal;


namespace teacc::ast
{


using utils::notification;


utils::result_code astbloc::build()
{
    utils::result_code r  = _rules.build();
    
    return
        {(
             notification::push(), __PRETTY_FUNCTION__ ,
             notification::code::implement
             ," wtf? I'm only testing!!"
         )};
    
}
astbloc::product astbloc::enter_rule(parsers::rule_t *rule)
{
    return
    {(
        notification::push(), __PRETTY_FUNCTION__, " Sorry, not implemented yet..."
    )};
}

ast_node* astbloc::up()
{
    notification::push(), __PRETTY_FUNCTION__, " Sorry, not planned yet...";
    return nullptr;
}

auto astbloc::begin()
{
    return _children.begin();
}

auto astbloc::end()
{
    return _children.end();
}

#pragma region TRIGRAPH

void ast_node::trace_tree_start(utils::xstr& a_out, const utils::xstr& Title)
{
    a_out << "AST Tree {\n";
    a_out << "ratio=compress; ranksep=.55; size = \"6.5,6.5\";\n";
    a_out << "    node [fontname=\"Source Code Pro\", fontsize=12];\n";
    a_out << "    label=\"AST:[" << Title << "]\"; fontsize = 10;\n";
}

void ast_node::trace_tree(ast_node* a_root, utils::xstr& a_out)
{
    a_root->tree_attr(a_out);
    ast_node::trace_node(a_root, a_out);
}


void ast_node::trace_node(ast_node* A, utils::xstr& a_out)
{
    //static int nullcount = 0;
    if(!A) return;
    if((!A->_lhs) && (!A->_rhs)) return;
    
    if (A->_lhs){
        a_out <<  "    A" << A << " -> A" <<  A->_lhs << ";\n";
        ast_node::trace_node(A->_lhs,a_out);
    }
    //     else
    //         t_node::trace_null_node(A, nullcount++, a_stream);
    
    if (A->_rhs){
        a_out <<  "    A" << A << " -> A" <<  A->_rhs << ";\n";
        ast_node::trace_node(A->_rhs,a_out);
    }
    //     else
    //         trace_null_node(A, nullcount++, a_stream);
}

void ast_node::trace_null_node(ast_node*, int, utils::xstr&)
{

}


void ast_node::trace_tree_close(utils::xstr& a_out)
{
}

void ast_node::tree_attr(utils::xstr& a_out)
{
    utils::xstr attr ;
    attr << _token->attribute();
    if(_token->t & lexer::type::assign)
        attr << ": [" << (_xio ? (*_xio->unit())() : "null") << "]";
    utils::xstr Shape;
    if((_token->t & lexer::type::text) || (_token->t & lexer::type::assign))
        Shape << "none";
    
    a_out <<  "    A" << this << " [shape=%s, label=\"%s\"]\n";
    a_out << Shape << attr;
    
    if(_lhs)
        _lhs->tree_attr(a_out);
    if(_rhs)
        _rhs->tree_attr(a_out);
}

std::string ast_node::attribute()
{
    return _token ? _token->attribute() : "[]";
}

#pragma endregion TRIGRAPH

ast_node::ast_node(ast_node* _parent_node, lexer::type::token_t *a_token)
:_parent(_parent_node), _token(a_token) {}


auto ast_node::begin()
{
    return _children.begin();
}

auto ast_node::end()
{
    return _children.end();
}

// -------- Arithmetic binary tree INPUT: -----------------------------------

// Arithmetic Binary Tree: associative building logic table:
ast_node::ar_input_collection  ast_node::_ar_input_assoc_collection = {
    {{lexer::type::binary,   lexer::type::leftpar},   &ast_node::ar_expr_set_right},
    {{lexer::type::leftpar,  lexer::type::leaf},      &ast_node::ar_expr_set_left},
    {{lexer::type::leftpar,  lexer::type::prefix},    &ast_node::ar_expr_set_left},
    {{lexer::type::leftpar,  lexer::type::binary},    &ast_node::ar_expr_set_left},
    {{lexer::type::prefix,   lexer::type::leftpar},   &ast_node::ar_expr_set_right},
    {{lexer::type::closepar,  lexer::type::leaf},     &ast_node::ar_expr_rpar_input_leaf},
    {{lexer::type::closepar,  lexer::type::binary},   &ast_node::ar_expr_close_par},
    {{lexer::type::closepar,  lexer::type::postfix},  &ast_node::ar_expr_close_par},
    {{lexer::type::closepar,  lexer::type::closepar}, &ast_node::ar_expr_rpar_rpar},
    {{lexer::type::prefix,   lexer::type::closepar},  &ast_node::ar_expr_input_rpar},
    {{lexer::type::leaf,     lexer::type::closepar},  &ast_node::ar_expr_input_rpar},
    {{lexer::type::leaf,     lexer::type::postfix},   &ast_node::ar_expr_set_right_to_op},
    {{lexer::type::leaf,     lexer::type::assign},    &ast_node::ar_expr_input_binary},
    {{lexer::type::postfix,  lexer::type::closepar},  &ast_node::ar_expr_input_rpar},
    {{lexer::type::leftpar,  lexer::type::binary},    &ast_node::ar_expr_set_left},
    {{lexer::type::leaf,     lexer::type::binary},    &ast_node::ar_expr_input_binary},
    {{lexer::type::binary,   lexer::type::binary},    &ast_node::ar_expr_input_binary},
    {{lexer::type::binary,   lexer::type::leaf},      &ast_node::ar_expr_set_right},
    {{lexer::type::prefix,   lexer::type::binary},    &ast_node::ar_expr_input_binary},
    {{lexer::type::binary,   lexer::type::prefix},    &ast_node::ar_expr_set_right},
    {{lexer::type::prefix,   lexer::type::leaf},      &ast_node::ar_expr_set_right},
    {{lexer::type::prefix,   lexer::type::number},    &ast_node::ar_expr_set_right},
    {{lexer::type::sign,     lexer::type::id},        &ast_node::ar_expr_set_right},
    {{lexer::type::sign,     lexer::type::number},    &ast_node::ar_expr_set_right},
    {{lexer::type::sign,     lexer::type::leaf},      &ast_node::ar_expr_set_right},
    {{lexer::type::postfix,  lexer::type::binary},    &ast_node::ar_expr_input_binary},
    {{lexer::type::assign,   lexer::type::binary},    &ast_node::ar_expr_set_right},
    {{lexer::type::assign,   lexer::type::leaf},      &ast_node::ar_expr_set_right},
    {{lexer::type::assign,   lexer::type::prefix},    &ast_node::ar_expr_set_right},
    {{lexer::type::assign,   lexer::type::postfix},   &ast_node::ar_expr_set_right}
    
};


ast_node::ar_input_method ast_node::associate(lexer::type::token_t *_input_location, lexer::type::token_t *_input_token)
{
    for(auto key : ast_node::_ar_input_assoc_collection)
    {
        if ((key.first.first & _input_location->s) && (_input_token->s & key.first.second))
            return key.second;
    }
    return nullptr;
}


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

ast_node::product ast_node::ar_expr_input(lexer::type::token_t *input_token)
{
    auto fn = ast_node::associate(_token, input_token);

/*
//    for(auto key : xio::tree_input_assoc_table)
//    {
//        if((key.first.first & _token->s) && (a_token->s & key.first.second))
//        {
//            logdebugfn << logger::White
//                       << "'" << logger::Yellow
//                       << _token->attribute()
//                       << logger::White  << "' - Key:"
//                       << logger::Yellow << lexer::type::to_s(key.first.first)
//                       << logger::White  << " <=> "
//                       << logger::Blue  << __FUNCTION__
//                       << logger::White  << ":'"
//                       << logger::Yellow << a_token->attribute()
//                       << logger::White  << "' - Key:"
//                       << logger::Yellow << lexer::type::to_s(key.first.second)
//                       << logger::White  << ":"
//                       << Ends;
//
//            pfn = key.second;
//            break;
//        }
//    }
*/
    if (fn)
        return (this->*fn)(new ast_node(_parent,input_token));
    
    return
        {(
             utils::notification::push(utils::notification::type::error),
                 ":(debug): syntax error, unexpected token:",
                 input_token->attribute(),
                 "\n",
                 input_token->mark()
         )};
}



ast_node::product ast_node::ar_expr_input_binary(ast_node* x)
{
    logdebugfn
        << logger::Yellow << _token->attribute()
        << logger::White << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << x->_token->attribute()
        << Ends;
    
    if (_token->is_binary())
    {
        if (!_rhs)
        {
            return { (
                         utils::notification::push(utils::notification::type::error),
                             " binary operator has no right hand side operand, then I cannot input this operator.",
                             _token->attribute(),_token->mark()
                     )};
        }
        if (_token->d >= x->_token->d)
        {
            return ar_expr_set_right(x);
        }
    }
    
    if (_op)
    {
        ar_input_method ptr = associate(_op->_token, x->_token);
        if (ptr)
            return (_op->*ptr)(x);
    }
    
    if(_parent)
        _parent->ar_expr_set_left(this);
    return x;
}


ast_node::product ast_node::ar_expr_input_leaf(ast_node *x)
{
    logdebugfn
        << logger::Yellow << _token->attribute()
        << logger::White  << ":"
        << logger::HBlue  << __FUNCTION__
        << logger::White  << ":"
        << logger::Yellow << x->_token->attribute()
        << Ends;
    
    if(!_token->is_operator())
        return {(
                    utils::notification::push(utils::notification::type::error),
                        "Expected an operator after a value operand.",
                        x->_token->mark()
                )};
    
    if(_token->is_binary()){
        if(!_lhs)
            return {(
                        utils::notification::push(utils::notification::type::error),
                            " binary operator has no left hand side operand.",
                            _token->attribute(),
                            _token->mark()
                    )};
    }
    
    ///@Todo Reach interpreter :: mark (token_t*, or xio::shared  );
    return ar_expr_set_right(x);
}



ast_node::product ast_node::ar_expr_set_left(ast_node *x)
{
    logdebugfn
        << logger::Yellow << _token->attribute()
        << logger::White  << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << x->_token->attribute()
        << Ends;
    
    /*
              (;[;{   // Appliqué sur aucun autre type de token car l'appel de tree_set_left ne se fait qu'� partir de tree_input qui r�soud l'associativit�.
               /
              x <- next_token
             /
           lhs
     */
    
    if (_lhs) {
        // here we are supposed to be the openning par/index/bracket. so the interior will become right hand side of the parent op of this.
        _lhs->_op = x;
        x->_lhs = _lhs;
    }
    if(_parent)
        x->_op = this;
    
    _lhs = x;
    return x;
}

static std::stack<ast_node*> pars;

static void push_lpar(ast_node* lpar)
{
    logdebugpfn << Ends;
    pars.push(lpar);
}

static ast_node* pop_lpar()
{
    if (pars.empty()) return nullptr;
    ast_node* x = pars.top();
    pars.pop();
    return x;
}


ast_node::product ast_node::ar_expr_set_right(ast_node *x)
{
    logdebugfn
        << logger::Yellow << _token->attribute()
        << logger::White << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << x->_token->attribute()
        << Ends;
    
    // Temporary hack....
    if (x->_token->c == lexer::lexem::mnemonic::openpar)
        push_lpar(x);
    
    if (_rhs) {
        /*
           this
              \
               x <- next_token
              /
            rhs
        */
        logdebugfn << _token->attribute() << " -> " << _rhs->_token->attribute()
                   << logger::Black << "tree_set_right "
                   << logger::White << "<- "
                   << logger::Yellow << x->_token->attribute()
                   <<   Ends;
        _rhs->_op = x;
        x->_lhs = _rhs;
    }
    _rhs = x;
    if(_parent)
        x->_op = this;
    return x;
}


ast_node::product ast_node::ar_expr_set_right_to_op(ast_node *x)
{
    logdebugfn << logger::Yellow << _token->attribute()
               << logger::White << ":"
               << logger::Black << __FUNCTION__
               << logger::White << ":"
               << logger::Yellow << x->_token->attribute()
               << Ends;
    
    if (!_op) {
        x->_lhs = this;
        _op = x;
        return x;
    }
    return _op->ar_expr_set_right(x);
}


ast_node::product ast_node::ar_expr_lpar_input_binary(ast_node *x)
{
    if(!_lhs) {
        return {(
                    utils::notification::push(utils::notification::type::error),
                        " syntax error : \n", _token->mark()
                )};
    }
    return ar_expr_set_left(this);
}


ast_node::product ast_node::ar_expr_input_rpar(ast_node *rpx)
{
    logdebugfn
        << logger::Yellow << _token->attribute()
        << logger::White << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << rpx->_token->attribute()
        << Ends;
    
    ast_node* x = pop_lpar();
    if (!x)
        return {(
                    utils::notification::push(utils::notification::type::error),
                        "Unmatched left paranthese.",
                        rpx->_token->mark()
                )};
    
    rpx->_op = x->_op;
    rpx->_lhs = x->_lhs;
    rpx->_lhs->_op = rpx;
    rpx->_op->_rhs = rpx; // oops!!
    logdebugfn << "new input vertex:[" << logger::Yellow <<  rpx->_token->attribute() << logger::Reset << "]" << Ends;
    
    return rpx;
}
ast_node::product ast_node::ar_expr_input_lpar(ast_node *x)
{
    logdebugfn
        << logger::Yellow << _token->attribute()
        << logger::White << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << x->_token->attribute()
        << Ends;
    
    // Binary input left par -> tree_set_right is called directly.
    return ar_expr_set_right(x);
}


ast_node::product ast_node::ar_expr_close_par(ast_node *x)
{
    logdebugfn
        << logger::Yellow << _token->attribute()
        << logger::White << ":"
        << logger::Black << __FUNCTION__
        << logger::White << ":"
        << logger::Yellow << x->_token->attribute()
        << Ends;    // Binary input left par -> tree_set_right is called directly.
    
    ast_node* v = _lhs;
    
    // Collapse lhs
    
    v->_op = _op;
    if (_op) _op->_rhs = v;
    
    // discard();
    
    if (v->_op) {
        auto p_fn = associate(v->_op->_token, x->_token);
        if (!p_fn) {
            utils::xstr str = _token->mark();
            return { (utils::notification::push(utils::notification::type::error), "syntax error on:", str()) };
        }
        return (v->_op->*p_fn)(x);
    }
    
    v->_op = x;
    x->_lhs = v;
    return x;
}


ast_node::product ast_node::ar_expr_rpar_input_postfix(ast_node *)
{
    return teacc::ast::ast_node::product();
}


ast_node::product ast_node::ar_expr_rpar_input_leaf(ast_node *x)
{
    logdebugfn << logger::Yellow << _token->attribute()
               << logger::White << ":"
               << logger::Black << __FUNCTION__
               << logger::White << ":"
               << logger::Yellow << x->_token->attribute()
               << Ends;    // Binary input left par -> tree_set_right is called directly.
    
    if (_lhs) {
        logdebugfn << "lhs:" << logger::Yellow << _lhs->_token->attribute()
                   << logger::White << Ends;
        
        if (_lhs->_token->is_prefix()) {
            if (_op) {
                logdebugfn << logger::Yellow  << _lhs->_token->attribute() << logger::White << "'s operator:`" << logger::Yellow << _lhs->_op->_token->attribute() << logger::Reset << "':" << Ends;
                //op->tree_set_right(lhs);
                _lhs->_op = _op;
                _op->_rhs = _lhs;
                
            }
            ast_node* xx = _lhs;
            // discard();
            
            return xx->ar_expr_set_right(x);
        }
    }
    return {(utils::notification::push(utils::notification::type::error), "illegal rvalue token :", x->_token->mark())};
}


ast_node::product ast_node::ar_expr_rpar_rpar(ast_node * x)
{
    logdebugfn << logger::Yellow << _token->attribute()
               << logger::White << ":"
               << logger::Black << __FUNCTION__
               << logger::White << ":"
               << logger::Yellow << x->_token->attribute()
               << Ends;    // Binary input left par -> tree_set_right is called directly.
    x->_op = _op;
    x->_lhs =_lhs;
    x->_lhs->_op = x;
    if (_op) _op->_rhs = x;
    return { x };
}


ast_node::product ast_node::ar_expr_close()
{
    logdebugfn << Ends;
    if (_token->c == lexer::lexem::mnemonic::openpar)
        return {(
                    utils::notification::push(utils::notification::type::error),
                        " unexpected end of file."
                )};
    if (!pars.empty()) {
        return { (
                     utils::notification::push(utils::notification::type::error),
                         " umatched closing parenthese."
                 ) };
    }
    
    if (_token->c == lexer::lexem::mnemonic::closepar) {
        logdebugfn << "Closing the tree on close parenthese:" << Ends;
        if (_lhs) {
            ast_node* x = _lhs;
            _lhs->_op = _op;
            
            if (_op) {
                _op->_rhs = _lhs;
            }
            // discard();
            return { x->expr_root() };
        }
    }
    return { expr_root() };
}


ast_node::product ast_node::expr_root()
{
    ast_node* x = this;
    ast_node* p = x;
    do{
        x = p;
        switch (x->_token->t) {
            case lexer::type::assign:
            case lexer::type::binary:
                if (!x->_lhs)
                    return {(
                                utils::notification::push(utils::notification::type::error), "Syntax error: binary operator is missing its left operand.\n", x->_token->mark()
                            )};
                if (!x->_rhs)
                    return {(
                                utils::notification::push(utils::notification::type::error), "Syntax error: binary operator is missing its right operand.\n", x->_token->mark()
                            )};
                break;
            case lexer::type::prefix:
                if (!x->_rhs)
                    return { (
                                 utils::notification::push(utils::notification::type::error), "Syntax error: prefix unary operator is missing its (right) operand.\n", x->_token->mark()
                             )};
                break;
            case lexer::type::postfix:
                if (!x->_lhs)
                    return { (
                                 utils::notification::push(utils::notification::type::error), "Syntax error: postfix unary operator is missing its (left) operand.\n", x->_token->mark()
                             )};
                break;
        }
        
        p = p->_op;
    } while (p);
    return  x ;
}

}
