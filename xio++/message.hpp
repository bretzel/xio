#pragma once
#include <xio++/xio++.hpp>
#include <xio++/string_t.hpp>
#include <functional>
#include <thread>
#include <mutex>
#include <stack>

namespace xio {


class xio_api message
{

public:
    enum class xclass: uint8_t{
        status = 1,
        error,
        notice,
        warning,
        fatal,
        exception,
        information,
        internal
    };

    enum class xcode : uint16_t {
        accepted,
        rejected,
        unexpected,
        empty,
        invalid,
        illegal,
        eof,
        null,
        unmatch,
        syntax
    };


private:
    
    using stack = std::stack<message*>;
    static message::stack sstack;

    message::xclass     mclass = message::xclass::status;
    message::xcode      mcode = message::xcode::null;
    string_t            mtext;
    static message      mmessage;
    static std::mutex   mstack_mtx;
    
    
public:
    
    using callback_t = std::function<std::string(message&)>;

    message();
    ~message();
    message(message::xclass cls) { mclass = cls; }
    message(message::xclass cls, const std::string& txt) {
        mtext   = txt;
        mclass  = cls;
    }
    message(const message& a_msg);
    message(message&& a_msg);

    message& operator=(const message& a_msg); 
    message& operator=(message&& a_msg);

    std::string operator ()() { return mtext(); }

    message::xcode code() { return mcode; }

    //message& operator, (message::callback_t p_fn);
    template<typename T> message& operator ,(const T& t) {
        mtext << t;
        return *this;
    }

    static message& push(message::xclass cls = message::xclass::status);
    static bool pop(message&);
    static int clear(std::function<void(message&)> p_fn);
    static message& null() { return message::mmessage; }

};

}

