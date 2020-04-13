#pragma once

#include <xio/utils/xio.utils>
#include <utility>

namespace teacc::utils
{


class c_iterator
{
    c_iterator* _lhs = nullptr;
    c_iterator* _rhs = nullptr;
    c_iterator* _op  = nullptr;
    void*       _data= nullptr;

public:

    c_iterator() = default;
    c_iterator(c_iterator* a_op) : _op(a_op) {}

    c_iterator(const c_iterator& ci)
    {
        _lhs  = ci._lhs;
        _rhs  = ci._rhs;
        _op   = ci._op;
        _data = ci._data;
    }

    c_iterator(c_iterator&& ci) noexcept
    {
        using std::swap;
        std::swap(_rhs, ci._rhs);
        std::swap(_op , ci._op);
        std::swap(_data , ci._data);
    }
    
    c_iterator(void* d){_data = d;}

    c_iterator& operator=(c_iterator&& rvalue) noexcept
    {
        if (&rvalue == this) return *this;
        using std::swap;
        std::swap(_rhs, rvalue._rhs);
        std::swap(_op, rvalue._op);
        std::swap(_data, rvalue._data);
        return *this;
    }
    c_iterator& operator=(const c_iterator& ci)
    {
        _lhs = ci._lhs;
        _rhs = ci._rhs;
        _op = ci._op;
        _data = ci._data;
        return *this;
    }

    operator bool() {return _data != nullptr;}

    template<typename T> T*& operator()() { return _data; }
    bool operator !=(const c_iterator& ci) { return _data != ci._data; }
    bool operator ==(const c_iterator& ci) { return _data == ci._data; }

    c_iterator operator++()
    {
        if (_rhs)
            return *_rhs;

        return c_iterator();
    }

    c_iterator operator++(int)
    {
        if (_rhs)
            return *_rhs;
        return c_iterator();
    }

    c_iterator operator--()
    {
        if (_lhs)
            return *_lhs;

        return c_iterator();
    }

    c_iterator operator--(int)
    {
        if (_lhs)
            return *_lhs;
        return c_iterator();
    }
    void set_parent(c_iterator* _p) { _op = _p;  }
    virtual ~c_iterator() = default;
};




class collection_t : public c_iterator
{

    c_iterator* _begin = nullptr;
    c_iterator* _c = nullptr;
    c_iterator* _end = nullptr;
public:

    collection_t() = default;
    collection_t(c_iterator* _parent_collection) { set_parent (_parent_collection); }

    template<typename ci_T> collection_t& operator << (ci_T* _data)
    {
        auto i = new c_iterator<ci_T>(this);
        if (!_begin)
        {
            _begin = i;
            _c = _end = _begin;
        }   
        else
        {
            i->_lhs = _end;
            _end = _c = _end->_rhs = i;
        }

        i() = _data;
        return *this;
    }
    
};

}
