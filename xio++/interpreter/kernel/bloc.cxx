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

bloc_t* bloc_t::query_object(const std::string& oid) const
{
    if (!_objects) return nullptr;
    for (auto o : *_objects)
        if (o->token()->attribute() == oid) return  o;
    return nullptr;
}



}