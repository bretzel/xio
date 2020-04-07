//
// Created by bretzel on 20-01-27.
//

#pragma once

#include <xio/utils/notification.h>

#include <memory>
#include <any>

namespace teacc::utils
{


template< class T >class expect
{
    bool f = false;
    std::any _a;

public:
    expect() = default;

    expect(notification &a_msg)
    {
        _a = a_msg; // new notification();
        f = false;
    }
    
    expect(const T &dt)
    {
        _a = dt;
        f = true;
    }
    
    expect(expect &&n) noexcept
    {
        _a = std::move(n._a);
        f = n.f;
    }
    
    expect(const expect & n)
    {
        f = n.f;
        _a = n._a;
    }
    
    expect &operator=(notification &a_msg)
    {
        if(f) {
            _a.reset();
        }
        f = false;
        _a = a_msg;
        return *this;
    }
    
    expect &operator=(expect &&n) noexcept
    {
        _a = std::move(_a);
        f = std::move(n.f);

        return *this;
    }
    
    expect &operator=(const expect &n)
    {
        if(&n == this)
            return *this;
        _a.reset();
        _a = n._a;
        f = n.f;
        return *this;
    }
    
    expect &operator=(const T &a_val)
    {
        if (f)
            _a.reset();
        _a = a_val;
        f = true;
        return *this;
    }
    
    operator bool() const { return f; }
    
    notification &note()
    {
        if(f)
            return notification::Null();
        return std::any_cast<notification&>(_a);
    }
    
    auto& value()
    {
        if (!f)
        {
            notification::push(), notification::type::error, ": ", __PRETTY_FUNCTION__, " - Expected value was not returned.";
            _a = T();
        } 
        return std::any_cast<T&>(_a);
    }
    
    auto& operator()()
    {
        if (!f)
        {
            _a = notification::push(), "expect: expected value on a false state.";
            return std::any_cast<notification&>(_a);
        }
        return std::any_cast<T&>(_a);
    }
    
    void reset()
    {
        _a.reset();
        f = false;
    }
    
    ~expect()
    {
        reset();
    }    
    /*!
        @brief  to be verified.
    */
    std::string text(std::function<std::string(const T&)> textify_ = nullptr)
    {
        if(*this){
            xstr str;
            std::string rr;
            T& v = value();
            if (textify_)
                return textify_(v);
            else {
                str << __PRETTY_FUNCTION__;
                rr = str.extract_substr("T =", ";");
                str = " no method given, for textifying this expect<T>:[";
                str  << rr << ']';
            }
            return str();
        }
        return std::any_cast<notification&>(_a)();
    }
};

using result_code = expect<notification::code>;


}

//#endif //PLSC_RETURNT_H
