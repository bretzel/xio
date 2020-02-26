//
// Created by bretzel on 20-01-27.
//

//#ifndef PLSC_RETURNT_H
//#define PLSC_RETURNT_H

#pragma once

#include <xio/utils/notification.h>

//#include <memory>
namespace xio::utils
{


template< class T >class xreturn
{
    bool f = false;
    union
    {
        T *v = nullptr;
        notification *m;
    } u;

public:
    xreturn()
    {
        //f = false;
        u.m = &notification::Null();
    }
    
    xreturn(notification &a_msg)
    {
        u.m = &a_msg; // new notification();
        // *u.m = a_msg;
        f = false;
        // std::cout << __PRETTY_FUNCTION__ << ":\n";
    }
    
    xreturn(const T &dt)
    {
        u.v = new T();
        *u.v = dt;
        f = true;
    }
    
    xreturn(xreturn &&n) noexcept
    {
        using std::swap;
        swap(u, n.u);
        swap(f, n.f);
    }
    
    xreturn(const xreturn & n)
    {
        f = n.f;
        if(f)
        {
            u.v = new T();
            *u.v = *n.u.v;
        }
        else
            u.m = n.u.m;
    }
    
    xreturn &operator=(notification &a_msg)
    {
        if(f) {
            if(u.v) {
                delete u.v;
                u.v = nullptr;
            }
        }
        f = false;
        u.m = &a_msg;
        return *this;
    }
    
    xreturn &operator=(xreturn &&n) noexcept
    {
        using std::swap;
        swap(u, n.u);
        swap(f, n.f);
        return *this;
    }
    
    xreturn &operator=(const xreturn &n)
    {
        if(&n == this)
            return *this;
        u = n.u;
        f = n.f;
        return *this;
    }
    
    xreturn &operator=(const T &a_val)
    {
        if(!u.v)
            u.v = new T();
        
        *u.v = a_val;
        f = true;
        return *this;
    }
    
    operator bool() const { return f; }
    
    notification &note()
    {
        if(f)
            return notification::Null();
        return *u.m;
    }
    
    T value()
    {
        if(!f)
            return T();
        return *u.v;
    }
    
    T operator()()
    {
        if(!f)
            return T();
        return *u.v;
    }
    
    void clear()
    {
        if(f) {
            if(u.v)
            {
                delete u.v;
                //std::cout << __PRETTY_FUNCTION__ << ":> deleted\n";
                u.v = nullptr;
            }
        }
        u.m = nullptr;
        f = false;
    }
    
    ~xreturn()
    {
        clear();
    }
    
    /*!
        @brief  to be verified.
    */
    std::string text(std::function<std::string(const T&)> textify_ = nullptr)
    {
        if(*this){
            xstr str;
            std::string rr;
            T v = *u.v;
            if (textify_)
                return textify_(v);
            else {
                str << __PRETTY_FUNCTION__;
                rr = str.extract_substr("T =", ";");
                str = " no method given, for textifying this xreturn<T>:[";
                str  << rr << ']';
            }
            return str();
        }
        return (*u.m)();
    }
};


}

//#endif //PLSC_RETURNT_H
