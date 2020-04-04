#pragma once


#include <xio/interpreter/interpreterdll.h>
#include <xio/interpreter/bloc.h>
#include <xio/interpreter/parsers/grammar.h>




namespace teacc
{

class INTERPRETERAPI interpreter : public bloc
{

    parsers::teacc_grammar _grammar;
    
public:
    interpreter() = default;
    ~interpreter() override = default;

    alu jsr() override;
    
    utils::result_code run();

};

}
