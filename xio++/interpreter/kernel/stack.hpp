#pragma once

#include <xio++/xio++.hpp>
#include <xio++/interpreter/kernel/variable.hpp>

namespace xio {

class xio_api xio_stack : public xio_t
{

    variable::list_t*       _local_vars    = nullptr;
    xio_t::list_t*          _instructions  = nullptr;
    xio_t::list_t*          _functions     = nullptr;

public:
    
    xio_stack();
    xio_stack(object* a_parent, token_t* a_token=nullptr, alu* a_alu=nullptr);

    virtual ~xio_stack();


    alu jsr() override;

    variable* query_local_variable(token_t* a_token);
    variable* query_local_variable(const char *var_id);
    variable* query_variable(token_t* a_token);
    variable* query_variable(const char* var_id);

    void push_instruction(xio_t* x);
    void push_variable(variable* var);
    

protected:
    virtual int push();

};


}

