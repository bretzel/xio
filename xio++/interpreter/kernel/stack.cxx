#include "stack.hpp"

namespace xio {

xio_stack::xio_stack():xio_t()
{
}

xio_stack::xio_stack(object * a_parent, token_t * a_token, alu * a_alu):xio_t(a_parent,a_token,a_alu)
{

}


xio_stack::~xio_stack()
{
    //...
    if(rt_functions.empty()) return;
    
    for( auto k : rt_functions ) {
        delete k.second;
    }
    rt_functions.clear();
}


alu xio_stack::jsr()
{
    if (!_instructions)
        return *acc;
    for (auto i : *_instructions) {
        *acc = i->jsr();
        if (i->mem.leave) return *acc;
    }
    return *acc;
}


variable * xio_stack::query_local_variable(token_t* a_token)
{
    if (!_local_vars) return nullptr;
    for (auto v : *_local_vars) if (v->t0->attribute() == a_token->attribute()) return v;
    return nullptr;
}

variable * xio_stack::query_local_variable(const char* var_id)
{
    if (!_local_vars) return nullptr;
    for (auto v : *_local_vars) if (v->t0->attribute() == var_id) return v;
    return nullptr;
}

variable * xio_stack::query_variable(token_t* a_token)
{
    variable* v = query_local_variable(a_token);
    if (v) return v;

    if (xio_stack* s; (s = parent<xio_stack>())) 
        return s->query_variable(a_token);

    return nullptr;
}

variable * xio_stack::query_variable(const char * var_id)
{
    variable* v = query_local_variable(var_id);
    if (v) return v;

    if (xio_stack* s; (s = parent<xio_stack>()))
        return s->query_variable(var_id);

    return nullptr;
}

void xio_stack::push_instruction(xio_t * x)
{
    if (!_instructions)
        _instructions = new xio_t::list_t;
    _instructions->push_back(x);

}

void xio_stack::push_variable(variable * var)
{
    if (!_local_vars)
        _local_vars = new variable::list_t;
    _local_vars->push_back(var);
}

int xio_stack::push()
{
    
    return 0;
}


}

