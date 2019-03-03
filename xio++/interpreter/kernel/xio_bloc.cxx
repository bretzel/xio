#include "xio_bloc.hpp"

namespace xio {
xio_bloc::xio_bloc()
{

}

xio_bloc::xio_bloc(xio_t * a_parent, token_t::list_t * a_tokens, token_t::cursor a_cursor):xio_stack(a_parent, nullptr, nullptr), m_tokens(a_tokens),m_cursor(a_cursor)
{

}



xio_bloc::~xio_bloc()
{
}
}