#pragma once


#include <xio/interpreter/interpreterdll.h>
#include <xio/interpreter/bloc.h>
#include <xio/interpreter/parsers/parsers.h>

namespace teacc
{

class INTERPRETERAPI interpreter : public bloc
{
    const char* _source = nullptr;
    
    /*!
     * @brief
     */
    struct input_file_t
    {
        using collection = std::vector<interpreter::input_file_t>;
        std::string         _file;
        const char*         _text = nullptr;
        parsers::parse_tree _parser;
        lexer::lexscanners  _lexical_data;
        //...
    };
    
public:
    interpreter() = default;
    ~interpreter() override = default;
    
    alu jsr() override;
    utils::result_code run();
    
private:
    parsers::parse_tree pstree;
    utils::result_code build();

};

}
