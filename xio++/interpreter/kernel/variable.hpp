#pragma once
#include <xio++/interpreter/kernel/xio.hpp>


namespace xio {



class bloc_t;

class xio_api variable : public xio_t
{
protected:
    type_t::T _effective_type = type_t::any;
    bloc_t* _bloc_type = nullptr;

    // std::string _type_id;
public:
    using list_t = std::vector<variable*>;

    variable();
    variable(object* a_parent, token_t* a_token, alu* a_alu = nullptr);

    virtual ~variable();
};

}

