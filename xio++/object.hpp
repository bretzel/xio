#pragma once
#include <xio++/xio++.hpp>
#include <vector>


namespace xio {

//class xio_api xio {
//
//};

class xio_api object
{

    object* m_parent = nullptr;

public:

    using list = std::vector<object*>;
    using iterator = list::iterator;
protected:
    object::list m_children;

public:
    object();
    object(object* a_parent);
    virtual ~object();


    bool append_child(object* a_child);
    bool has_child(object* a_child);
    bool remove_child(object* a_child);
    bool detach();

    template<class T> T* type() const { return dynamic_cast<T*>(this); }
    template<class T> T* parent() const { return dynamic_cast<T*>(m_parent); }

    object::iterator at(object* a_child);
    object::iterator begin();
    object::iterator end();
};



}



