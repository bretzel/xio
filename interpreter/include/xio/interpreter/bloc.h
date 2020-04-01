// &copy; 2020, Serge Lussier ( lonesomecoder; lussier.serge@gmail.com )



#pragma once

#include <xio/interpreter/var.h>


namespace teacc{

class INTERPRETERAPI bloc : public xio
{

public:

    using shared = std::shared_ptr<bloc>;
    bloc() = default;
    ~bloc() override;// = default;
    
    bloc(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_acc);

    alu jsr() override;
protected:
    xio::shared_collection  _instructions;
    xio::shared_collection  _static_vars;
    xio::shared_collection  _static_blocs;

    xio::shared_collection  _vars;
    xio::shared_collection  _blocs; ///< Sub block/stacks/objects

    xio::shared copy(xio::shared a_stack) override;

    virtual xio::shared query_var(const std::string& var_id);
    virtual xio::shared query_var_local(const std::string& var_id);

    virtual utils::result_code push_i(xio::shared a_i);
    virtual utils::result_code push_v(lexer::type::token_t* a_token);
    virtual utils::result_code push_b(xio::shared a_bloc);

    static bloc::shared make(xio::shared a_parent, lexer::type::token_t* a_token, alu::shared a_acc);


};

}