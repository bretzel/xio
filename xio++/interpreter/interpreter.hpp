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

template<typename C, typename R, typename ...A>
class methodology{
    
    std::string _name;
    
    C* rt_obj = nullptr;
    
    using rt_fn_t = R(C::*)(A...);
    rt_fn_t rt_fn = nullptr;
    //using out_fn_t  = alu(const alu::list_t& params);
    
public:
    std::string& name() { return _name; }
    R operator()(A& ...args){
        auto param = [](auto a){
            return alu(a);
        };
        alu::list_t params = { param(args)...};
        // alu a = interpreter::enter(_name, params);
        // return a.value<R>();
        return R();
    }
    
    alu operator()(const alu::list_t& params){
        //params => A..args ???
        //alu a = (rt_obj->*rt_fn)(args...);
        return a;
    }
};



}
