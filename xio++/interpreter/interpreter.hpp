#pragma once

#include <xio++/interpreter/module.hpp>
#include <xio++/interpreter/kernel/alu.hpp>
#include <xio++/journal/logger.hpp>
namespace xio{




class xio_api interpreter : public xio_module{
        
public:

    class xio_api rtfc {
        std::string _name;
    public:
        using list_t = std::vector<rtfc*>;
        rtfc() {}
        rtfc(const std::string& a_id):_name(a_id){}
        virtual ~rtfc() {_name.clear();  }

        std::string& name() { return _name; }

        virtual alu operator()(alu::list_t& args) = 0;
    };
    
    /*!
        @brief  runtime function call.

    */
    template<typename C, typename R, typename ...A> class rtfc_t : public rtfc{
        
        C* rt_obj = nullptr;

        using rt_fn_t = R(C::*)(A...);
        rt_fn_t rt_fn = nullptr;
        
    public:
        rtfc_t(){}
        rtfc_t(const std::string& a_name, C& rt_inst, rt_fn_t a_fn):rtfc(a_name), rt_obj(&rt_inst),rt_fn(a_fn) {}
            
        ~rtfc_t() override {}
        
        
        /*!
            @brief rt calls interpreter script source function.
        */
        R operator()(const A& ...args) {
            auto param = [](auto a) {
                logdebugpfn << " a= '" << logger::Yellow << a << ":\n";
                return alu(a);
            };
            alu::list_t params = { param(args)... };
            // alu a = interpreter::enter(_name, params);
            // return a.value<R>();
            return R();
        }

        template <std::size_t ... Is> alu accumulate(alu::list_t const& params, std::index_sequence<Is...> const &)
        {
            return (rt_obj->*rt_fn)(params[Is].value<A>()...);
        }

        alu operator()(const alu::list_t& params) {
//             logdebugpfn << ": " << Ends;
            return accumulate(params, std::index_sequence_for<A...>{});
        }

        template <std::size_t ... Is> alu accumulate(alu::list_t& params, std::index_sequence<Is...> const &)
        {
            return (rt_obj->*rt_fn)(params[Is].value<A>()...);
        }

        alu operator()(alu::list_t& args) override {
            logdebug << " interpreter::rtfc_t(alu::list_t):  " << Ends;
            for( auto a : args ) {
                logdebug << "arg: [" << logger::Yellow << a() << logger::Reset << "]\n";
            }
            return accumulate(args, std::index_sequence_for<A...>{});
        }
    };

    //rtfc& 
private:
    rtfc::list_t g_fdfn;
public:
    interpreter();

    template<typename C, typename R, typename ...A> rtfc* bind(const std::string& a_id, C& rt_instance, R(C::*rt_fn)(A...) ) {
        interpreter::rtfc_t<C,R,A...>* inst = new interpreter::rtfc_t<C, R,A...>(a_id, rt_instance, rt_fn);
        g_fdfn.push_back(inst);
        return g_fdfn.back();
    }

    ~interpreter() override;
    alu jsr() override;
    
}; 



}
