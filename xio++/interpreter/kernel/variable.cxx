#include "variable.hpp"


namespace xio {
variable::variable()
{

}

variable::variable(object * a_parent, token_t * a_token, alu * a_alu):xio_t(a_parent, a_token, a_alu)
{

}


variable::~variable()
{
}

}

