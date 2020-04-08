/*
    Copyright 2019 Serge Lussier (lussier.serge@gmail.com)
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files 
    (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
    merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
    FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#pragma once

//#include <xio/utils/expect>
#include <xio/interpreter/parsers/grammar.h>
#include <xio/interpreter/bloc.h>
#include <memory>
#include <vector>



namespace teacc::ast
{
/*!
    @brief ...

    @note Pour l'instant, les ast sont des structures passives. 
*/
struct INTERPRETERAPI ast_node
{
    
    using collection    = std::vector<ast_node*>;
    using product       = utils::expect<ast_node*>;
    
    ast_node*     _parent = nullptr;   ///< Parent node  (Noeud parent).
    
    // Arithmetic Expression (Sub)Tree
    ast_node* _lhs= nullptr; ///< Left-hand side operand node;
    ast_node* _rhs= nullptr; ///< Right-hand side operand node;
    ast_node* _op = nullptr;  ///< Parent Operator node.
    //--------------------------------------------------------
    
    ast_node::collection _children; ///< Recursive descendant ast nodes. Récursion descendante de l'arbre abstrait.
    parsers::term_t  _term; ///< Element de la règle : { sous-règle en récursion; terminale; directive; }
                            ///< Element's rule : { récursive (sub-)rule; terminal; directive; }
    
    xio*      _xio = nullptr;  ///< Élément concret produit.
                            ///< Produced concrete element
    ast_node()  = default;
    ast_node(ast_node* _parent_node, lexer::type::token_t* a_token);
    
    ~ast_node() = default;
    
    lexer::type::token_t* _token = nullptr;
    
    auto begin();
    auto end();
    
    // -------- Arithmetic binary tree INPUT: -----------------------------------
    #pragma region INPUT
    
    using ar_associative_token_type = std::pair<lexer::type::T, lexer::type::T>;
    using ar_input_method           = ast_node::product(ast_node::*)(ast_node*);  ///< callable arithmetic tree input function ptr
    using associated_input_method   = std::pair < ast_node::ar_associative_token_type, ast_node::ar_input_method>;
    using ar_input_collection       = std::vector<ast_node::associated_input_method>;
    
    static ast_node::ar_input_collection _ar_input_assoc_collection;
    static ast_node::ar_input_method associate(lexer::type::token_t* _input_location, lexer::type::token_t* _input_token);
    
    ast_node::product ar_expr_input(lexer::type::token_t* a_token);
    
    ast_node::product ar_expr_input_binary      (ast_node*);
    ast_node::product ar_expr_input_leaf        (ast_node*);
    ast_node::product ar_expr_set_left          (ast_node*);
    ast_node::product ar_expr_set_right         (ast_node*);
    ast_node::product ar_expr_set_right_to_op   (ast_node*);
    ast_node::product ar_expr_lpar_input_binary (ast_node*);
    ast_node::product ar_expr_input_rpar        (ast_node*);
    ast_node::product ar_expr_input_lpar        (ast_node*);
    ast_node::product ar_expr_close_par         (ast_node*);
    ast_node::product ar_expr_rpar_input_postfix(ast_node*);
    ast_node::product ar_expr_rpar_input_leaf   (ast_node*);
    ast_node::product ar_expr_rpar_rpar         (ast_node*);
    
    #pragma endregion INPUT
    // -------- Arithmetic binary tree: -----------------------------------
    product expr_root();
    product ar_expr_close();
};

class INTERPRETERAPI astbloc
{
    parsers::teacc_grammar  _rules;
    ast_node* _root = nullptr;
    ast_node* _node = nullptr;
    
    using product = utils::expect<ast_node*>;
    
public:
    astbloc()   = default;
    ~astbloc()  = default;

    utils::result_code build();
    
private:
    // building ast:
    astbloc::product enter_rule(parsers::rule_t* rule);
    
    
    //-------------------------------------------------
    
    // Visiation/Navigation :
    ast_node* up();
    ast_node::collection _children;
    auto begin();
    auto end();
    
#pragma region TRIGRAPH
public:
    static void trace_tree_start(utils::xstr& a_out, const utils::xstr& Title);
    static void trace_node(ast_node* A, utils::xstr& a_out);
    static void trace_null_node(ast_node*, int, utils::xstr&);
    static void trace_tree(ast_node* a_root, utils::xstr& a_out);
    static void trace_tree_close(utils::xstr& a_out);
    void tree_attr(utils::xstr& a_out);
    std::string attribute();
#pragma endregion TRIGRAPH

    // -----------------------------------------
    //ast_node::collection::iterator
    //ast_node::shared lr_next(ast_node::shared to);
};

}

