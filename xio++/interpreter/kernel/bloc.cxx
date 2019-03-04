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



}