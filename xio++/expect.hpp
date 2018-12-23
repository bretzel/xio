#pragma once

#include <xio++/message.hpp>
//#include <variant>




namespace xio{
    
    

template<typename T> class expect{
    
    bool f = false;
    union{
        T*          v=nullptr;
        message*    m;
    }u;
    
public:
    expect() {
        f=false;
        u.m = &message::null();
    }
    
    
    expect(message& a_msg) {
        u.m = &a_msg; // new message();
        // *u.m = a_msg;
        f = false;
        std::cout << __PRETTY_FUNCTION__ << ":\n";
    }

    expect(const T& dt){
        u.v = new T();
        *u.v = dt;
        f = true;
    }
    
    
    expect(expect&& n)
    {
        using std::swap;
        swap(u, n.u);
        swap(f, n.f);
    }
    
    
    expect& operator = (message& a_msg)
    {
        if(f){
            if(u.v){
                delete u.v;
                u.v = nullptr;
            }
        }
  
        f = false;
        u.m = &a_msg;
        return *this;
    }
    
    expect& operator = (expect&& n)
    {
        using std::swap;
        swap(u, n.u);
        swap(f, n.f);
        return *this;
    }
    
    expect& operator = (const expect& n)
    {
        if(&n == this) return *this;
        u = n.u;
        f = n.f;
        return *this;
    }
    
    expect& operator = (const T& a_val)
    {
        if(!u.v)        
            u.v = new T();
        
        *u.v = a_val;
        f = true;
        return *this;
    }

    operator bool() { return f;}
    
    message& notice() { 
        if(f) 
            return message::null();
        return *u.m;
    }
    
    T value() {
        if(!f) return T();
        return *u.v;
    }
    
    void clear()
    {
        if(f){
            if(u.v)
                delete u.v;
        }
        //else{
        //    if(u.m){
        //        delete u.m;
        //    }
        //}
        u.m = nullptr;
    }
    
    ~expect(){

    }
}; 
   

}
