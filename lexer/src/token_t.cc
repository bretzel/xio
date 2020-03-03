//
// Created by bretzel on 20-02-03.
//

#include <xio/lexer/token_t.h>

#include <map>



using xio::utils::xstr;

namespace xio::lexer
{

namespace type
{
std::map<T, std::string> T_STR = {{null,        "null"},
                                  {number,      "number"},
                                  {text,        "text"},
                                  {voidptr,     "voidptr"},
                                  {any,         "any"},
                                  {leaf,        "leaf"},
                                  {sign,        "sign"},
                                  {i8,          "i8"},
                                  {i16,         "i16"},
                                  {i32,         "i32"},
                                  {i64,         "i64"},
                                  {u8,          "u8"},
                                  {u16,         "u16"},
                                  {u32,         "u32"},
                                  {u64,         "u64"},
                                  {real,        "real"},
                                  {var,         "var"},
                                  {bloc,        "bloc"},
                                  {filo,        "filo"},
                                  {keyword,     "keyword"},
                                  {unary,       "unary"},
                                  {prefix,      "prefix"},
                                  {postfix,     "postfix"},
                                  {oper,        "operator"},
                                  {binary,      "binary"},
                                  {func,        "rtfn"},
                                  {funcptr,     "rtfn ptr"},
                                  {obj,         "object"},
                                  {pointer,     "pointer"},
                                  {type,        "t"},
                                  {id,          "identifier"},
                                  {ref,         "reference"},
                                  {punctuation, "punctuation"},
                                  {assign,      "assign"},
                                  {boolean,     "boolean"},
                                  {hex,         "hex"},
                                  {bin,         "bin"},
                                  {octal,       "oct"},
                                  {f32,         "f32"},
                                  {f64,         "f64"},
                                  {f128,        "f128"},
                                  {leftpar,     "leftpar"},
                                  {closepar,    "closepar"},
                                  {openabs,     "openabs"},
                                  {closeabs,    "closeabs"},
                                  {static_t,    "static"}};

std::map<std::string, T> STR_T = {{"null",         null},
                                  {"number",       number},
                                  {"text",         text},
                                  {"string",       text},
                                  {"voidptr",      voidptr},
                                  {"void-pointer", voidptr},
                                  {"any",          any},
                                  {"leaf",         leaf},
                                  {"sign",         sign},
                                  {"i8",           i8},
                                  {"i16",          i16},
                                  {"i32",          i32},
                                  {"i64",          i64},
                                  {"u8",           u8},
                                  {"u16",          u16},
                                  {"u32",          u32},
                                  {"u64",          u64},
                                  {"real",         real},
                                  {"var",          var},
                                  {"variable",     var},
                                  {"bloc",         bloc},
                                  {"filo",         filo},
                                  {"stack",        filo},
                                  {"keyword",      keyword},
                                  {"unary",        unary},
                                  {"prefix",       prefix},
                                  {"postfix",      postfix},
                                  {"oper",         oper},
                                  {"operator",     oper},
                                  {"binary",       binary},
                                  {"binary-op",    binary},
                                  {"func",         func},
                                  {"rtfn",         func},
                                  {"funcptr",      funcptr},
                                  {"rtfn-pointer", funcptr},
                                  {"obj",          obj},
                                  {"object",       obj},
                                  {"pointer",      pointer},
                                  {"t",            type},
                                  {"id",           id},
                                  {"identifier",   id},
                                  {"ref",          ref},
                                  {"reference",    ref},
                                  {"punctuation",  punctuation},
                                  {"assign",       assign},
                                  {"boolean",      boolean},
                                  {"hex",          hex},
                                  {"leftpar",      leftpar},
                                  {"closepar",     closepar},
                                  {"openabs",      openabs},
                                  {"closeabs",     closeabs}
};

std::string to_s(T ty)
{
    
    T tbm = ty;
    xstr str;
    int a,c,b;
    
    
    for(const auto &t : T_STR) {
        if(!tbm)
            break;
        if(t.first & tbm) {
            str << t.second;
            tbm &= ~t.first;
            if(tbm)
                str << ':';
        }
    }
    if(str.empty())
        str = "null";
    return str();
}

T to_i(const std::string &a_names)
{
    xstr::word::collection Array;
    xstr names = a_names;
    
    T result = 0;
    std::size_t count = names.words(Array, xstr::use_default_separators(), false);
    if(count > 0) {
        for(auto s : Array) {
            std::string txt = s();
            result |= STR_T[txt];
        }
    }
    
    return result;
}

}



static const char *fmt = "'%s'(%d,%d) %s[%s]";

namespace type
{
std::string token_t::location::operator()()
{
    // line y, col x: `attribute`
    return lexem();
}

std::string token_t::location::lexem()
{
    if(begin == nullptr)
        return std::string();
    
    std::string str;
    const char *ptr = begin;
    
    str += *ptr++;
    if(!end) {
        while(*ptr)
            str += *ptr++;
        return str;
    }
    while(ptr <= end)
        str += *ptr++;
    return str;
}

std::string token_t::location::pos()
{
    xstr Str = "%d,%d";
    Str << L << C;
    return Str();
}

token_t LEXERAPI token_t::_null = token_t();

token_t::token_t(lexem::mnemonic aCode, type::T aType, type::T aSem, type::delta::T aDelta, lexem::T aLexem, int8_t aFlags)
{
    c = aCode;
    t = aType;
    s = aSem;
    d = aDelta;
    _loc = {aLexem, 0, 0, 0, -1};
    _f = {aFlags, 0, 0};
    
}

token_t::token_t(lexem::mnemonic aCode, type::T aType, type::T aSem, type::delta::T aDelta, token_t::location aLoc, token_t::flag aFlag, void *aPtr)
{
    c = aCode;
    t = aType;
    s = aSem;
    d = aDelta;
    _loc = aLoc;
    _f = aFlag;
    _data = aPtr;
}

token_t::token_t(const token_t &aToken)
{
    c = aToken.c;
    t = aToken.t;
    s = aToken.s;
    d = aToken.d;
    _loc = aToken._loc;
    _f = aToken._f;
    _data = aToken._data;
    
}

token_t::token_t(token_t &&aToken) noexcept
{
    std::swap(c, aToken.c);
    std::swap(t, aToken.t);
    std::swap(s, aToken.s);
    std::swap(d, aToken.d);
    std::swap(_loc, aToken._loc);
    std::swap(_f, aToken._f);
    std::swap(_data, aToken._data);
    
}

token_t &token_t::operator=(token_t &&aToken) noexcept
{
    std::swap(c, aToken.c);
    std::swap(t, aToken.t);
    std::swap(s, aToken.s);
    std::swap(d, aToken.d);
    std::swap(_loc, aToken._loc);
    std::swap(_f, aToken._f);
    std::swap(_data, aToken._data);
    return *this;
}

token_t &token_t::operator=(const token_t &aToken)
{
    c = aToken.c;
    t = aToken.t;
    s = aToken.s;
    d = aToken.d;
    _loc = aToken._loc;
    _f = aToken._f;
    _data = aToken._data;
    return *this;
}

xstr token_t::mark()
{
    xstr str;
    const char *B = _loc.begin - _loc.I;
    //std::string str;
    int l = 1;
    const char *cc = _loc.begin;
    // localiser le debut de la ligne;
    while(*cc && (cc > B) && (*cc != '\n') && (*cc != '\r'))
        --cc;
    // debut de la ligne ou de la source:
    if(cc >= B) {
        if((*cc == '\n') || (*cc == '\r'))
            ++cc;
        while(*cc && (*cc != '\n') && (*cc != '\r'))
            str += *cc++;
    }
    xstr tstr;
    
    tstr << str << '\n';
    for(int x = 1; x < _loc.C; x++)
        tstr << ' ';
    tstr << '^';
    //std::cerr << tstr() << '\n';
    return tstr;
}

bool token_t::operator||(type::T aSem)
{
    return (s & aSem) != 0;
}

std::string token_t::attribute()
{
    return _loc.lexem();
}

std::string token_t::semantic_names()
{
    return type::to_s(s);
}

std::string token_t::type_name()
{
    return type::to_s(t);
}

static token_t::collection tokens_table =
    {
        {lexem::mnemonic::knull,        type::keyword,     0x000040000,  type::delta::identifier, lexem::knull,        1},
        {lexem::mnemonic::lshift,       type::binary,      0x000C00000,  type::delta::shift,      lexem::lshift,       1},
        {lexem::mnemonic::openabs,      type::openabs,     0x080C00000,  type::delta::par,        lexem::openabs,      1},
        {lexem::mnemonic::closeabs,     type::closeabs,    0x080C00000,  type::delta::par,        lexem::closeabs,     1},
        {lexem::mnemonic::radical,      type::binary,      0x000C00000,  type::delta::exponent,   lexem::radical,      1},
        {lexem::mnemonic::exponent,     type::binary,      0x000C00000,  type::delta::exponent,   lexem::exponent,     1},
        {lexem::mnemonic::rshift,       type::binary,      0x000400000,  type::delta::shift,      lexem::rshift,       1},
        {lexem::mnemonic::bitxor,       type::binary,      0x000C00000,  type::delta::bits,       lexem::bitxor,       1},
        {lexem::mnemonic::decr,         type::unary,       0x000580000,  type::delta::unary,      lexem::decr,         1},
        {lexem::mnemonic::incr,         type::unary,       0x000580000,  type::delta::unary,      lexem::incr,         1},
        {lexem::mnemonic::assignadd,    type::assign,      0x100C40000,  type::delta::assign,     lexem::assignadd,    1},
        {lexem::mnemonic::assignsub,    type::assign,      0x100C40000,  type::delta::assign,     lexem::assignsub,    1},
        {lexem::mnemonic::assignmul,    type::assign,      0x100C40000,  type::delta::assign,     lexem::assignmul,    1},
        {lexem::mnemonic::assigndiv,    type::assign,      0x100C40000,  type::delta::assign,     lexem::assigndiv,    1},
        {lexem::mnemonic::assignmod,    type::assign,      0x100C40000,  type::delta::assign,     lexem::assignmod,    1},
        {lexem::mnemonic::assignand,    type::assign,      0x100C40000,  type::delta::assign,     lexem::assignand,    1},
        {lexem::mnemonic::assignor,     type::assign,      0x100C40000,  type::delta::assign,     lexem::assignor,     1},
        {lexem::mnemonic::assignxor,    type::assign,      0x100C40000,  type::delta::assign,     lexem::assignxor,    1},
        {lexem::mnemonic::assignx1,     type::assign,      0x100C40000,  type::delta::assign,     lexem::assignx1,     1},
        {lexem::mnemonic::assignlshift, type::assign,      0x100C40000,  type::delta::assign,     lexem::assignlshift, 1},
        {lexem::mnemonic::assignrshift, type::assign,      0x100C40000,  type::delta::assign,     lexem::assignrshift, 1},
        {lexem::mnemonic::leq,          type::binary,      0x200C00000,  type::delta::less,       lexem::lteq,         1},
        {lexem::mnemonic::geq,          type::binary,      0x200C00000,  type::delta::less,       lexem::gteq,         1},
        {lexem::mnemonic::eq,           type::binary,      0x200C00000,  type::delta::less,       lexem::eq,           1},
        {lexem::mnemonic::neq,          type::binary,      0x200C00000,  type::delta::less,       lexem::neq,          1},
        {lexem::mnemonic::add,          type::binary,      0x000C00000,  type::delta::add,        lexem::add,          1},
        {lexem::mnemonic::sub,          type::binary,      0x000C00000,  type::delta::add,        lexem::sub,          1},
        {lexem::mnemonic::mul,          type::binary,      0x000C00000,  type::delta::product,    lexem::mul,          1},
        {lexem::mnemonic::cppcomment,   type::punctuation, 0x00000,      type::delta::noop,       lexem::cppcomment,   0},
        {lexem::mnemonic::modulo,       type::binary,      0x000C00000,  type::delta::product,    lexem::modulo,       1},
        {lexem::mnemonic::lt,           type::binary,      0x200C00000,  type::delta::less,       lexem::lt,           1},
        {lexem::mnemonic::gt,           type::binary,      0x200C00000,  type::delta::less,       lexem::gt,           1},
        {lexem::mnemonic::assign,       type::assign,      0x100C40000,  type::delta::assign,     lexem::assign,       1},
        {lexem::mnemonic::binand,       type::binary,      0x000C00000,  type::delta::bits,       lexem::binand,       1},
        {lexem::mnemonic::binor,        type::binary,      0x000C00000,  type::delta::bits,       lexem::binor,        1},
        {lexem::mnemonic::x2,           type::prefix,      0x000580000,  type::delta::prefix,     lexem::x2,           1},
        {lexem::mnemonic::x1,           type::prefix,      0x000580000,  type::delta::prefix,     lexem::x1,           1},
        {lexem::mnemonic::bitnot,       type::unary,       0x000580000,  type::delta::prefix,     lexem::bitnot,       1},
        {lexem::mnemonic::booland,      type::binary,      0x200C00000,  type::delta::logand,     lexem::booland,      1},
        {lexem::mnemonic::boolor,       type::binary,      0x200C00000,  type::delta::logor,      lexem::boolor,       1},
        {lexem::mnemonic::openpar,      type::leftpar,     0x880C00000,  type::delta::par,        lexem::openpar,      1},
        {lexem::mnemonic::closepar,     type::closepar,    0x1080C00000, type::delta::par,        lexem::closepar,     1},
        {lexem::mnemonic::openindex,    type::binary,      0x2080C00000, type::delta::par,        lexem::openindex,    1},
        {lexem::mnemonic::closeindex,   type::binary,      0x4080C00000, type::delta::par,        lexem::closeindex,   1},
        {lexem::mnemonic::openbrace,    type::binary,      0x880C00000,  type::delta::par,        lexem::openbrace,    1},
        {lexem::mnemonic::closebrace,   type::binary,      0x1080C00000, type::delta::par,        lexem::closebrace,   1},
        {lexem::mnemonic::bcomment,     type::punctuation, 0x00000,      type::delta::noop,       lexem::bcomment,     0},
        {lexem::mnemonic::ecomment,     type::punctuation, 0x00000,      type::delta::noop,       lexem::ecomment,     0},
        {lexem::mnemonic::division,     type::binary,      0x000C00000,  type::delta::product,    lexem::division,     1},
        {lexem::mnemonic::comma,        type::binary,      0x080C00000,  type::delta::comma,      lexem::comma,        1},
        {lexem::mnemonic::scope,        type::binary,      0x080C00000,  type::delta::scope,      lexem::scope,        1},
        {lexem::mnemonic::semicolon,    type::punctuation, 0x080000000,  type::delta::noop,       lexem::semicolon,    0},
        {lexem::mnemonic::colon,        type::binary,      0x080C00000,  type::delta::noop,       lexem::colon,        0},
        {lexem::mnemonic::range,        type::binary,      0x080C00000,  type::delta::scope,      lexem::range,        1},
        {lexem::mnemonic::factorial,    type::postfix,     0x000580000,  type::delta::unary,      lexem::factorial,    1},
        {lexem::mnemonic::positive,     type::prefix,      0x000580020,  type::delta::unary,      lexem::positive,     1},
        {lexem::mnemonic::negative,     type::prefix,      0x000580020,  type::delta::unary,      lexem::negative,     1},
        {lexem::mnemonic::squote,       type::text,        0x000000012,  type::delta::identifier, lexem::squote,       1},
        {lexem::mnemonic::dquote,       type::text,        0x000000012,  type::delta::identifier, lexem::dquote,       1},
        {lexem::mnemonic::kternary,     type::keyword,     0x000C40000,  type::delta::unary,      lexem::kternary,     1},
        {lexem::mnemonic::hash,         type::prefix,      0x000580000,  type::delta::unary,      lexem::hash,         1},
        {lexem::mnemonic::dollard,      type::prefix,      0x000580000,  type::delta::unary,      lexem::dollard,      1},
        {lexem::mnemonic::dot,          type::binary,      0x080C00000,  type::delta::scope,      lexem::dot,          1},
        {lexem::mnemonic::kreturn,      type::keyword,     0x000040000,  type::delta::identifier, lexem::kreturn,      0},
        {lexem::mnemonic::kif,          type::keyword,     0x000040000,  type::delta::identifier, lexem::kif,          0},
        {lexem::mnemonic::kpi,          type::keyword,     0x000044011,  type::delta::identifier, lexem::kpi,          1},
        {lexem::mnemonic::knumber,      type::prefix,      0x000540001,  type::delta::identifier, lexem::knumber,      1},
        {lexem::mnemonic::ku8,          type::prefix,      0x000540400,  type::delta::identifier, lexem::ku8,          1},
        {lexem::mnemonic::ku16,         type::prefix,      0x000540800,  type::delta::identifier, lexem::ku16,         1},
        {lexem::mnemonic::ku32,         type::prefix,      0x000541000,  type::delta::identifier, lexem::ku32,         1},
        {lexem::mnemonic::ku64,         type::prefix,      0x000542000,  type::delta::identifier, lexem::ku64,         1},
        {lexem::mnemonic::ki8,          type::prefix,      0x000540040,  type::delta::identifier, lexem::ki8,          1},
        {lexem::mnemonic::ki16,         type::prefix,      0x000540080,  type::delta::identifier, lexem::ki16,         1},
        {lexem::mnemonic::ki32,         type::prefix,      0x000540100,  type::delta::identifier, lexem::ki32,         1},
        {lexem::mnemonic::ki64,         type::prefix,      0x000540200,  type::delta::identifier, lexem::ki64,         1},
        {lexem::mnemonic::kreal,        type::prefix,      0x000544000,  type::delta::identifier, lexem::kreal,        1},
        {lexem::mnemonic::kstring,      type::prefix,      0x000540002,  type::delta::identifier, lexem::kstring,      1},
        {lexem::mnemonic::kthen,        type::keyword,     0x000040000,  type::delta::identifier, lexem::kthen,        0},
        {lexem::mnemonic::kelse,        type::keyword,     0x000040000,  type::delta::identifier, lexem::kelse,        0},
        {lexem::mnemonic::kconst,       type::keyword,     0x000040000,  type::delta::identifier, lexem::kconst,       0},
        {lexem::mnemonic::kinclude,     type::keyword,     0x000040000,  type::delta::identifier, lexem::kinclude,     0},
        {lexem::mnemonic::kmodule,      type::keyword,     0x000040000,  type::delta::identifier, lexem::kmodule,      0},
        {lexem::mnemonic::kat,          type::prefix,      0x000580000,  type::delta::unary,      lexem::kat,          1},
        {lexem::mnemonic::kprime,       type::prefix,      0x000580000,  type::delta::unary,      lexem::kprime,       1},
        {lexem::mnemonic::kdo,          type::keyword,     0x000040000,  type::delta::noop,       lexem::kdo,          0},
        {lexem::mnemonic::kwhile,       type::keyword,     0x000040000,  type::delta::noop,       lexem::kwhile,       0},
        {lexem::mnemonic::kfor,         type::keyword,     0x000040000,  type::delta::noop,       lexem::kfor,         0},
        {lexem::mnemonic::kuntil,       type::keyword,     0x000040000,  type::delta::noop,       lexem::kuntil,       0},
        {lexem::mnemonic::krepeat,      type::keyword,     0x000040000,  type::delta::noop,       lexem::krepeat,      0},
        {lexem::mnemonic::kswitch,      type::keyword,     0x000040000,  type::delta::noop,       lexem::kswitch,      0},
        {lexem::mnemonic::kcase,        type::keyword,     0x000040000,  type::delta::noop,       lexem::kcase,        0},
        {lexem::mnemonic::ktype,        type::keyword,     0x000040000,  type::delta::noop,       lexem::ktype,        0},
        {lexem::mnemonic::khex,         type::hex,         0x000040011,  type::delta::noop,       lexem::khex,         1},
        {lexem::mnemonic::kHex,         type::hex,         0x000040011,  type::delta::noop,       lexem::kHex,         1},
        {lexem::mnemonic::kcos,         type::prefix,      0x000580000,  type::delta::unary,      lexem::kcos,         1},
        {lexem::mnemonic::kacos,        type::prefix,      0x0005C0000,  type::delta::unary,      lexem::kacos,        1},
        {lexem::mnemonic::ktan,         type::prefix,      0x0005C0000,  type::delta::unary,      lexem::ktan,         1},
        {lexem::mnemonic::katan,        type::prefix,      0x0005C0000,  type::delta::unary,      lexem::katan,        1},
        {lexem::mnemonic::ksin,         type::prefix,      0x0005C0000,  type::delta::unary,      lexem::ksin,         1},
        {lexem::mnemonic::kasin,        type::prefix,      0x0005C0000,  type::delta::unary,      lexem::kasin,        1},
        {lexem::mnemonic::kobject,      type::keyword,     0x000040000,  type::delta::identifier, lexem::kobject,      0},
        {lexem::mnemonic::kstatic,      type::keyword,     0x000040000,  type::delta::identifier, lexem::kstatic,      0},
        {lexem::mnemonic::kme,          type::keyword,     0x000040000,  type::delta::identifier, lexem::kme,          1}
    };

token_t token_t::scan(const char *sPtr)
{
    int unicode = 0;
    int index = 0;
    
    for(auto token_t : tokens_table) {
        const char *crs = sPtr;
        const char *rtxt = token_t._loc.begin;
        std::size_t sz = std::strlen(rtxt);
        if(*crs != *token_t._loc.begin) {
            ++index;
            continue;
        }
        ++index;
        /*
         * I arbitrary assume that the UNICODE UNIT is a signed 16 bits AND THAT THE VALUE OF FIRST BYTE IS NEGATIVE.
         * Then I take the next byte to determine the unicode ...code...
         */
        while((*crs && *rtxt) && (*crs == *rtxt)) {
            ////std::cout << *crs <<  *rtxt << ">>>";
            if(*crs < 0)
                ++unicode;
            ++crs;
            ++rtxt;
            ////std::cout << *crs << *rtxt << ',';
        }
        
        if(*rtxt == 0) {
            ////std::cout << "\nDebug Trace - TokenUnit::Scan: rtxt=[" << token_t._loc.b << "]\n";
            if((*crs && (isalnum(*(crs)))) && (token_t.s & type::keyword) && !(token_t.s & type::oper) && !(token_t.t == type::hex))
                continue;
            
            token_t._loc.begin = sPtr;
            token_t._loc.end = token_t._loc.begin + sz - 1;
            //--------------------------------------------------------
            return token_t;
        }
    }
    return token_t::_null;
}

token_t token_t::operator[](lexem::mnemonic CC)
{
    for(token_t T : tokens_table) {
        if(CC == T.c)
            return T;
    }
    return token_t::_null;
}
std::string token_t::typestr(type::T ty)
{
    auto t = T_STR.find(ty);
    if(t == T_STR.end()) return "nan";
    return T_STR[ty];
}
    
}
}


namespace xio::lexer::lexem
{

    mnemonic code(lexem::T a_lexem)
    {
        for (auto token : type::tokens_table)
            if (std::string(a_lexem) == std::string(token._loc.begin)) return token.c;
        return mnemonic::noop;
    }
    
}