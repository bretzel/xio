#include <xio/lexer/rtlx_lexer.h>
#include <xio/lexer/lexer.h>
#include <map>


namespace xio::lexer
{

using utils::rtlx;

xio::lexer::rtlx_lexer::~rtlx_lexer()
{

}
xio::lexer::rtlx_lexer::rtlx_lexer(const std::string &dlname) : rtlx(dlname)
{

}


utils::notification::code xio::lexer::rtlx_lexer::execute()
{
    
    return utils::notification::code::implement;
}


}



BEGIN_EXPORT


std::string highlight(xio::lexer::rtlx_lexer::shared _instance, const std::string& _src)
{
    if(!_instance) return "";
    xio::lexer::lexscanners lexer;
    lexer.source() = _src.c_str();
    lexer.Scan();
    // .. for ( auto& t : lexer.tokens() )
    //{
    //     highlight ...
    //}
    return "this source is highlighted.";
    
}

CREATE_INSTANCE_C_IMPL
{
    return std::make_shared<xio::lexer::rtlx_lexer>(aId);
};

DELETE_INSTANCE_C_IMPL
{
    _dll_instance = nullptr;
};

xio::utils::rtlx::interface_map export_symbols()
{
    return { 
        {"highlight",(void*)highlight}
    };
            
}




END_EXPORT
