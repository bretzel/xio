//
// Created by slussier on 19-08-23.
//

#include <xio/interpreter/alu.h>
#include <cmath>
#include <xio/utils/expect.h>
#include <xio/utils/journal.h>

#include <any>

namespace teacc {


std::string INTERPRETERAPI alu::__nil="";

#define _ctor_u(rhs) { _type |= lexer::type::number|lexer::type::leaf; a = (rhs);}
#define _ctor_s(rhs) { _type |= lexer::type::number|lexer::type::sign|lexer::type::leaf; a = (rhs);}
#define _assign_u _type |= lexer::type::number|lexer::type::leaf; a.reset(); a = (rhs);
#define _assign_s _type |= lexer::type::number|lexer::type::sign|lexer::type::leaf; a.reset(); a = (rhs);

alu::alu(int8_t rhs)  : _type(lexer::type::i8)    _ctor_s(rhs)
alu::alu(int16_t rhs) : _type(lexer::type::i16)   _ctor_s(rhs)
alu::alu(int32_t rhs) : _type(lexer::type::i32)   _ctor_s(rhs)
alu::alu(int64_t rhs) : _type(lexer::type::i64)   _ctor_s(rhs)
alu::alu(uint8_t rhs) : _type(lexer::type::u8)    _ctor_u(rhs)
alu::alu(uint16_t rhs): _type(lexer::type::u16)   _ctor_u(rhs)
alu::alu(uint32_t rhs): _type(lexer::type::u32)   _ctor_u(rhs)
alu::alu(uint64_t rhs): _type(lexer::type::u64)   _ctor_u(rhs)
alu::alu(bool rhs)    : _type(lexer::type::boolean)
_ctor_s(rhs)


#define cast(rhs)\
(rhs)._type & lexer::type::boolean  ? std::any_cast<uint64_t>((rhs).a) != false :\
(rhs)._type & lexer::type::i8    ? std::any_cast<int8_t>  ((rhs).a):\
(rhs)._type & lexer::type::i16   ? std::any_cast<int16_t> ((rhs).a):\
(rhs)._type & lexer::type::i32   ? std::any_cast<int32_t> ((rhs).a):\
(rhs)._type & lexer::type::i64   ? std::any_cast<int64_t> ((rhs).a):\
(rhs)._type & lexer::type::u8    ? std::any_cast<uint8_t> ((rhs).a):\
(rhs)._type & lexer::type::u16   ? std::any_cast<uint16_t>((rhs).a):\
(rhs)._type & lexer::type::u32   ? std::any_cast<uint32_t>((rhs).a):\
(rhs)._type & lexer::type::u64   ? std::any_cast<uint64_t>((rhs).a):\
(rhs)._type & lexer::type::real  ? std::any_cast<double>((rhs).a) :0




alu::alu(double rhs) {
    _type = lexer::type::number | lexer::type::real | lexer::type::leaf; a = rhs;
    //    std::cout << __PRETTY_FUNCTION__ << ":" << types() << ": " << sizeof(a) << '\n';
}

alu::alu(float rhs) {
    _type = lexer::type::number | lexer::type::real | lexer::type::leaf; a = (double)rhs;
    //    std::cout << __PRETTY_FUNCTION__ << ":" << types() << ": " << sizeof(a) << '\n';
}

alu::alu(const std::string & rhs) {
    _type = lexer::type::text | lexer::type::leaf;
    a = rhs;
    //std::cout << __PRETTY_FUNCTION__ << ":" << types() << ": " << sizeof(a) << '\n';
}

alu::alu(const char* rhs) {
    _type = lexer::type::text | lexer::type::leaf;
    a = std::string(rhs);
    //std::cout << __PRETTY_FUNCTION__ << ":" << types() << ": " << sizeof(a) << '\n';
}

alu::alu(void * rhs) {
    _type = lexer::type::any | lexer::type::voidptr;
    a = rhs;
}

alu::alu(alu && rhs) noexcept
{
    std::swap(_type, rhs._type);
    rhs._type = lexer::type::null;
    std::swap(a, rhs.a);
}




void alu::reset()
{
    a.reset();
    _type = lexer::type::null;
}


bool alu::type_size(const alu & rv)
{
    using namespace lexer::type;
    lexer::type::T me = _type& (i8 | i16 | i32 | i64| u8 | u16 | u32 | u64| real);
    lexer::type::T r = rv._type&(i8 | i16 | i32 | i64 | u8 | u16 | u32 | u64 | real);
    return me - r >= 0;
}

alu::operator bool()
{
    if(!a.has_value()) return false;
    if (!_type) return false;
    if (_type&lexer::type::text)
        return !value<std::string>().empty();
    if (_type&lexer::type::number)
        return number<double>() != 0.0f;
    if (_type&lexer::type::voidptr)
        return value<void*>() != nullptr;
    return false;
}


#define lr_number(rv) if ((_type&lexer::type::number) && ((rv)._type&lexer::type::number))
#define lr_text(rv)   if ((_type&lexer::type::text) && ((rv)._type&lexer::type::text))
#define lr_any(rv)    if ((_type&lexer::type::any)  && ((rv)._type&lexer::type::any))
#define is_integrale(rv) if (!((rv)._type&lexer::type::real))
#define is_text(i)    if((i)._type & lexer::type::text)
#define is_number if (!(_type&(lexer::type::real|lexer::type::number)))
#define delstring_if(rhs) if ((_type&lexer::type::text) && (!(rhs)._type & lexer::type::text)) delete a.t;





alu & alu::operator=(const alu & rhs)
{
    if(&rhs != this) {
        _type = rhs._type;
        a.reset();
        a = rhs.a;
    }
    return *this;
}

alu & alu::operator=(alu&& rhs) noexcept
{
    //logdebugpfn << logger::HBlue << " RHS: " << rhs() << logger::Reset << Ends;
    std::swap(_type, rhs._type);
    std::swap(a,rhs.a);
    return *this;
}


alu alu::operator==(const alu & rhs)
{

    if (_type & lexer::type::number)
        return alu((bool)(( (cast(*this)) == (cast(rhs)))));
    if (_type & lexer::type::text)
        return alu( std::any_cast<std::string>(a) == std::any_cast<std::string>(rhs.a) );

    if (_type & lexer::type::any)
        return alu( std::any_cast<void*>(a) == std::any_cast<void*>(rhs.a) );
    return alu(false);
}



std::string alu::types() const
{
    return lexer::type::to_s(_type);
}

alu alu::operator +(const alu& rv)
{
    lr_text(rv)
        return alu(value<std::string>() + rv.value<std::string>());
    else
    lr_number(rv)
        return alu(number<double>() + rv.number<double>());

    utils::notification::push(utils::notification::type::warning), "warning:[", lexer::type::to_s(_type), "] and [", lexer::type::to_s(rv._type), "] are incompatible.";
    return alu(false);

}

alu alu::operator[](alu::conv c)
{
    is_text(*this) { return alu(0.0); }
    double d = number<double>();
    alu a;
    switch (c) {
        case alu::DEG:
            a = d * 180 / 3.141592653589793;
            break;
        case alu::RAD:
            a = d*3.141592653589793 / 180;
            break;
        default:
            a = 0.0;
    }

    return a;
}



alu alu::operator -(const alu& rv)
{

    lr_text(rv){
        return remove_substr_copy(rv.value<std::string>());
    }

    lr_number(rv)
        return alu(number<double>() - rv.number<double>());
    lr_any(rv)
        return alu(reinterpret_cast<uint64_t>(value<void*>()) - reinterpret_cast<uint64_t>(rv.value<void*>()));

    utils::notification::push(utils::notification::type::warning), "cannot apply sign on ", lexer::type::to_s(_type);
    return alu(false);
}

alu alu::operator *(const alu& rv)
{
    alu f;
    lr_text(rv){
        std::size_t pos = std::any_cast<std::string>(rv.a).find(std::any_cast<std::string>(a));
        return alu(pos != std::string::npos);
    }

    //else
    lr_number(rv)
        return alu(number<double>() * rv.number<double>());

    utils::notification::push(utils::notification::type::warning), "cannot multiply", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return alu();
}


alu alu::operator /(const alu& rv)
{
    lr_text(rv){
        utils::notification::push(utils::notification::type::warning), "cannot divide strings";
        return alu();
    }
    //    f = value<std::string>() + rv.value<std::string>();
    //else
    lr_number(rv) {
        double vr = rv.number<double>();
        if (!vr) {
            logfatalpfn << " :" << (
                utils::notification::push(),
                " in alu arithmetic operation: division by zero error. (",
                (*this)(), " / ", rv(), ")" 
                )() 
            << ends;
            return alu();
        }
        return alu(number<double>() / rv.number<double>());
    }

    utils::notification::push(utils::notification::type::warning), "cannot divide ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return alu();
}

alu alu::operator %(const alu& rv)
{
    alu f;
    lr_text(rv) {
        utils::notification::push(utils::notification::type::warning), "cannot modulo strings";
        return alu();
    }
    lr_number(rv)
        return alu(number<uint64_t>() % rv.number<uint64_t>());

    utils::notification::push(utils::notification::type::warning), "cannot modulo ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return alu();
}

alu alu::operator ^(const alu& rv)
{
    alu f;
    lr_text(rv) {
        utils::notification::push(utils::notification::type::warning), "cannot xor strings";
        return alu();
    }

    lr_number(rv)
        return alu(number<uint64_t>() ^ rv.number<uint64_t>());

    utils::notification::push(utils::notification::type::warning), "cannot xor ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return alu();
}


alu alu::operator <<(const alu& rhs)
{
    alu f;
    lr_text(rhs) {
        utils::xstr str = value<std::string>();
        str << rhs.value<std::string>();
        return alu(str());
    }

    lr_number(rhs)
        return alu(number<uint64_t>() << rhs.number<uint64_t>());

    utils::notification::push(utils::notification::type::warning), "cannot execute bitwise left-shift or insert with ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rhs._type), ".";
    return f;
}
alu alu::operator >>(const alu& rhs)
{
    alu f;
    lr_text(rhs){
        utils::notification::push(utils::notification::type::warning), "cannot execute bitwise right-shift or extract/write/send string on string ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rhs._type), ".";
    }
    lr_number(rhs)
        return alu(number<uint64_t>() >> rhs.number<uint64_t>());

    utils::notification::push(utils::notification::type::warning), "cannot execute bitwise right-shift or extract/write/send with ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rhs._type), ".";
    return f;
}


alu alu::operator |(const alu& rhs)
{

    lr_text(rhs) {
        return alu(value<std::string>() + rhs.value<std::string>());
    }
    lr_number(rhs)
        return alu(number<uint64_t>() | rhs.number<uint64_t>());

    utils::notification::push(utils::notification::type::warning), "cannot execute bitwise or with ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rhs._type), ".";
    return alu();
}

alu alu::operator &(const alu& rhs)
{
    lr_text(rhs){
        utils::notification::push(utils::notification::type::warning), "cannot execute bitwise and on string ";
    }
    lr_number(rhs)
        return alu(number<uint64_t>() & rhs.number<uint64_t>());

    utils::notification::push(utils::notification::type::warning), "cannot execute bitwise and between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rhs._type), ".";
    return alu();
}

alu alu::operator~()
{
    is_text(*this){
        utils::notification::push(utils::notification::type::warning), "cannot execute bitwise invert on string ";
    }

    is_number return alu(~number<uint64_t>());

    utils::notification::push(utils::notification::type::warning), "cannot execute bitwise invert on ", lexer::type::to_s(_type);
    return alu();
}


// Assign operators:
alu& alu::operator +=(const alu& rv)
{
    lr_text(rv) {
        a = std::string(value<std::string>() + rv.value<std::string>());
        return *this;
    }

    lr_number(rv) {
        double r = rv.number<double>();
        double l = number<double>();
        double ans = l + r;
        //logdebug << "alu:: += l[" << this->operator()() << "] + r[" << rv() << "]:" << Ends;

        a = l + r;

        return *this;
    }

    utils::notification::push(utils::notification::type::warning), "cannot execute assign add between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return *this;
}

alu& alu::operator -=(const alu& rv)
{
    lr_text(rv)
        return remove_substr(rv.value<std::string>());

    lr_number(rv) {
        a = number<double>() - rv.number<double>();
        return *this;
    }

    utils::notification::push(utils::notification::type::warning), "cannot execute assign substract between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return *this;
}

alu& alu::operator *=(const alu& rv)
{
    lr_text(rv) {
        utils::notification::push(utils::notification::type::warning), "cannot execute assign multiply on strings [", (*this)(), " *= ", rv() , "].";
        return *this;
    }

    lr_number(rv) {
        a = number<double>() * rv.number<double>();
        return *this;
    }

    utils::notification::push(utils::notification::type::warning), "cannot execute assign multiply between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return *this;
}


alu& alu::operator /=(const alu& rv)
{
    lr_text(rv) {
        utils::notification::push(utils::notification::type::warning), "cannot execute assign divide on strings [", (*this)(), " /= ", rv() , "].";
        return *this;
    }

    lr_number(rv) {
        double vr = rv.number<double>();
        if (!vr) {
            utils::notification::push(utils::notification::type::warning), "alu: cannot divide by zero.";
            a = 0.0;
            _type = lexer::type::null;
            return *this;
        }
        a = number<double>() / rv.number<double>();
        return *this;
    }
    utils::notification::push(utils::notification::type::warning), "cannot execute assign divide between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return *this;
}


alu::~alu()
{
    a.reset();
}

alu& alu::operator %=(const alu& rv)
{
    lr_text(rv)
        return completely_remove_substr(rv.value<std::string>());

    lr_number(rv) {
        a = number<uint64_t>() % rv.number<uint64_t>();
        return *this;
    }

    return *this;
}

alu& alu::operator |=(const alu& rv)
{

    lr_text(rv){
        utils::notification::push(utils::notification::type::warning), "cannot execute assign bitwise or on strings [", (*this)(), " |= ", rv() , "].";
        return *this;
    }

    lr_number(rv) {
        a = number<uint64_t>() | rv.number<uint64_t>();
        return *this;
    }
    utils::notification::push(utils::notification::type::warning), "cannot execute assign bitwise or between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return *this;
}

alu& alu::operator &=(const alu& rv)
{
    lr_text(rv) {
        utils::notification::push(utils::notification::type::warning), "cannot execute assign bitwise and on strings [", (*this)(), " &= ", rv() , "].";
        return *this;
    }
    //    a = std::string(value<std::string>() + rv.value<std::string>());
    //else
    lr_number(rv) {
        a = number<uint64_t>() & rv.number<uint64_t>();
        return *this;
    }
    utils::notification::push(utils::notification::type::warning), "cannot execute assign bitwise and between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return *this;
}


alu& alu::operator ^=(const alu& rv)
{
    lr_text(rv){
        utils::notification::push(utils::notification::type::warning), "cannot execute assign xor on strings [", (*this)(), " ><= ", rv() , "].";
        return *this;
    }

    lr_number(rv) {
        a = number<uint64_t>() ^ rv.number<uint64_t>();
        return *this;
    }

    utils::notification::push(utils::notification::type::warning), "cannot execute assign xor between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return *this;
}


// boolean operators:
alu alu::operator >(const alu& rv)
{
    lr_text(rv)
        return alu(value<std::string>() > rv.value<std::string>());

    lr_number(rv)
        return alu(number<double>() > rv.number<double>());

    utils::notification::push(utils::notification::type::warning), "cannot execute relational operations between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return alu(false);
}

alu alu::operator <(const alu& rv)
{
    lr_text(rv)
        return alu(value<std::string>() < rv.value<std::string>());

    lr_number(rv)
        return alu(number<double>() < rv.number<double>());

    utils::notification::push(utils::notification::type::warning), "cannot execute relational operations between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return alu(false);

}
alu alu::operator >=(const alu& rv)
{
    lr_text(rv)
        return alu(value<std::string>() >= rv.value<std::string>());

    lr_number(rv)
        return alu(number<double>() >= rv.number<double>());

    utils::notification::push(utils::notification::type::warning), "cannot execute relational operations between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return alu(false);

}


alu alu::operator <=(const alu& rv)
{
    lr_text(rv)
        return alu(value<std::string>() <= rv.value<std::string>());

    lr_number(rv)
        return alu(number<double>() <= rv.number<double>());

    utils::notification::push(utils::notification::type::warning), "cannot execute relational operations between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return alu(false);

}


alu alu::operator !=(const alu& rv)
{
    lr_text(rv)
        return alu(value<std::string>() != rv.value<std::string>());

    lr_number(rv)
        return alu(number<double>() != rv.number<double>());

    utils::notification::push(utils::notification::type::warning), "cannot execute relational operations between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return alu(false);
}


alu alu::operator ||(const alu& rv)
{
    lr_text(rv)
        return  alu(!value<std::string>().empty() || (!rv.value<std::string>().empty()));

    lr_number(rv)
        return alu(number<double>() || rv.number<double>());

    utils::notification::push(utils::notification::type::warning), "cannot execute relational operations between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return alu(false);
}

alu alu::operator &&(const alu& rv)
{
    lr_text(rv)
        return  alu(!value<std::string>().empty() && (!rv.value<std::string>().empty()));

    lr_number(rv)
        return alu(number<bool>() && rv.number<bool>());

    utils::notification::push(utils::notification::type::warning), "cannot execute relational operations between ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return alu(false);
}


bool alu::operator !() { return a.has_value(); }//_type == lexer::type::null; }
alu alu::operator -()
{
    is_text(*this){
        utils::notification::push(utils::notification::type::warning), "cannot execute sign operator on string -(\"", (*this)(), "\").";
    }
    utils::notification::push(utils::notification::type::warning), "cannot execute relational operations on ", lexer::type::to_s(_type);
    return -number<double>();
}

alu alu::operator +()
{
    is_text(*this){
        utils::notification::push(utils::notification::type::warning), "cannot execute sign operator on string +(\"", (*this)(), "\").";
    }

    is_number
        if (number<double>() < 0.0f)
            return alu(number<double>() * -1);


    utils::notification::push(utils::notification::type::warning), "cannot execute relational operations on ", lexer::type::to_s(_type);
    return alu(false);
}


alu alu::operator++(int)
{

    is_text(*this){
        utils::notification::push(utils::notification::type::warning), "cannot increment string (\"", (*this)(), "\")++.";
    }

    is_number {
        double f = number<double>();
        alu ff(f);
        ++f;
        a = f;
        return ff;
    }
    utils::notification::push(utils::notification::type::warning), "cannot execute increment operations on ", lexer::type::to_s(_type);
    return alu(false);
}

alu alu::operator++()
{
    is_text(*this){
        utils::notification::push(utils::notification::type::warning), "cannot increment string ++(\"", (*this)(), "\").";
    }

    is_number {
        double f = number<double>();
        ++f;
        a = f;
        return *this;
    }
    utils::notification::push(utils::notification::type::warning), "cannot execute increment operations on ", lexer::type::to_s(_type);
    return alu(false);
}


alu alu::operator--()
{

    is_text(*this){
        utils::notification::push(utils::notification::type::warning), "cannot decrement string --(\"", (*this)(), "\").";
    }

    is_number {
        double f = number<double>();
        ++f;
        a = f;
        return *this;
    }
    utils::notification::push(utils::notification::type::warning), "cannot execute decrement operations on ", lexer::type::to_s(_type);
    return alu(false);
}

alu alu::operator--(int)
{
    is_text(*this){
        utils::notification::push(utils::notification::type::warning), "cannot idecrement string (\"", (*this)(), "\")++.";
    }

    is_number {
        double f = number<double>();
        alu ff(f);
        ++f;
        a = f;
        return ff;
    }
    utils::notification::push(utils::notification::type::warning), "cannot execute decrement operations on ", lexer::type::to_s(_type);
    return alu(false);
}

alu alu::radical(const alu & rhs)
{
    alu al;
    lr_number(rhs) {
        al = alu(std::pow(number<double>(), 1 / rhs.number<double>()));
        return al;
    }

    utils::notification::push(utils::notification::type::warning), "cannot execute radical operator on ", lexer::type::to_s(_type);
    return alu(false);
}


alu alu::factorial(const alu & rhs)
{
    lr_number(rhs) {
        int fac = 1;
        int N = rhs.number<int>();
        if (N >= 10) return alu();
        for (int j = 1; j <= N; j++)
            fac *= j;
        return alu(fac);
    }
    utils::notification::push(utils::notification::type::warning), "cannot execute radical operator on ", lexer::type::to_s(_type);
    return alu(false);
}



alu &alu::operator<<=(const alu &rv) {

    lr_text(rv) {
        utils::notification::push(utils::notification::type::warning), "cannot execute assign extraction/bitwise right-shift / output on strings (\"", (*this)(), "\") >>= [...].";
        return *this;
    }

    lr_number(rv) {
        a = number<uint64_t>() << rv.number<uint64_t>();
        return *this;
    }

    utils::notification::push(utils::notification::type::warning), "cannot execute assign bitwise left-shift or insert with ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return *this;
}

alu &alu::operator>>=(const alu &rv) {

    lr_text(rv) {
        utils::notification::push(utils::notification::type::warning), "cannot execute assign extraction/bitwise right-shift / output on strings (\"", (*this)(), "\") >>= [...].";
        return *this;
    }

    lr_number(rv) {
        a = number<uint64_t>() << rv.number<uint64_t>();
        return *this;
    }

    utils::notification::push(utils::notification::type::warning), "cannot execute assign bitwise right-shift or insert with ", lexer::type::to_s(_type), " and ", lexer::type::to_s(rv._type), ".";
    return *this;
}



alu alu::remove_substr_copy(const std::string &to_erase) {

    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )
    auto cp = std::any_cast<std::string>(a);
    size_t pos = cp.find(to_erase);

    if (pos != std::string::npos)
    {
        // If found then erase it from string

        cp.erase(pos, to_erase.length());
    }
    return alu(cp);
}


alu alu::completely_remove_substr_copy(const std::string &to_erase) {
    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )


    size_t pos = std::string::npos;
    auto cp = std::any_cast<std::string>(a);
    // Search for the substring in string in a loop untill nothing is found
    while ((pos  = cp.find(to_erase) )!= std::string::npos)
    {
        // If found then erase it from string
        cp.erase(pos, to_erase.length());
    }

    return alu(cp);
}



alu& alu::remove_substr(const std::string &to_erase) {

    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )
    auto cp = std::any_cast<std::string&>(a);
    size_t pos = cp.find(to_erase);

    if (pos != std::string::npos)
    {
        // If found then erase it from string

        cp.erase(pos, to_erase.length());
    }
    return *this;
}



bool alu::number_t() { return (_type&(lexer::type::real|lexer::type::number)); }

alu& alu::completely_remove_substr(const std::string &to_erase) {
    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )


    size_t pos = std::string::npos;
    auto cp = std::any_cast<std::string&>(a);
    // Search for the substring in string in a loop untill nothing is found
    while ((pos  = cp.find(to_erase) )!= std::string::npos)
    {
        // If found then erase it from string
        cp.erase(pos, to_erase.length());
    }

    return *this;
}

std::string alu::to_s()
{
    return (std::string)*this;
}


}