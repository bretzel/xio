
#pragma once


#include <any>
#include <xio/interpreter/interpreterdll.h>
#include <xio/utils/expect>
#include <xio/lexer/token_t.h>
#include <memory>

namespace teacc{

/*!
    @brief Arithmetic Logical Unit.
*/
class  INTERPRETERAPI alu
{
    friend class xio;
    static std::string __nil;
    lexer::type::T _type = lexer::type::null;
    std::any a;

public:

    using collection = std::vector<alu>;
    using shared     = std::shared_ptr<alu>;
    enum conv {
        DEG,
        RAD
    };

    alu() { a.reset(); }

    alu(const alu& lf) { a = lf.a; _type = lf._type; }
    alu(double lf);
    alu(float lf);
    //alu(token_t* rhs);

    alu(const std::string& lf);
    alu(const char* lf);
    alu(void* lf);
    alu(alu&& lf) noexcept;
    alu(int8_t lf);
    alu(int16_t lf);
    alu(int32_t lf);
    alu(int64_t lf);
    alu(uint8_t lf);
    alu(uint16_t lf);
    alu(uint32_t lf);
    alu(uint64_t lf);
    alu(bool lf);

    void reset();
    alu& operator =(const alu& lf);// { a = lf.a; _type = lf._type; return *this; }

    alu& operator =(alu&& rhs) noexcept;

    alu operator==(const alu & lf);
    lexer::type::T type() { return _type; }

    template<typename T> T value() const {

        if constexpr (std::is_same<T, std::string>::value)
            return (_type& lexer::type::text ? std::any_cast<std::string>(a) : alu::__nil);
        else
        if constexpr (std::is_same<T, const std::string&>::value)
            return (_type& lexer::type::text ? std::any_cast<const std::string&>(a) : alu::__nil);
        else
        if constexpr (std::is_same<T, const char*>::value)
            return (_type& lexer::type::text ? std::any_cast<std::string>(a).c_str() : alu::__nil.c_str());
        else
        if constexpr (std::is_same<T, void*>::value)
            return std::any_cast<void*>(a);
        else
            return static_cast<T>(
                _type & lexer::type::i8 ?  std::any_cast<int8_t>(a) :
                _type & lexer::type::i16 ? std::any_cast<int16_t>(a) :
                _type & lexer::type::i32 ? std::any_cast<int32_t>(a) :
                _type & lexer::type::i64 ? std::any_cast<int64_t>(a) :
                _type & lexer::type::u8 ?  std::any_cast<uint8_t>(a) :
                _type & lexer::type::u16 ? std::any_cast<uint16_t>(a) :
                _type & lexer::type::u32 ? std::any_cast<uint32_t>(a) :
                _type & lexer::type::u64 ? std::any_cast<uint64_t>(a) :
                _type & lexer::type::boolean ? std::any_cast<bool>(a) :
                _type & lexer::type::real ? std::any_cast<double>(a) :0

            );
    }

    template<typename T> T number() const
    {
        if constexpr (std::is_same<T, void*>::value)
            return std::any_cast<void*>(a);
        else
        if constexpr (std::is_same<T, std::string>::value)
            return (std::string)*this;
        else
            return static_cast<T>(
                _type & lexer::type::i8 ?  std::any_cast<int8_t>  (a) :
                _type & lexer::type::i16 ? std::any_cast<int16_t> (a) :
                _type & lexer::type::i32 ? std::any_cast<int32_t> (a) :
                _type & lexer::type::i64 ? std::any_cast<int64_t> (a) :
                _type & lexer::type::u8 ?  std::any_cast<uint8_t> (a) :
                _type & lexer::type::u16 ? std::any_cast<uint16_t>(a) :
                _type & lexer::type::u32 ? std::any_cast<uint32_t>(a) :
                _type & lexer::type::u64 ? std::any_cast<uint64_t>(a) :
                _type & lexer::type::boolean ? std::any_cast<bool>(a) :
                _type & lexer::type::real ? std::any_cast<double>(a) : 0
            );

    }


    template<lexer::type::T T> bool is() {
        return _type == T;
    }

    bool text_t() { return _type&lexer::type::text;}
    std::string to_s();
    bool number_t();
    std::string types() const;
    virtual ~alu();



    // Arithmetic operations overload between two alus:

    // Binary operators:

    alu operator +(const alu& rv);
    alu operator -(const alu& rv);
    alu operator *(const alu& rv);
    alu operator [](alu::conv a_conv);

    alu operator /(const alu& rv);
    alu operator %(const alu& rv);
    alu operator ^(const alu& rv);
    alu operator <<(const alu& lf);
    alu operator >>(const alu& lf);
    alu operator |(const alu& lf);
    alu operator &(const alu& lf);
    alu operator ~();
    // Assign operators:
    alu& operator +=(const alu& rv);
    alu& operator -=(const alu& rv);
    alu& operator *=(const alu& rv);
    alu& operator /=(const alu& rv);
    alu& operator %=(const alu& rv);
    alu& operator |=(const alu& rv);
    alu& operator &=(const alu& rv);
    alu& operator ^=(const alu& rv);

    alu& operator <<=(const alu& rv);
    alu& operator >>=(const alu& rv);


    // boolean operators:
    alu operator >(const alu& rv);
    alu operator <(const alu& rv);
    alu operator >=(const alu& rv);
    alu operator <=(const alu& rv);
    alu operator !=(const alu& rv);
    alu operator ||(const alu& rv);
    alu operator &&(const alu& rv);

    bool operator !();
    bool is_set() { return a.has_value(); }
    operator bool();

    alu operator -();
    alu operator +();

    alu operator ++(int);
    alu operator ++();
    alu operator --();
    alu operator --(int);

    alu radical(const alu& lf);
    alu factorial(const alu& lf);

    bool type_size(const alu& rv);
    std::string operator()()const { return (std::string)(*this); }
    operator std::string() const {
        utils::xstr str;
        if (_type &lexer::type::boolean)
            str << (value<bool>() ? "true" : "false");
        else
        if (_type&lexer::type::number)
            str << number<double>();
        else
        if (_type&lexer::type::text)
            return value<std::string>();
        else
        if (_type&lexer::type::voidptr) {
            str = "@[%ld]";
            str << number<void*>();
        }
        else
        if (_type&lexer::type::null)
            str << "null";
        return str();
    }

    private:
    alu remove_substr_copy(const std::string& to_erase);
    alu completely_remove_substr_copy(const std::string& to_erase);
    alu& remove_substr(const std::string& to_erase);
    alu& completely_remove_substr(const std::string& to_erase);

};


}
