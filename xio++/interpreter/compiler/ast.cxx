//
// Created by bretzelus on 19-03-14.
//

#include "ast.hpp"


namespace xio{



ast::ast(ast *a_ast, token_t *a_token):object(a_ast), m_token(a_token){

}

ast::~ast() {

}


}