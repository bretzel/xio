#pragma once

#include <xio++/interpreter/module.hpp>
#include <xio++/interpreter/kernel/alu.hpp>
namespace xio{

    


class xio_api interpreter : public xio_module{
        
public:
    interpreter();
    
    ~interpreter() override;
    alu jsr() override;
    
}; 

template<typename C, typename R, typename ...A> class methodology{
    
    std::string _name;
    
    C* rt_obj = nullptr;
    
    using rt_fn_t = R(C::*)(A...);
    rt_fn_t rt_fn = nullptr;
    //using out_fn_t  = alu(const alu::list_t& params);
    
public:
    
    methodology(C& a_obj, rt_fn_t fn){
        rt_obj = &a_obj;
        rt_fn = fn;
    }
    
    std::string& name() { return _name; }
    R operator()(const A& ...args){
        auto param = [](auto a){
            return alu(a);
        };
        alu::list_t params = { param(args)...};
        // alu a = interpreter::enter(_name, params);
        // return a.value<R>();
        return R();
    }
    
    template <std::size_t ... Is> alu op_helper (alu::list_t const & params, std::index_sequence<Is...> const &)
    { return (rt_obj->*rt_fn)(params[Is].value<A>()...); }

    alu operator()(const std::string& f_name, const alu::list_t& params){        
        return op_helper(params, std::index_sequence_for<A...>{});
    }
};



}
