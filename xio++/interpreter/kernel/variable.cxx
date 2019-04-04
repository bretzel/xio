#include "variable.hpp"


namespace xio {
variable::variable()
{

}

variable::variable(object * a_parent, token_t * a_token, alu * a_alu, type_t::T a_type, bool a_static):xio_t(a_parent, a_token, a_alu),
_effective_type(a_type)
{
    if(a_static)
        mem.sstatic = 1;
}


variable::~variable()
{
}

std::string variable::informations() {

    string_t str;
    if(mem.sstatic)
        str << "static ";
    str << type_t::name(_effective_type) << ' ' << t0->attribute();
   
    return str();

}

}

