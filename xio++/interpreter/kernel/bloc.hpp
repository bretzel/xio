/*
 * Copyright (c) 2019 Serge Lussier <lussier.serge@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


#pragma once

#include <xio++/interpreter/kernel/variable.hpp>


namespace xio {

/*!
    @brief Xio's Bloc base to all derived specialized blocs such as Functions, Objects, Function calls, Source-Blocs. 

    ...But a Bloc or Stack at runtime, is as big as its definition. 

    @author @copy;2019, Serge Lussier (bretzelus) lussier.serge@gmail.com
*/


class xio_api bloc_t : public xio_t
{

public:
    class xio_api rt_function : public xio_t {
        std::string _name; // Sous réserve...L'identificateur de cette fonction devrait être lié au jetton et non défini par stockage locale dupliqué.
    public:
        using list_t = std::vector<rt_function*>;
        using map = std::map<std::string, rt_function*>;

        rt_function() :xio_t() {}
        rt_function(xio_t* a_parent, const std::string& a_id) : xio_t(a_parent, 0, 0), _name(a_id) {}
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
        rtf(xio_t* a_parent, const std::string& a_name, C& rt_inst, rt_fn_t a_fn) :rt_function(a_parent, a_name), rt_obj(&rt_inst), rt_fn(a_fn) {}

        ~rtf() override {}


        /*!
            @brief rt calls interpreter script source function.
        */
        R operator()(const A& ...args) {
            auto param = [](auto a) {
//                logdebugpfn << " a= '" << logger::Yellow << a << ":\n";
                return alu(a);
            };
            alu::list_t params = { param(args)... };
            // alu a = call_script_function(_name, params);
            // return a.value<R>();

            return R();

        }

        template <std::size_t ... Is> alu accumulate(alu::list_t const& params, std::index_sequence<Is...> const&)
        {

            return (rt_obj->*rt_fn)(params[Is].value<A>()...);
        }

        alu operator()(const alu::list_t& params) {
  //          logdebug << " xio_stack::rtf(const alu::list_t&):  " << Ends;
            for (auto a : params) {
  //              logdebug << "arg: [" << logger::Yellow << a() << logger::Reset << "]\n";
            }
            return accumulate(params, std::index_sequence_for<A...>{});
        }

        template <std::size_t ... Is> alu accumulate(alu::list_t& params, std::index_sequence<Is...> const&)
        {
            return (rt_obj->*rt_fn)(params[Is].value<A>()...);
        }

    };
protected:
    
    rt_function::map rt_functions;

    variable::list_t* _variables   = nullptr;
    xio_t::list_t* _instructions = nullptr;
    xio_t::list_t* _functions = nullptr;
    xio_t::list_t*  _objects = nullptr;


public:
    bloc_t();
    virtual ~bloc_t();

    template<typename C, typename R, typename ...A> rt_function* bind(const std::string& a_id, C& rt_instance, R(C::* rt_fn)(A...)) {
        rtf<C, R, A...>* inst = new rtf<C, R, A...>(this, a_id, rt_instance, rt_fn);
        rt_functions[a_id] = inst;
        return inst;
    }

    virtual alu jsr_rtf(const std::string& a_id, const alu::list_t& args);
    virtual alu jsr();

protected:
    friend class compiler;
    //. ..
    virtual variable* query_local_variable(const std::string& vid) const;
    virtual variable* query_variable(const std::string& vid) const;

    virtual bloc_t* query_object(const std::string& oid) const;


};

}

