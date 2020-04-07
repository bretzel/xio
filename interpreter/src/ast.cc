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
//        if((key.first.first & t0->s) && (a_token->s & key.first.second))
//        {
//            logdebugfn << logger::White
//                       << "'" << logger::Yellow
//                       << t0->attribute()
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


ast_node::product ast_node::ar_expr_set_right_to_op(ast_node *)
{
    return teacc::ast::ast_node::product();
}
ast_node::product ast_node::ar_expr_lpar_input_binary(ast_node *)
{
    return teacc::ast::ast_node::product();
}
ast_node::product ast_node::ar_expr_input_rpar(ast_node *)
{
    return teacc::ast::ast_node::product();
}
ast_node::product ast_node::ar_expr_input_lpar(ast_node *)
{
    return teacc::ast::ast_node::product();
}
ast_node::product ast_node::ar_expr_close_par(ast_node *)
{
    return teacc::ast::ast_node::product();
}
ast_node::product ast_node::ar_expr_rpar_input_postfix(ast_node *)
{
    return teacc::ast::ast_node::product();
}
ast_node::product ast_node::ar_expr_rpar_input_leaf(ast_node *)
{
    return teacc::ast::ast_node::product();
}
ast_node::product ast_node::ar_expr_rpar_rpar(ast_node *)
{
    return teacc::ast::ast_node::product();
}

//-------------------------------------------------------------------------------------------------------




}
