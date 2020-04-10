#pragma once


#include <xio/interpreter/interpreterdll.h>
#include <xio/interpreter/bloc.h>
#include <xio/interpreter/parsers/parsers.h>

namespace teacc
{

class INTERPRETERAPI interpreter : public bloc
{
    
    parsers::teacc_grammar _rules;
    /*!
     * @brief
     */
    struct tree_unit
    {
        using collection = std::vector<interpreter::tree_unit*>;
        using ast = utils::expect<ast::node*>;
        
        std::string         _file;
        const char*         _text = nullptr;
        parsers::parse_tree _parser;
        lexer::lexscanners  _lexical_data;
        //...to be continued...
        
        tree_unit::ast compile();
        
        ~tree_unit();
    };
    
public:
    interpreter() = default;
    ~interpreter() override;
    
    alu jsr() override;
    utils::result_code run();
    
private:
    utils::result_code build();
    tree_unit::collection _units;
    tree_unit*            _unit = nullptr;
};

}
