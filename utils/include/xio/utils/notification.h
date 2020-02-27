//
// Created by bretzel on 20-01-27.
//


#pragma once
#include <xio/utils/xstr.h>
#include <stack>
#include <functional>


namespace xio::utils
{

struct XIOUTILS_API notification final
{
    enum class type : uint8_t {
        unset = 0,
        status,
        error,
        notice,
        warning,
        fatal,
        exception,
        information,
        internal,
        debug,
        success,
        fail
    };
    
    enum class code : uint16_t {
        unset = 0,
        ok,
        accepted,
        rejected,
        unexpected,
        empty,
        invalid,
        illegal,
        eof,
        null,
        unmatched,
        syntax,
        exists,
        implement,
        unterminated_litteral,
        notfound
    };


    using stack = std::stack<notification>;
    static notification::stack _stack;
    
    notification::type _type = notification::type::unset;
    notification::code _code = notification::code::unset;
    xstr _text;
    
    std::vector<std::string> _components;



    
    
    notification() = default;
    
    notification(const notification& A);
    notification(notification&& A) noexcept;
    
    ~notification();
    static notification& Null();
    notification& operator = (const notification& A);
    notification& operator = (notification&& A)  noexcept;
    
    static void clear(std::function<void(notification&)> LambdaFn);
    static notification& push(notification::type aType = notification::type::notice);
    std::string operator()();
    template<class T> notification& operator ,(const T& aValue)
    {
        xstr Str;
        Str << aValue;
        _components.push_back(Str());
        return *this;
    }
    
    notification& operator ,(notification::code C) { _code = C;  _components.push_back(notification::to_s(C)); return *this; }
    notification& operator ,(notification::type T) { _type = T;  _components.push_back(notification::to_s(T)); return *this; }
    
    static std::string to_s(notification::code C);
    static std::string to_s(notification::type T);
};


}

