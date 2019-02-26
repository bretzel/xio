#include "message.hpp"
#include <map>

namespace xio {

message::stack message::sstack;


std::map<message::code, std::string> codes = {
    {message::code::accepted,   "accepted"    },
    {message::code::rejected,   "rejected"    },
    {message::code::unexpected, "unexpected"  },
    {message::code::empty,      "empty"       },
    {message::code::invalid,    "invalid"     },
    {message::code::illegal,    "illegal"     },
    {message::code::eof,        "eof"         },
    {message::code::null,       "null"        },
    {message::code::unmatch,    "unmatch"     },
    {message::code::syntax,     "syntax"      }
};


message message::mmessage = { message::xclass::internal };

message::message()
{
}


message::~message()
{
    mtext.clear();
}

message::message(const message & a_msg)
{
    mclass = a_msg.mclass;
    mtext = a_msg.mtext;

}

message::message(message && a_msg)
{
    std::swap(mclass, a_msg.mclass);
    std::swap(mtext, a_msg.mtext);

}

message & message::operator=(const message & a_msg)
{
    if (&a_msg == this) return *this;
    mclass = a_msg.mclass;
    mtext = a_msg.mtext;

    return *this;
}

message & message::operator=(message && a_msg)
{
    std::swap(mclass, a_msg.mclass);
    std::swap(mtext, a_msg.mtext);
    return *this;
}

message & message::operator,(message::code cc)
{
    
    mtext << ' ' << message::name(cc) << ' ';
    return *this;
    // TODO: insérer une instruction return ici
}

message & xio::message::push(message::xclass cls)
{
    
    message::sstack.push(new message(cls));
    return *message::sstack.top();
}

std::string message::name(message::code cc)
{
    return codes[cc];
    
}

bool xio::message::pop(message & m)
{
    if (message::sstack.empty()) return false;       
        
    m = *message::sstack.top();
    message::sstack.pop();
    return true;
}


int xio::message::clear(std::function<void(message&)> p_fn)
{
    std::size_t sz = message::sstack.size();
    while(!message::sstack.empty()){
        message* m = message::sstack.top();
        message::sstack.pop();
        if(p_fn) p_fn(*m);
        delete m;
    }
    return sz;
}



}
