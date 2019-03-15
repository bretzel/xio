//
// Created by bretzelus on 19-03-14.
//

#include "ast.hpp"


namespace xio{



ast::ast(ast *a_ast, term_t* t, token_t *a_token):m_term(t),m_token(a_token)
{
    if (a_ast) {
        a_ast->append(this);
    }
}

ast::~ast() {
    m_children.clear();
}

void ast::append(ast* a)
{
    a->m_parent = this;
    m_children.push_back(a);

    // TODO: insérer une instruction return ici
}


}