//
// Created by Serge Lussier, lussier.serge@gmail.com, (bretzel) on 20-02-26.
//

//#ifndef LIBXIO_XSTR_H
//#define LIBXIO_XSTR_H

#pragma once

#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <time.h>
#include <cstdint>
#include <cmath>
#include <xio/utils/xio.utils>


namespace teacc::utils
{

class XIOUTILS_API xstr
{
    static  std::string __nullstr__;
    std::string _s;///< Private std::string instance.
    std::string::size_type _argpos = 0; // Initialize Argument index position...
    static std::string default_separators;
    uint8_t _precision = 4;

private:
    // %[flags][width][.precision][length]specifier
    struct  prv_format {
        uint8_t _f = 0; // Flag ( - + . # 0 )
        uint8_t _w = 0; // Width ( length )
        uint8_t _r = 0; // Custom flag set if this format requier floating point spec.
        uint8_t _p = 6; // Precision (Same as  default).
        uint8_t _l = 0; // Length modifier ( l,ll,h,hh )
        std::size_t _delta = 0; // Format length.
        std::size_t _pos = 0; //
        char    _s = 0; // Effective data t
        //char    _format[20]; // Unused yet.
        const char* _c = nullptr;
        
        prv_format(std::string& a_s) : _c(a_s.c_str()) {}
        std::string operator()();
        
    };
public:
    
    using collection = std::vector<std::string>;
    using iterator = collection::iterator;
    using citerator = collection::const_iterator;


public:
    struct XIOUTILS_API word {
        std::string::const_iterator B;
        std::string::const_iterator E;
        std::string::const_iterator SE;
        
        std::string operator ()();
        std::string operator *();
        
        using collection = std::vector<word>;
        using cursor = collection::iterator;
        std::string    mark();
        
        int l = 1;
        int c = 1;
        uint64_t pos = 0;
        void operator ++();
        void operator ++(int);
        
        std::string location();
    };


private:
    struct _bce
    {
        std::string::const_iterator b;
        std::string::const_iterator c;
        std::string::const_iterator e; /// ...
        
        int l = 1;
        int col = 1;
        uint64_t i = 0;
        
        _bce() = default;
        ~_bce() = default;
        
        _bce(const std::string& a_s);
        bool skip();
        bool end();
        bool operator ++();
        bool operator ++(int);
        void reset(const std::string& _str)
        {
            c = b;
            l = col = 1;
            i = 0;
            e = _str.end();
        }
        _bce& operator >> (word& w);
        //_bce& operator = (const word& w);
        
    }_cursor;
public:
    
    xstr();
    xstr(const xstr& aStr);
    xstr(xstr&& aStr) noexcept;
    xstr(const std::string& aStr);
    xstr(std::string&& aStr);
    xstr(const char* aStr);
    
    ~xstr();
    
    xstr& operator=(const xstr&) = default;
    
    xstr& operator=(std::string&& aStr);
    xstr& operator=(xstr&& aStr) noexcept;
    xstr& operator=(const char* aStr);
    
    
    
    xstr& operator += (const xstr& aStr);
    xstr& operator += (const std::string& aStr);
    xstr& operator += (char c);
    
    xstr& operator + (const xstr& aStr);
    xstr& operator + (const std::string& aStr);
    xstr& operator + (char c);
    
    
    
    template<typename t> xstr& operator += (const t& a)
    {
        std::ostringstream os;
        os.precision(_precision);
        os << a;
        _s.append(os.str());
        return *this;
    }
    
    template<typename t> xstr& operator + (const t& a)
    {
        std::ostringstream os;
        os.precision(_precision);
        os << a;
        _s.append(os.str());
        return *this;
    }
    
    bool operator==(const xstr& aStr) const;
    
    bool empty() { return _s.empty(); }
    operator bool() { return !_s.empty(); }
    operator std::string() { return _s; }
    std::string& str() { return _s; }
    std::string   operator()() const { return _s; }
    
    // -- on peut maintenant commencer nos routines de manipulations et de traitements....
    xstr::collection to_args(int argc, char** argv);
    xstr& operator << (const xstr& aStr);
    xstr& operator << (const char* aStr);
    xstr& operator << (const std::string& aStr);
    xstr& operator << (char c);
    
    bool skipws(std::string::iterator& pos);
    static bool skipws(const char* pos);
    xstr& operator >> (std::string& _arg);
    static std::string makestr(const char* B, const char* E);
    template<typename T> xstr& operator >> (T& _arg) {
        
        if constexpr (
            std::is_same<T, uint64_t&>::value ||
            std::is_same<T, uint16_t&>::value ||
            std::is_same<T, uint32_t&>::value ||
            std::is_same<T, uint64_t&>::value ||
            std::is_same<T, uint8_t&>::value ||
            std::is_same<T, uint16_t&>::value ||
            std::is_same<T, uint32_t&>::value ||
            std::is_same<T, uint64_t&>::value)
        {
            std::string::size_type pos;
            if (((pos = _s.find("0x")) != std::string::npos) || ((pos = _s.find("0X")) != std::string::npos))
            {
                if (_s[pos] == '0') {
                    ++pos;
                    if ((_s[pos] == 'x') || (_s[pos] == 'X')) {
                        ++pos;
                        std::istringstream i(_s.c_str() + pos);
                        i >> std::hex >> _arg;
                        return *this;
                    }
                }
            }
        }
        std::istringstream in(_s); //  When xstr was derived from std::string ... Oops!  std::istringstream in(*this);
        in.precision(_precision);
        in >> _arg;
        return *this;
    }
    
    char* dup() {
        return strdup(_s.c_str());
    }
    const char* c_str() { return _s.c_str(); }
    void clear();
    
    //virtual const std::string& tea_id() { return "xstr";}
    
    static std::string datetime(const std::string& str_fmt);
    
    template<typename t> xstr& operator =(const t& _a) {
        std::ostringstream os;
        os.precision(_precision);
        os << _a;
        clear();
        _s = os.str();
        return *this;
    }
    std::string extract_substr(const std::string& first_lhs, const std::string& first_rhs);
    std::string::const_iterator scanto(std::string::const_iterator start, char c);
    const char* scanto(const char* start, char c);
    
    bool test();
    xstr& octal(uint64_t __arg) {
        
        std::ostringstream os;
        os << std::oct << __arg;
        if (scanarg() == std::string::npos) {
            _s.append(os.str());
            return *this;
        }
        
        return printf<std::string>(os.str());
    }
    
    size_t length() { return _s.size(); }
    char& operator[](size_t p) { return _s[p]; }
    //bool empty() { return _str.empty(); }
    
    static std::string use_default_separators() { return xstr::default_separators; }
    static std::string separators() { return xstr::default_separators; }
    std::size_t words(xstr::word::collection& wcollection, const std::string& a_delimiters = "", bool keep_as_word = true);
    
    template<typename T> xstr& arg(T _arg) {
        if (scanarg() == std::string::npos) {
            std::ostringstream os;
            os << std::oct << _arg;
            _s.append(os.str());
            return *this;
        }
        
        return printf<T>(_arg);
    }
    int filter(const xstr::collection& a_exp);
    
    template<typename T> std::string expand(const T& cnt) {
        xstr ss;
        
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
    
    static std::string sizef(uint64_t sz);
    
    template<typename T> xstr& printf(const T& _argv);
    template<typename T> xstr& operator << (const T& a)
    {
        if (scanarg() == std::string::npos) {
            std::ostringstream os;
            os << a;
            _s += os.str();
            return *this;
        }
        return this->printf<T>(a);
    }
    
    template<typename T> xstr& hex(T& v)
    {
        std::stringstream is(_s.c_str() + 2);
        //std::cerr << " this:'" << _s.c_str()+2 << "' -> ";
        is >> std::hex >> v;
        //std::cerr << v << '\n';
        return *this;
    }
    
    
    static std::string type_of_T(std::string&& func_desc);
    
    template<typename T> static std::string tobinary(T __arg, bool padd = false, int f = 128)
    {
        uint8_t seq;
        int nbytes = sizeof(T);
        
        uint8_t tableau[sizeof(T)];
        memcpy(tableau, (uint8_t*)&__arg, nbytes);
        std::string stream = "";
        int s = 0;
        //bool discard = false;
        for (int x = 1; x <= nbytes; x++) {
            seq = tableau[nbytes - x];
            if ((x == 1 && !padd && !seq) || (stream.empty() && !padd && !seq)) continue;
            for (int y = 7; y >= 0; y--) { // est-ce que le bit #y est � 1 ?
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
    
    
    std::string::const_iterator begin();
    std::string::const_iterator end();

private:
    std::string::size_type scanarg();
    
    void putarg(const std::string& aStr);
    int pushword(word::collection& strm, word& w, std::string::size_type sz);
    
};


template<typename T> xstr& xstr::printf(const T& argf)
{
    prv_format fmt = { _s };
    char buf[256];
    std::memset(buf, 0, 200);
    //LFnl << "\n";
    
    
    std::string::iterator c = _s.begin() + _argpos;
    std::string::iterator n, beg, l;
    beg = n = c;
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
            fmt._w = fmt._w + (*(n--) - static_cast<uint64_t>('0'))* pow(10, t++);
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
            fmt._r = fmt._r + (*(n--) - static_cast<uint64_t>('0'))* pow(10, t++);
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
            if constexpr (!std::is_same<T, const std::string&>::value)
            {
                // Special Bretzelus :
                bool pad = fmt._f == '0';
                BinaryStr = xstr::tobinary<T>(argf, pad, fmt._w <= 128 ? fmt._w : 128);
                
                //std::sprintf(buf, "%s", BinaryStr.c_str());
                fmt._delta = (c + 1) - beg;
                _s.erase(_argpos, fmt._delta);
                _s.insert(_argpos, BinaryStr.c_str(), BinaryStr.length());
                _argpos = 0;
                return *this;
            }
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
    
    fmt._delta = c - beg;
    std::string ff(_s, _argpos, fmt._delta);
    
    // -- Clang-tidy:error: cannot pass object of non-trivial type 'const std::__cxx11::basic_string<char>' through variadic function
    //
    if constexpr (std::is_same<T, const std::string&>::value)
        std::snprintf(buf, 199, ff.c_str(), argf.c_str());
    else
        std::sprintf(buf, ff.c_str(), argf);
    
    _s.erase(_argpos, fmt._delta);
    _s.insert(_argpos, buf, std::strlen(buf));
    _argpos = 0;
    return *this;
};
    
    
}
//#endif //LIBXIO_XSTR_H
 //plsc : Poor LoneSome Coder. Then LoneSome Coder.
 // FUCKING CMAKE BUG FUCK THE FUCK!!!!!!!!!!!!!
 
 