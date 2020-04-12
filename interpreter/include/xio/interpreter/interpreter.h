#pragma once


#include <xio/interpreter/interpreterdll.h>
#include <xio/interpreter/bloc.h>
#include <xio/interpreter/parsers/parsers.h>

namespace teacc
{

class INTERPRETERAPI interpreter : public bloc
{
    
    static parsers::teacc_grammar _rules;
    //parsers::parser               _parser;
    /*!
     * @brief
     */
    struct translation_unit
    {
        using collection    = std::vector<interpreter::translation_unit*>;
        using expect_ast    = utils::expect<ast::node*>;
        
        std::string         _file;
        const char*         _text = nullptr;
        lexer::lexscanners  _lexical_data;
        ast::node*          _ast  = nullptr;
        //...to be continued...
        
        translation_unit::expect_ast compile();  
        ~translation_unit();
    };
    
public:
    interpreter() = default;
    ~interpreter() override;
    
    alu jsr() override;
    utils::result_code run();
    static parsers::teacc_grammar& grammar();
private:
    utils::result_code build();
    translation_unit::collection _units;
    translation_unit*            _unit = nullptr;
};

}
