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
    using shared        = std::shared_ptr<ast_node>;
    using collection    = std::shared_ptr<std::vector<ast_node::shared>>;
    
    ast_node::shared     _parent;   ///< Parent node  (Noeud parent).
    ast_node::collection _children; ///< Recursive descendant ast nodes. Récursion descendante de l'arbre abstrait.
    parsers::term_t  _term; ///< Element de la règle : { sous-règle en récursion; terminale; directive; }
                            ///< Element's rule : { récursive (sub-)rule; terminal; directive; }
    
    xio::shared      _xio;  ///< Élément concret produit.
                            ///< Produced concrete element.
                            
    ast_node()  = default;
    ~ast_node() = default;
};

class INTERPRETERAPI astbloc
{
    parsers::teacc_grammar  _rules;
    ast_node::shared        _root;
    
public:
    astbloc()   = default;
    ~astbloc()  = default;

    utils::result_code build();
};

}

