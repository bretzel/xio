#pragma once

#include <xio++/xio++.hpp>
#include <string>



#include <cstring>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdlib.h>
#include <time.h>

#include <cmath>
#include <stdint.h>




class xio_api string_t
{
    
    static std::string __nullstr__;

    std::string _s;// = string_t::__nullstr__; ///< Private std::string instance.    

    std::string::size_type _argpos = 0; // Initialize Argument index position...
    static std::string default_separators;
    uint8_t _precision = 8;


    // %[flags][width][.precision][length]specifier
    struct xio_api format_t {
        uint8_t _f = 0; // Flag ( - + . # 0 )
        uint8_t _w = 0; // Width ( length )
        uint8_t _r = 0; // Custom flag set if this format requiers floating point spec.
        uint8_t _p = 6; // Precision (Same as  default).
        uint8_t _l = 0; // Length modifier ( l,ll,h,hh )
        std::size_t _delta = 0; // Format length.
        std::size_t _pos = 0; //
        char    _s = 0; // Effective data type
        //char    _format[20]; // Unused yet.
        const char* _c = nullptr;

        format_t(std::string& a_s) : _c(a_s.c_str()) {}
        std::string operator()();

    };

    //template<class T> struct xio_api input {
    //// %[flags][width][.precision][length]specifier

    //    uint8_t _f = 0; // Flag ( - + . # 0 )
    //    uint8_t _w = 0; // Width ( length )
    //    uint8_t _r = 0; // Custom flag set if this format requiers floating point spec.
    //    uint8_t _p = 6; // Precision (Same as  default).
    //    uint8_t _l = 0; // Length modifier ( l,ll,h,hh )
    //    std::size_t _delta = 0; // Format length.
    //    const T& _arg_t;
    //
    //    input()


    //};


public:

using list = std::vector<std::string>;
using iterator = list::iterator;
using citerator = list::const_iterator;

struct xio_api word {
    std::string::iterator begin;
    std::string::iterator end;
    std::string operator ()();

    using list = std::vector<word>;
    using cursor = list::iterator;
};

struct xio_api cword {
    const char* begin;
    const char* end;
    std::string operator ()();

    using list = std::vector<cword>;
    using iterator = list::iterator;
    using cursor_t = list::iterator;
};

string_t();
string_t(const string_t& a_str);
string_t(string_t&& a_str);
string_t(const std::string& a_str);
string_t(std::string&& a_str);
string_t(const char* a_str);

~string_t();
string_t& operator=(const string_t& a_str);
string_t& operator=(std::string&&     a_atr);
string_t& operator=(string_t&&    a_str);
string_t& operator=(const char*  a_str);



string_t& operator += (const string_t& a_atr);
string_t& operator += (const std::string& a_atr);
string_t& operator += (char c);

string_t& operator + (const string_t& a_atr);
string_t& operator + (const std::string& a_atr);
string_t& operator + (char c);



template<typename t> string_t& operator += (const t& a)
{
    std::ostringstream os;
    //os.precision(_precision);
    os << a;
    _s.append(os.str());
    return *this;
}

template<typename t> string_t& operator + (const t& a)
{
    std::ostringstream os;
    //os.precision(_precision);
    os << a;
    _s.append(os.str());
    return *this;
}
bool operator==(const string_t& a_str) const;

bool empty() { return _s.empty(); }
operator bool() { return !_s.empty(); }
operator std::string() { return _s; }
std::string&  str() { return _s; }
std::string   operator()() const { return _s; }

// -- on peut maintenant commencer nos routines de manipulations et de traitements....

string_t& operator << (const string_t& a_atr);
string_t& operator << (const char* a_atr);
string_t& operator << (const std::string& a_atr);
string_t& operator << (char c);

bool skipws(std::string::iterator& pos);
bool skipws(const char* pos);
string_t & operator >> (std::string & _arg);

template<typename T> string_t & operator >> (T& _arg) {

    if constexpr (
        std::is_same<T, uint64_t&>::value ||
        std::is_same<T, uint16_t&>::value ||
        std::is_same<T, uint32_t&>::value ||
        std::is_same<T, uint64_t&>::value ||
        std::is_same<T, uint8_t&>::value ||
        std::is_same<T, uint16_t&>::value ||
        std::is_same<T, uint32_t&>::value ||
        std::is_same<T, uint64_t&>::value

        ) {
        std::string::size_type pos;
        if (((pos = _s.find("0x")) != std::string::npos) || ((pos = _s.find("0X")) != std::string::npos))
        {
            if (_s[pos] == '0') {
                ++pos;
                if ((_s[pos] == 'x') || (_s[pos] == 'X')){
                    ++pos;
                    std::stringstream i(_s.c_str() + pos);
                    i >> std::hex >> _arg;
                    return *this;
                }
            }
        }
    }
    std::istringstream in(*this);
    in.precision(_precision);
    in >> _arg;
    return *this;
}

    char* dup() {
        return _strdup(_s.c_str());
    }
    const char* c_str() { return _s.c_str(); }
    void clear();

    //virtual const std::string& tea_id() { return "string_t";}

    static std::string datetime(const std::string& str_fmt);

    template<typename t> string_t & operator =(const t& _a) {
        std::ostringstream os;
        os.precision(_precision);
        os << _a;
        clear();
        _s = os.str();
        return *this;
    }

    std::string::iterator scanto(std::string::iterator start, char c);
    const char* scanto(const char* start, char c);


    string_t& octal(uint64_t __arg) {

        std::ostringstream os;
        os << std::oct << __arg;
        if (scanarg() == std::string::npos) {
            _s.append(os.str());
            return *this;
        }

        return printf<std::string>(os.str());
    }

    size_t len() { return _s.size(); }
    char& operator[](size_t p) { return _s[p]; }
    //bool empty() { return _str.empty(); }

    static std::string use_default_separators() { return string_t::default_separators; }

    std::size_t words(string_t::word::list& warray, const std::string& a_delimiters = "", bool keep_as_word = true);

    template<typename T> string_t& arg(T _arg) {
        if (scanarg() == std::string::npos) {
            std::ostringstream os;
            os << std::oct << _arg;
            _s.append(os.str());
            return *this;
        }

        return printf<T>(_arg);
    }
    bool filter(const string_t::list& a_exp);
    template<typename T> std::string expand(const T& cnt) {
        string_t ss;

        int x = cnt.size();
        for (auto item : cnt) {
            ss << item;
            if (x-- > 1)
                ss << ',';
        }
        return ss();
    }
    static std::string up(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
            return std::toupper(c);
        });

        return s;
    }
    static std::string low(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        return s;
    }

    static std::string str_size(uint64_t sz);

    template<typename T> string_t& printf(const T& _argv);
    template<typename T> string_t& operator << (const T& a)
    {
        if (scanarg() == std::string::npos) {
            std::ostringstream os;
            os << a;
            _s += os.str();
            return *this;
        }
        return this->printf<T>(a);
    }

    template<typename T> string_t& hex(T& v)
    {
        std::stringstream is(_s.c_str()+2);
        //std::cerr << " this:'" << _s.c_str()+2 << "' -> ";
        is >> std::hex >> v;
        //std::cerr << v << '\n';
        return *this;
    }

    template<typename T> static std::string bin(T __arg, bool padd = false, int f = 128)
    {
        uint8_t seq;
        int nbytes = sizeof(T);

        uint8_t tableau[sizeof(T)];
        memcpy(tableau, (uint8_t*)&__arg, nbytes);
        std::string stream = "";
        int s = 0;
        bool discard = false;
        for (int x = 1; x <= nbytes; x++) {
            seq = tableau[nbytes - x];
            if ((x == 1 && !padd && !seq) || (stream.empty() && !padd && !seq)) continue;
            for (int y = 7; y >= 0; y--) { // est-ce que le bit #y est à 1 ?
                if (s >= f) { stream += ' '; s = 0; }
                ++s;
                uint8_t b = 1 << y;
                if (b & seq) stream += '1';
                else stream += '0';
            }
        }
        /*tableau.clear();*/
        return stream;
    }

private:
    std::string::size_type scanarg();

    void putarg(const std::string& a_str);

    bool pushword(word::list& strm, word& w, std::string::size_type sz);
};


template<typename T> string_t& string_t::printf(const T& argf)
{
    format_t fmt = { _s };
    char buf[256];
    std::memset(buf, 0, 200);
    //LFnl << "\n";


    std::string::iterator c = _s.begin() + _argpos;
    std::string::iterator n, b, l;
    b = n = c;
    ++c;
    // %[flag] :

    switch (*c) {
    case '-':
    case '+':
    case '#':
    case '0':
        fmt._f = *c++;
        break;
    default:
        //++c;
        break;
    }

    n = c;
    // %[width]:
    while ((n != _s.end()) && isdigit(*n)) ++n;
    l = n;
    --n;
    if (n >= c) {
        int t = 0;
        while (n >= c)
            fmt._w += (*(n--) - '0') * pow(10, t++);
    }
    else
        fmt._w = 0;
    c = l;

    if (*c == '.') {
        fmt._r = fmt._p;
        ++c;
        n = c;
        while ((n != _s.end()) && isdigit(*n)) ++n;
        l = n;
        --n;
        int t = 0;
        fmt._r = 0;
        while (n >= c)
            fmt._r += (*(n--) - '0') * pow(10, t++);
        c = l;
    }
    else
        fmt._r = fmt._p;

    //[.precision]
    n = c;
    //% ([length]) [specifier]
    std::string BinaryStr;
    switch (*c) {
    case 'b':
    {
        // Special Bretzelus :
        bool pad = fmt._f == '0';
        BinaryStr = string_t::bin<T>(argf, pad, fmt._w <= 128 ? fmt._w : 128);

        std::sprintf(buf, "%s", BinaryStr.c_str());
        fmt._delta = (c + 1) - b;
        _s.erase(_argpos, fmt._delta);
        _s.insert(_argpos, BinaryStr.c_str(), BinaryStr.length());
        _argpos = 0;
        return *this;
        break;
    }

    case 'd': // Decimale ou entier
    case 'i':
        fmt._s = *c++;
        break;
    case 'x':
    case 'X':
        fmt._s = *c++;
        break;
    case 'f':
    case 'F':
    case 'g':
    case 'G':
        fmt._s = *c++;
        break;
    case 's':
        fmt._s = *c++;
    }

    fmt._delta = c - b;
    std::string ff(_s, _argpos, fmt._delta);

    if constexpr (std::is_same<T, const std::string&>::value)
        std::snprintf(buf, 199, ff.c_str(), argf.c_str());
    else
        std::sprintf(buf, ff.c_str(), argf);

    _s.erase(_argpos, fmt._delta);
    _s.insert(_argpos, buf, std::strlen(buf));
    _argpos = 0;
    return *this;
}




