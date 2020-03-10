#pragma once

#include <xio/utils/rtlx.h>
#include <memory>
namespace xio::lexer
{


class rtlx_lexer : public utils::rtlx 
{
public:

    using shared = std::shared_ptr<rtlx_lexer>;

    rtlx_lexer() = delete;
    ~rtlx_lexer() override;
    rtlx_lexer(const std::string& dlname);
    
    
    utils::notification::code execute() override;
    



};



}
