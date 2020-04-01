#include <xio/interpreter/bloc.h>


namespace teacc
{

bloc::bloc(const bloc& a_bloc)
{
    // implement;
    _instructions = a_bloc._instructions;
    _static_blocs = a_bloc._static_blocs;
    _static_vars  = a_bloc._static_vars;

    //@todo duplicate local vars and blocs, recursively:

}

bloc::bloc(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_acc): xio(a_parent,a_token,a_acc)
{

}

utils::result_code bloc::push_i(xio::shared a_i)
{
    if(!_instructions)
        _instructions = std::make_shared<xio::collection>();
    
    _instructions->push_back(a_i);
    return utils::notification::code::accepted;
}


bloc::~bloc()
{
    std::cout << __PRETTY_FUNCTION__ << " \\O/\n";
}



bloc& bloc::operator = (const bloc& a_bloc)
{
    // implement;
    _instructions = a_bloc._instructions;
    _static_blocs = a_bloc._static_blocs;
    _static_vars  = a_bloc._static_vars;

    //@todo duplicate local vars and blocs, recursively:

    return *this;
}




alu bloc::jsr()
{
    if(!_instructions)
        return *unit();
    for(auto x : *_instructions)
    {
        *acc = x->jsr();
        if(x->mem.leave) return *acc;
    }
    return *acc;
}


xio::shared bloc::query_var(const std::string& var_id)
{
    xio::shared This;

    if((This = query_var_local(var_id)))
        return This;

    if(_parent)
    {
        if(bloc* b; (b=_parent->to<bloc>()))
            return b->query_var(var_id);    
    }

    return nullptr;
}


xio::shared bloc::query_var_local(const std::string& var_id)
{
    if(!_vars) return nullptr;
    for(auto v : *_vars) if(v->t0->attribute() == var_id) return v;
    return nullptr;
}


 utils::result_code bloc::push_v(lexer::type::token_t* a_token)
 {
     if(!a_token)
        return {(
            utils::notification::push(), 
            utils::notification::type::internal," :",
            __PRETTY_FUNCTION__, " - unexpected nullptr for required token instance."
        )};
    
    if(!_vars)
        _vars = std::make_shared<xio::collection>();
    
    _vars->push_back(var::make(_parent ? _parent->query_child(this) : xio::get_global_root() , a_token, nullptr));
    return utils::notification::code::accepted;
 }



 utils::result_code bloc::push_b(xio::shared a_bloc)
 {
     return utils::notification::code::implement;
 }


bloc::shared bloc::make(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_acc)
{
    return std::make_shared<bloc>(a_parent, a_token, a_acc);
}


}