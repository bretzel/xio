#include "bloc.hpp"


namespace xio {

bloc_t::bloc_t()
{
}


bloc_t::~bloc_t()
{
}


alu bloc_t::jsr_rtf(const std::string& a_id, const alu::list_t& args)
{
    bloc_t::rt_function* rtf = rt_functions[a_id];
    if (rtf)
        return (*rtf)(args);
    return alu{ 0 };
}

alu bloc_t::jsr()
{
    return alu();
}

variable* bloc_t::query_local_variable(const std::string& vid) const
{
    if (!_variables) return nullptr;

    for (auto v : *_variables)
        if (v->token()->attribute() == vid) return v;

    return nullptr;
}

variable* bloc_t::query_variable(const std::string& vid) const
{
    bloc_t* par = parent<bloc_t>();
    if(variable* v; (v = query_local_variable(vid))) return v;
  
    return (par ? par->query_variable(vid) : nullptr);
}

xio_t* bloc_t::push_variable(token_t* a_token, bool static_stor, type_t::T a_type)
{
    if (!_variables)
        _variables = new variable::list_t;

    variable* v;
    _variables->push_back(new variable(this, a_token, nullptr, a_type, static_stor));
    v = _variables->back();
    if(static_stor)
        v->mem.sstatic = 1;


    return _variables->back();
}

bloc_t* bloc_t::query_object(const std::string& oid) const
{
    if (!_objects) return nullptr;
    for (auto o : *_objects)
        if (o->token()->attribute() == oid) return  dynamic_cast<bloc_t*>(o);
    
    return nullptr;
}



}
