#pragma once

#include <xio++/interpreter/module.hpp>
#include <xio++/interpreter/kernel/alu.hpp>
namespace xio{




class xio_api interpreter : public xio_module{
        
public:

    class rtfc {
    public:
        using list_t = std::vector<rtfc*>;
    };
    
    /*!
        @brief  runtime function call.

    */
    template<typename C, typename R, typename ...A> class rtfc_t : public rtfc{
        std::string _name;
        C* rt_obj = nullptr;

        using rt_fn_t = R(C::*)(A...);
        rt_fn_t rt_fn = nullptr;
        
    public:
        rtfc_t(){}
        rtfc_t(const std::string a_name, C& rt_inst, rt_fn_t a_fn): _name(a_name), rt_obj(&rt_inst),rt_fn(a_fn) {}
            
        ~rt_fc_t() {}
        
        R operator()(const A& ...args) {
            auto param = [](auto a) {
                return alu(a);
            };
            alu::list_t params = { param(args)... };
            // alu a = interpreter::enter(_name, params);
            // return a.value<R>();
            return R();
        }

        template <std::size_t ... Is> alu accumulate(alu::list_t const & params, std::index_sequence<Is...> const &)
        {
            return (rt_obj->*rt_fn)(params[Is].value<A>()...);
        }

        alu operator()(const alu::list_t& params) {
            return accumulate(params, std::index_sequence_for<A...>{});
        }
    };

    //rtfc& 

private:
    rtfc::list_t* g_fdfn=nullptr;
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
    
    template <std::size_t ... Is> alu pack (alu::list_t const & params, std::index_sequence<Is...> const &)
    { return (rt_obj->*rt_fn)(params[Is].value<A>()...); }

    alu operator()(const std::string& f_name, const alu::list_t& params){        
        return pack(params, std::index_sequence_for<A...>{});
    }
};



}
