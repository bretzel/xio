/*
 * Copyright (c) 2017, Serge Lussier (bretzelus), lussier.serge@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list_t of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list_t of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author/organization nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Serge Lussier (bretzelus) ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Serge Lussier (bretzelus) BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#pragma once


#include <xio++/xio++.hpp>
#include <xio++/interpreter/kernel/alu.hpp>


namespace xio {


struct xio_api  _nil_{};
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wpmf-conversions"

struct xio_api function_t{

    void* _object  = nullptr;
    ///@todo REMOVE _rtfunc. MSVC fails(it seem it's  doesn't want) to  convert from void* to R(T::*)(params...);
    void* _function_ptr = nullptr; // type f = reinterpret_cast<R(Args...)*>(_rtfunc);
    using fntype_t = void();
    using list_t = std::vector<function_t*>;
    //std::string _id;

    function_t() = default;

    function_t(std::string a_id, void* a_rtobj, void* a_rtfunc):_object(a_rtobj), _function_ptr(a_rtfunc),_id(std::move(a_id)) {
        //lfnl << " id:" << _id << " ; ...\n";
    }
    function_t(function_t&& f)noexcept{
        //lfnl << " id:" << _id << " ; ...\n";
        _id     = f._id; f._id.clear();
        _object  = f._object; f._object = nullptr;
        _function_ptr = f._function_ptr; f._function_ptr = nullptr;
    }
//    function_t(const function_t&& f)noexcept {
//        //lfnl << " id:" << _id << " ; ...\n";
//        _id     = f._id;
//        _object  = f._object;
//        _function_ptr = f._function_ptr;
//    }
    function_t(const function_t& src) {
        _function_ptr = src._function_ptr;
        _object = src._object;
        _id = src._id;
    }

    function_t& operator=(function_t&& src) noexcept{
        std::swap(_id, src._id);
        src._id.clear();
        _function_ptr = src._function_ptr;
        _object  = src._object;

        src._function_ptr = src._object = nullptr;
        
        return *this;
    }
    //function_t& operator=(const function_t&& src) { std::move(src); return *this;}
    function_t& operator=(const function_t& src) =default;
//    {
//        _rtfunc = src._rtfunc;
//        _rtobj = src._rtobj;
//        _id = src._id;
//        return *this;
//    }

    virtual ~function_t(){ _id.clear(); }

    virtual alu operator()(const alu::list_t&){return alu();}


    template<typename T> T& rt_object()
    {
        return *reinterpret_cast<T*>(_object);
    }

    virtual alu call(alu::list_t& params);

    

    std::string _id;

};



template<typename T, typename R> struct   function0 : public function_t
{
    using fntype_t = R(T::*)();
    fntype_t fn = nullptr;

    using class_type = T;
    using parent_class = function_t;

    function0()=default;
    function0(function0&& src) noexcept:parent_class(std::move(src)){}
    function0(const function0&& src)noexcept :parent_class(std::move(src)){}
    function0(const function0& src):parent_class(src) {}
    function0(const std::string& a_id,T* a_rtobj, fntype_t a_method): parent_class(a_id,(void*)a_rtobj,nullptr),fn(a_method){}

    ~function0()= default;


    function0& operator = (const function0& src){
        function_t::operator=(src);        
        return *this;
    }

    function0& operator=(function0&& src) noexcept{ ///blah blah ....
        function_t::operator=(std::move(src));        
        return *this;
    }

//    function0& operator=(const function0&& src)noexcept {
//        function_t::operator=(std::move(src));
//        return *this;
//    }
    alu operator()(const alu::list_t& params) override {        
        return (reinterpret_cast<class_type*>(_object)->*fn)();
    }
    R operator()()
    {
        alu::list_t params;
        alu var = call(params);
        return var.value<R>();
    }

};


template<typename T, typename R, typename _A1> struct   function1 : public function_t
{
    using fntype_t = R(T::*)(_A1);
    fntype_t fn = nullptr;

    using class_type = T;
    using parent_class = function_t;
    function1(){}
    function1(function1&& src)noexcept:parent_class(std::move(src)){;}
    function1(const function1&& src)noexcept:parent_class(std::move(src)){}
    function1(const function1& src):parent_class(src) {}
    function1(const std::string& a_id,T* a_rtobj, fntype_t a_method): parent_class(a_id,(void*)a_rtobj,nullptr),fn(a_method){}

    virtual ~function1(){}


    function1& operator = (const function1& src){
        function_t::operator=(src);
        
        return *this;
    }

    function1& operator=(function1&& src)noexcept {
        function_t::operator=(std::move(src));
        
        return *this;
    }

//    function1& operator=(const function1&& src)noexcept {
//        function_t::operator=(std::move(src));
//        //lfnl <<   " id:" << _id << " ; ...\n";
//        return *this;
//    }
    alu operator()(const alu::list_t& params) override {
        _A1 a1 = params[0].value<_A1>();
        return (reinterpret_cast<class_type*>(_object)->*fn)(a1);
    }

    R operator()(_A1 _1)
    {
        std::vector<alu> params(1);// = std::vector<alu>(7);
        params = {alu(_1)};
        alu var = call(params);
        return var.value<R>();
    }

};

template<typename T, typename R, typename _A1,typename _A2> struct   function2 : public function_t
{
    using fntype_t = R(T::*)(_A1,_A2);
    fntype_t fn = nullptr;

    using class_type = T;
    using parent_class = function_t;
    function2(){}
    function2(function2&& src) noexcept :parent_class(std::move(src)){}
    function2(const function2&& src) noexcept :parent_class(std::move(src)){}
    function2(const function2& src):parent_class(src) {}
    function2(const std::string& a_id,T* a_rtobj, fntype_t a_method): parent_class(a_id,(void*)a_rtobj,nullptr),fn(a_method){}

    ~function2() override {}


    function2& operator = (const function2& src){
        function_t::operator=(src);
        
        return *this;
    }

    function2& operator=(function2&& src)  noexcept  {
        function_t::operator=(std::move(src));
        
        return *this;
    }

//    function2& operator=(const function2&& src) {
//        function_t::operator=(std::move(src));
//        lfnl <<   " id:" << _id << " ; ...\n";
//        return *this;
//    }
    alu operator()(const alu::list_t& params) override {
        _A1 a1 = params[0].value<_A1>();
        _A2 a2 = params[1].value<_A2>();
        
        return (reinterpret_cast<class_type*>(_object)->*fn)(a1,a2);
    }

    R operator()(_A1 _1,_A2 _2)
    {
        //std::cerr << __Y_FUNCTION__ << ":\n";
        std::vector<alu> params(2);// = std::vector<alu>(7);
        params = {alu(_1),alu(_2)};
        alu var = call(params);
        return var.value<R>();
    }

};

template<typename T, typename R, typename _A1,typename _A2, typename _A3> struct   function3 : public function_t
{
    using fntype_t = R(T::*)(_A1,_A2,_A3);
    fntype_t fn = nullptr;

    using class_type = T;
    using parent_class = function_t;
    function3(){}
    function3(function3&& src):parent_class(std::move(src)){}
    function3(const function3&& src)noexcept:parent_class(std::move(src)){}
    function3(const function3& src):parent_class(src) {std::cerr << " id:" << _id << " ; ...\n";}
    function3(const std::string& a_id,T* a_rtobj, fntype_t a_method): parent_class(a_id,(void*)a_rtobj,nullptr),fn(a_method){}

    virtual ~function3(){}


    function3& operator = (const function3& src){
        function_t::operator=(src);
        
        return *this;
    }

    function3& operator=(function3&& src)noexcept {
        function_t::operator=(std::move(src));
        
        return *this;
    }

//    function3& operator=(const function3&& src)noexcept {
//        function_t::operator=(std::move(src));
//
//        return *this;
//    }
    alu operator()(const alu::list_t& params) override {
        _A1 a1 = params[0].value<_A1>();
        _A2 a2 = params[1].value<_A2>();
        _A3 a3 = params[2].value<_A3>();
        
        return (reinterpret_cast<class_type*>(_object)->*fn)(a1,a2,a3);
    }

    R operator()(_A1 _1,_A2 _2,_A3 _3)
    {
        std::vector<alu> params(3);// = std::vector<alu>(7);
        params = {alu(_1),alu(_2),alu(_3)};
        alu var = call(params);
        return var.value<R>();
    }
};


template<typename T, typename R, typename _A1,typename _A2, typename _A3, typename _A4> struct   function4 : public function_t
{
    using fntype_t = R(T::*)(_A1,_A2,_A3,_A4);
    fntype_t fn = nullptr;

    using class_type = T;
    using parent_class = function_t;
    function4(){}
    function4(function4&& src):parent_class(std::move(src)){}
    function4(const function4&& src)noexcept:parent_class(std::move(src)){}
    function4(const function4& src):parent_class(src) {}
    function4(const std::string& a_id,T* a_rtobj, fntype_t a_method): parent_class(a_id,(void*)a_rtobj, nullptr),fn(a_method){}

    virtual ~function4(){}


    function4& operator = (const function4& src){
        function_t::operator=(src);
        
        return *this;
    }

    function4& operator=(function4&& src)noexcept {
        function_t::operator=(std::move(src));
        
        return *this;
    }

//    function4& operator=(const function4&& src)noexcept {
//        function_t::operator=(std::move(src));
//
//        return *this;
//    }
    alu operator()(const alu::list_t& params) override {
        _A1 a1 = params[0].value<_A1>();
        _A2 a2 = params[1].value<_A2>();
        _A3 a3 = params[2].value<_A3>();
        _A4 a4 = params[3].value<_A4>();

        return (reinterpret_cast<class_type*>(_object)->*fn)(a1,a2,a3,a4);
    }

    R operator()(_A1 _1,_A2 _2,_A3 _3,_A4 _4)
    {
        std::vector<alu> params(4);// = std::vector<alu>(7);
        params = {alu(_1),alu(_2),alu(_3),alu(_4)};
        alu var = call(params);
        return var.value<R>();
    }
};



template<typename T, typename R, typename _A1,typename _A2, typename _A3, typename _A4, typename _A5> struct   function5 : public function_t
{
    using fntype_t = R(T::*)(_A1,_A2,_A3,_A4,_A5);
    fntype_t fn = nullptr;
    using class_type = T;
    using parent_class = function_t;
    function5(){}
    function5(function5&& src):parent_class(std::move(src)){}
    function5(const function5&& src)noexcept:parent_class(std::move(src)){}
    function5(const function5& src):parent_class(src) {}
    function5(const std::string& a_id,T* a_rtobj, fntype_t a_method): parent_class(a_id,(void*)a_rtobj, nullptr), fn(a_method){}

    virtual ~function5(){}


    function5& operator = (const function5& src){
        function_t::operator=(src);
        
        return *this;
    }

    function5& operator=(function5&& src)noexcept {
        function_t::operator=(std::move(src));
        
        return *this;
    }

//    function5& operator=(const function5&& src)noexcept {
//        function_t::operator=(std::move(src));
//
//        return *this;
//    }
    alu operator()(const alu::list_t& params) override {
        _A1 a1 = params[0].value<_A1>();
        _A2 a2 = params[1].value<_A2>();
        _A3 a3 = params[2].value<_A3>();
        _A4 a4 = params[3].value<_A4>();
        _A5 a5 = params[4].value<_A5>();
        return (reinterpret_cast<class_type*>(_object)->*fn)(a1,a2,a3,a4,a5);
    }
    R operator()(_A1 _1,_A2 _2,_A3 _3,_A4 _4,_A5 _5)
    {
        std::vector<alu> params(5);// = std::vector<alu>(7);
        params = {alu(_1),alu(_2),alu(_3),alu(_4),alu(_5)};
        alu var = call(params);
        return var.value<R>();
    }
};

template<typename T, typename R, typename _A1,typename _A2, typename _A3, typename _A4, typename _A5, typename _A6> struct   function6 : public function_t
{
    using fntype_t = R(T::*)(_A1,_A2,_A3,_A4,_A5, _A6);
    using class_type = T;
    fntype_t fn = nullptr;
    using parent_class = function_t;
    function6(){}
    function6(function6&& src)noexcept:parent_class(std::move(src)){}
    function6(const function6&& src)noexcept:parent_class(std::move(src)){}
    function6(const function6& src):parent_class(src) {}
    function6(const std::string& a_id,T* a_rtobj, fntype_t a_method): parent_class(a_id,(void*)a_rtobj, nullptr),fn(a_method){}

    virtual ~function6(){}


    function6& operator = (const function6& src){
        function_t::operator=(src);
        
        return *this;
    }

    function6& operator=(function6&& src)noexcept {
        function_t::operator=(std::move(src));
        
        return *this;
    }

//    function6& operator=(const function6&& src)noexcept {
//        function_t::operator=(std::move(src));
//
//        return *this;
//    }
    alu operator()(const alu::list_t& params) override {
        _A1 a1 = params[0].value<_A1>();
        _A2 a2 = params[1].value<_A2>();
        _A3 a3 = params[2].value<_A3>();
        _A4 a4 = params[3].value<_A4>();
        _A5 a5 = params[4].value<_A5>();
        _A6 a6 = params[5].value<_A6>();
        return (reinterpret_cast<class_type*>(_object)->*fn)(a1,a2,a3,a4,a5,a6);
    }
    R operator()(_A1 _1,_A2 _2,_A3 _3,_A4 _4,_A5 _5, _A6 _6)
    {
        std::vector<alu> params(6);// = std::vector<alu>(7);
        params = {alu(_1),alu(_2),alu(_3),alu(_4),alu(_5),alu(_6)};
        alu var = call(params);
        return var.value<R>();
    }
};


template<typename T, typename R, typename _A1,typename _A2, typename _A3, typename _A4, typename _A5, typename _A6, typename _A7>
struct   function7 : public function_t
{
    using fntype_t = R(T::*)(_A1,_A2,_A3,_A4,_A5, _A6, _A7);
    fntype_t fn = nullptr;
    using class_type = T;
    using parent_class = function_t;
    function7(){}
    function7(function7&& src):parent_class(std::move(src)){}
    function7(const function7&& src)noexcept:parent_class(std::move(src)){}
    function7(const function7& src):parent_class(src) {}
    function7(const std::string& a_id,T* a_rtobj, fntype_t a_method): parent_class(a_id,(void*)a_rtobj, nullptr),fn(a_method){}

    ~function7()override{}


    function7& operator = (const function7& src){
        function_t::operator=(src);
       
        return *this;
    }

    function7& operator=(function7&& src) {
        function_t::operator=(std::move(src));
       
        return *this;
    }

//    function7& operator=(const function7&& src)noexcept {
//        function_t::operator=(std::move(src));
//
//        return *this;
//    }
    alu operator()(const alu::list_t& params) override {
        _A1 a1; /*= params[0].value<_A1>(); */ params[0] >> a1;
        _A2 a2; /*= params[1].value<_A2>(); */ params[1] >> a2;
        _A3 a3; /*= params[2].value<_A3>(); */ params[2] >> a3;
        _A4 a4; /*= params[3].value<_A4>(); */ params[3] >> a4;
        _A5 a5; /*= params[4].value<_A5>(); */ params[4] >> a5;
        _A6 a6; /*= params[5].value<_A6>(); */ params[5] >> a6;
        _A7 a7; /*= params[6].value<_A7>(); */ params[6] >> a7;
        return (reinterpret_cast<class_type*>(_object)->*fn)(a1,a2,a3,a4,a5,a6,a7);
    }

    R operator()(_A1 _1,_A2 _2,_A3 _3,_A4 _4,_A5 _5, _A6 _6, _A7 _7)
    {
        std::vector<alu> params(7);// = std::vector<alu>(7);
        params = {alu(_1),alu(_2),alu(_3),alu(_4),alu(_5),alu(_6),alu(_7)};
        alu var = call(params);
        return var.value<R>();
    }
};


template<typename T, typename R, typename _1=_nil_,typename _2=_nil_, typename _3=_nil_, typename _4=_nil_, typename _5=_nil_, typename _6=_nil_, typename _7=_nil_, typename _Tail=_nil_>
struct   method : public function7<T,R,_1,_2,_3,_4,_5,_6,_7>
{
    using parent_class = function7<T,R,_1,_2,_3,_4,_5,_6,_7>;
    method()= default;
    method(const method& src): parent_class (src){}
    method(method&& src)noexcept: parent_class (std::move(src)) {}

    method& operator=(const method& src)
    {
        parent_class::operator=(src);
        return *this;
    }

    method& operator=(method&& src)noexcept
    {
        parent_class::operator=(std::move(src));
        return *this;
    }

};

template<typename T, typename R>
struct   method<T,R,_nil_,_nil_,_nil_,_nil_,_nil_,_nil_,_nil_> : public function0<T,R>
{
    using parent_class = function0<T,R>;
    method()= default;
    method(const method& src): parent_class (src){}
    method(method&& src)noexcept: parent_class (std::move(src)) {}

    method& operator=(const method& src)
    {
        parent_class::operator=(src);
        return *this;
    }

    method& operator=(method&& src)noexcept
    {
        parent_class::operator=(std::move(src));
        return *this;
    }

};

template<typename T, typename R, typename _1>
struct   method<T,R,_1,_nil_,_nil_,_nil_,_nil_,_nil_,_nil_> : public function1<T,R,_1>
{
    using parent_class = function1<T,R,_1>;
    using type = typename parent_class :: fntype_t;
    method()= default;
    method(const method& src): parent_class (src){}
    method(method&& src)noexcept: parent_class (std::move(src)) {}
    method(const std::string& a_id,T* a_rtobj, type a_method): parent_class(a_id,a_rtobj, a_method){}
    method& operator=(const method& src)
    {
        parent_class::operator=(src);
        return *this;
    }

    method& operator=(method&& src)noexcept
    {
        parent_class::operator=(std::move(src));
        return *this;
    }

};

template<typename T, typename R, typename _1,typename _2>
struct   method<T,R,_1,_2,_nil_,_nil_,_nil_,_nil_,_nil_> : public function2<T,R,_1,_2>
{
    using parent_class = function2<T,R,_1,_2>;
    using type = typename parent_class :: fntype_t;
    method()= default;
    method(const method& src): parent_class (src){}
    method(method&& src)noexcept: parent_class (std::move(src)) {}
    method(const std::string& a_id,T* a_rtobj, type a_method): parent_class(a_id,a_rtobj, a_method){}
    method& operator=(const method& src)
    {
        parent_class::operator=(src);
        return *this;
    }

    method& operator=(method&& src)noexcept
    {
        parent_class::operator=(std::move(src));
        return *this;
    }

};


template<typename T, typename R, typename _1,typename _2, typename _3>
struct   method<T,R,_1,_2,_3,_nil_,_nil_,_nil_,_nil_> : public function3<T,R,_1,_2,_3>
{
    using parent_class = function3<T,R,_1,_2,_3>;
    using type = typename parent_class :: fntype_t;
    method()= default;
    method(const method& src): parent_class (src){}
    method(method&& src)noexcept: parent_class (std::move(src)) {}
    method(const std::string& a_id,T* a_rtobj, type a_method): parent_class(a_id,a_rtobj, a_method){}
    method& operator=(const method& src)
    {
        parent_class::operator=(src);
        return *this;
    }

    method& operator=(method&& src)noexcept
    {
        parent_class::operator=(std::move(src));
        return *this;
    }

};

template<typename T, typename R, typename _1,typename _2, typename _3, typename _4>
struct   method<T,R,_1,_2,_3,_4,_nil_,_nil_,_nil_> : public function4<T,R,_1,_2,_3,_4>
{
    using parent_class = function4<T,R,_1,_2,_3,_4>;
    using type = typename parent_class :: fntype_t;
    method()= default;
    method(const method& src): parent_class (src){}
    method(method&& src)noexcept: parent_class (std::move(src)) {}
    method(const std::string& a_id,T* a_rtobj, type a_method): parent_class(a_id,a_rtobj, a_method){}
    method& operator=(const method& src)
    {
        parent_class::operator=(src);
        return *this;
    }

    method& operator=(method&& src)noexcept
    {
        parent_class::operator=(std::move(src));
        return *this;
    }

};

template<typename T, typename R, typename _1,typename _2, typename _3, typename _4, typename _5>
struct   method<T,R,_1,_2,_3,_4,_5,_nil_,_nil_> : public function5<T,R,_1,_2,_3,_4,_5>
{
    using parent_class = function5<T,R,_1,_2,_3,_4,_5>;
    using type = typename parent_class :: fntype_t;
    method()= default;
    method(const method& src): parent_class (src){}
    method(method&& src)noexcept: parent_class (std::move(src)) {}
    method(const std::string& a_id,T* a_rtobj, type a_method): parent_class(a_id,a_rtobj, a_method){}
    method& operator=(const method& src)
    {
        parent_class::operator=(src);
        return *this;
    }

    method& operator=(method&& src)noexcept
    {
        parent_class::operator=(std::move(src));
        return *this;
    }

};

template<typename T, typename R, typename _1,typename _2, typename _3, typename _4, typename _5, typename _6>
struct   method<T,R,_1,_2,_3,_4,_5,_6,_nil_> : public function6<T,R,_1,_2,_3,_4,_5,_6>
{
    using parent_class = function6<T,R,_1,_2,_3,_4,_5,_6>;
    using type = typename parent_class :: fntype_t;
    method()= default;
    method(const method& src): parent_class (src){}
    method(method&& src)noexcept: parent_class (std::move(src)) {}
    method(const std::string& a_id,T* a_rtobj, type a_method): parent_class(a_id,a_rtobj, a_method){}
    method& operator=(const method& src)
    {
        parent_class::operator=(src);
        return *this;
    }

    method& operator=(method&& src)noexcept
    {
        parent_class::operator=(std::move(src));
        return *this;
    }

};

template<typename T, typename R, typename _1,typename _2, typename _3, typename _4, typename _5, typename _6, typename _7>
struct   method<T,R,_1,_2,_3,_4,_5,_6,_7> : public function7<T,R,_1,_2,_3,_4,_5,_6,_7>
{
    using parent_class = function7<T,R,_1,_2,_3,_4,_5,_6,_7>;
    using type = typename parent_class :: fntype_t;
    method()= default;
    method(const method& src): parent_class (src){}
    method(method&& src): parent_class (std::move(src)) {}
    method(const std::string& a_id,T* a_rtobj, type a_method): parent_class(a_id,a_rtobj, a_method){}
    method& operator=(const method& src)
    {
        parent_class::operator=(src);
        return *this;
    }

    method& operator=(method&& src)noexcept
    {
        parent_class::operator=(std::move(src));
        return *this;
    }

};




//#pragma GCC diagnostic pop //  -Wpmf-conversions (void*<->object)
}

//#endif