#pragma once

#include <xio++/xio++.hpp>
#include <xio++/interpreter/kernel/variable.hpp>
#include <xio++/journal/logger.hpp>

namespace xio {

class xio_api xio_stack : public xio_t
{

    variable::list_t*       _local_vars    = nullptr; // inner copy { variable::push() }
    xio_t::list_t*          _instructions  = nullptr; // no copy
    xio_t::list_t*          _functions     = nullptr; // inner copy { xio_stack <- xio_function_t::instanciate() }

    // Limiter la recursion (infinie, entre autre).
    uint32_t nth=0;
    // ------------------------------------------
    xio_t* __dup(xio_t* a_parent);

public:
    class xio_api rt_function : public xio_t{
        std::string _name;
    public:
        using list_t = std::vector<rt_function*>;
        using map = std::map<std::string, rt_function*>;

        rt_function():xio_t() {}
        rt_function(xio_t* a_parent, const std::string& a_id): xio_t(a_parent,0,0),_name(a_id) {}
        virtual ~rt_function() { _name.clear(); }

        std::string& name() { return _name; }

        virtual alu operator()(const alu::list_t& args) = 0;
    };

    /*!
        @brief  runtime struct::function call.

    */
    template<typename C, typename R, typename ...A> class rtf : public rt_function {

        C* rt_obj = nullptr;

        using rt_fn_t = R(C::*)(A...);
        rt_fn_t rt_fn = nullptr;

    public:
        rtf() {}
        rtf(xio_t* a_parent, const std::string& a_name, C& rt_inst, rt_fn_t a_fn) :rt_function(a_parent,a_name), rt_obj(&rt_inst), rt_fn(a_fn) {}

        ~rtf() override {}


        /*!
            @brief rt calls interpreter script source function.
        */
        R operator()(const A& ...args) {
            auto param = [ ](auto a) {
                logdebugpfn << " a= '" << logger::Yellow << a << ":\n";
                return alu(a);
            };
            alu::list_t params = { param(args)... };
            // alu a = call_script_function(_name, params);
            // return a.value<R>();
            
            return R();

        }

        template <std::size_t ... Is> alu accumulate(alu::list_t const& params, std::index_sequence<Is...> const &)
        {
            
            return (rt_obj->*rt_fn)(params[Is].value<A>()...);
        }

        alu operator()(const alu::list_t& params) {
            logdebug << " xio_stack::rtf(const alu::list_t&):  " << Ends;
            for( auto a : params ) {
                logdebug << "arg: [" << logger::Yellow << a() << logger::Reset << "]\n";
            }
            return accumulate(params, std::index_sequence_for<A...>{});
        }

        template <std::size_t ... Is> alu accumulate(alu::list_t& params, std::index_sequence<Is...> const &)
        {
            return (rt_obj->*rt_fn)(params[Is].value<A>()...);
        }

    };



    xio_stack();
    xio_stack(object* a_parent, token_t* a_token=nullptr, alu* a_alu=nullptr);

    virtual ~xio_stack();


    alu jsr() override;
    virtual alu jsr_rtf(const std::string& a_id, const alu::list_t& args);


    variable* query_local_variable(token_t* a_token);
    variable* query_local_variable(const char *var_id);
    variable* query_variable(token_t* a_token);
    variable* query_variable(const char* var_id);

    void push_instruction(xio_t* x);
    void push_variable(variable* var);
    
    
    template<typename C, typename R, typename ...A> rt_function* bind(const std::string& a_id, C& rt_instance, R(C::*rt_fn)(A...)) {
        rtf<C, R, A...>* inst = new rtf<C, R, A...>(this,a_id, rt_instance, rt_fn);
        rt_functions[a_id] = inst;
        return inst;
    }

protected:
    virtual std::size_t push();
    virtual std::size_t pop();

    rt_function::map rt_functions;
    //virtual xio_t* instanciate(); // xio_t::jsr() <- xio*::jsr();

};


}

