#include "object.hpp"


namespace xio {

object::object()
{
}

object::object(object * a_parent)
{
    if (a_parent)
        a_parent->append_child(this);

}


object::~object()
{
    if (!m_children.empty()) m_children.clear();
}


bool object::append_child(object * a_child)
{
    if (has_child(a_child))
        return false;
    m_children.emplace_back(a_child);
    a_child->m_parent = this;
    return true;
}



bool object::has_child(object * a_child)
{
    if (at(a_child) != end()) return true;
    return false;
}

bool object::remove_child(object * a_child)
{
    iterator i = at(a_child);
    if (i == end()) return false;
    m_children.erase(i);
    return true;
}

bool object::detach()
{
    if (m_parent)
        m_parent->remove_child(this);
    return m_parent != nullptr;
}

bool object::end(object::iterator it)
{
    if ((*it) && (*it)->m_parent != this) return true;
    
    return it == m_children.end();
}


object::iterator object::at(object * a_child)
{
    if (m_children.empty()) return m_children.end();
    for (iterator i = begin(); i != end(); i++)
        if (*i == a_child) return i;
    return end();
}

object::iterator object::begin()
{
    return m_children.begin();
}

object::iterator object::end()
{
    return m_children.end();
}



}
