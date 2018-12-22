#pragma once
#include <xio++/interpreter/kernel/xio.hpp>


namespace xio {

class xio_api variable : public xio_t
{

public:
    using list_t = std::vector<variable*>;

    variable();
    variable(object* a_parent, token_t* a_token, alu* a_alu = nullptr);

    virtual ~variable();
};

}

