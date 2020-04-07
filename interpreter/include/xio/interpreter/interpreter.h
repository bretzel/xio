#pragma once


#include <xio/interpreter/interpreterdll.h>
#include <xio/interpreter/bloc.h>
#include <xio/interpreter/parsers/ast.h>



namespace teacc
{

class INTERPRETERAPI interpreter : public bloc
{
    
public:
    interpreter() = default;
    ~interpreter() override = default;
    
    alu jsr() override;
    utils::result_code run();
    
    
private:
    ast::astbloc       _ast;
    utils::result_code build();

};

}
