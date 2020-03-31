//
// Created by bretzel on 20-02-03.
//

//#ifndef PLSC_TOKENT_H
//#define PLSC_TOKENT_H


#pragma once

#include <xio/utils/xstr.h>
#include <xio/lexer/lexerdll.h>





namespace teacc::lexer
{

namespace type
{
using T = uint64_t;
constexpr T  null        = 0x000000000;
constexpr T  number      = 0x000000001;
constexpr T  text        = 0x000000002;
constexpr T  voidptr     = 0x000000004;
constexpr T  any         = 0x000000008;
constexpr T  leaf        = 0x000000010;
constexpr T  sign        = 0x000000020;
constexpr T  i8          = 0x000000040;
constexpr T  i16         = 0x000000080; // 0x7F41
constexpr T  i32         = 0x000000100;
constexpr T  i64         = 0x000000200;
constexpr T  u8          = 0x000000400;
constexpr T  u16         = 0x000000800;
constexpr T  u32         = 0x000001000;
constexpr T  u64         = 0x000002000;
constexpr T  real        = 0x000004000; // real: f32, f64, f128... ^^
constexpr T  var         = 0x000008000; // Ne pas oublier d'ajouter ce type a toutes "variables"
constexpr T  bloc        = 0x000010000;
constexpr T  filo        = 0x000020000;
constexpr T  keyword     = 0x000040000;
constexpr T  unary       = 0x000080000;
constexpr T  prefix      = 0x000100000;
constexpr T  postfix     = 0x000200000;
constexpr T  oper        = 0x000400000;
constexpr T  binary      = 0x000800000;
constexpr T  func        = 0x001000000;
constexpr T  funcptr     = 0x002000000;
constexpr T  obj         = 0x004000000;
constexpr T  pointer     = 0x008000000;
constexpr T  type        = 0x010000000;
constexpr T  id          = 0x020000000;
constexpr T  ref         = 0x040000000;
constexpr T  punctuation = 0x080000000;
constexpr T  assign      = 0x100000000;
constexpr T  boolean     = 0x200000000;
constexpr T  hex         = 0x400000000;
constexpr T  leftpar     = 0x800000000;
constexpr T  closepar    = 0x1000000000;
constexpr T  openabs     = 0x2000000000;
constexpr T  closeabs    = 0x4000000000;
constexpr T  static_t    = 0x8000000000; ///< Flag set at the parser phase when applicable.
constexpr T  f32         = 0x10000000000;
constexpr T  f64         = 0x20000000000;
constexpr T  f128        = 0x40000000000;
constexpr T  octal       = 0x80000000000;
constexpr T  bin         = 0x100000000000;

std::string LEXERAPI to_s(T ty);
T           LEXERAPI to_i(const std::string& names);

}

namespace type::delta{
using T = uint8_t;
constexpr T scope      = 0;
constexpr T assign_rs  = 1;
constexpr T unary      = 2;
constexpr T prefix     = 3;
constexpr T deref      = 4;
constexpr T par        = 5;
constexpr T exponent   = 6;
constexpr T product    = 7;
constexpr T bits       = 8;
constexpr T add        = 9;
constexpr T shift      = 10;
constexpr T less       = 11;
constexpr T comp       = 12;
constexpr T logand     = 13;
constexpr T logxor     = 14;
constexpr T logor      = 15;
constexpr T boolean    = 16;
constexpr T ternary    = 17;
constexpr T comma      = 18;
constexpr T identifier = 19;
constexpr T assign     = 20; // a = b + c = 0 - 1
constexpr T noop       = 21;
}


namespace lexem {
enum class mnemonic : uint64_t {
    knull = 0,
    lshift,
    radical,
    exponent,
    rshift,
    decr,
    incr,
    assignadd,
    assignsub,
    assignmul,
    assigndiv,
    assignmod,
    assignand,
    assignor,
    assignxor,
    assignx1,
    assignlshift,
    assignrshift,
    leq,
    geq,
    eq,
    neq,  // != <>
    add,
    sub,
    mul,
    cppcomment,
    modulo,
    lt,
    gt,
    assign,
    binand,
    binor,
    bitxor,
    x1,           // complement a 1
    x2,
    bitnot,          //
    booland,
    boolor,
    openabs,
    closeabs,
    openpar,
    closepar,
    openindex,
    closeindex,
    openbrace,
    closebrace,
    bcomment,
    ecomment,
    division,
    comma,
    scope,
    semicolon,
    colon,
    range,
    factorial,
    positive,
    negative,
    squote, // '
    dquote, // "
    kternary, // ? :
    hash, // #
    dollard,
    dot,
    kreturn,
    kif,
    kpi,
    knumber,
    ku8,
    ku16,
    ku32,
    ku64,
    ki8,
    ki16,
    ki32,
    ki64,
    kreal,
    kstring,
    kthen,
    kelse,
    kconst,
    kinclude,
    kmodule,
    kat,
    kprime,
    kdo,
    kwhile,
    kfor,
    kuntil,
    krepeat,
    kswitch,
    kcase,
    ktype,
    khex,
    kHex,
    kcos,
    kacos,
    ktan,
    katan,
    ksin,
    kasin,
    kobject,
    kstatic,
    kme,
    noop
};
}


namespace lexem{

using T = const char*;
constexpr T knull        = "null";
constexpr T lshift       = "<<";
constexpr T radical      = "^/";
constexpr T exponent     = "^";
constexpr T rshift       = ">>";
constexpr T decr         = "--";
constexpr T incr         = "++";
constexpr T assignadd    = "+=";
constexpr T assignsub    = "-=";
constexpr T assignmul    = "*=";
constexpr T assigndiv    = "/=";
constexpr T assignmod    = "%=";
constexpr T assignand    = "&=";
constexpr T assignor     = "|=";
constexpr T assignxor    = "><=";
constexpr T assignx1     = "`=";
constexpr T assignlshift = "<<=";
constexpr T assignrshift = ">>=";
constexpr T lteq         = "<=";
constexpr T gteq         = ">=";
constexpr T eq           = "==";
constexpr T neq          = "!=";  // != <>
constexpr T add          = "+";
constexpr T sub          = "-";
constexpr T mul          = "*";
constexpr T cppcomment   = "//";
constexpr T modulo       = "%";
constexpr T bitxor       = "><";
constexpr T lt           = "<";
constexpr T gt           = ">";
constexpr T assign       = "=";
constexpr T binand       = "&";
constexpr T binor        = "|";
constexpr T x1           = "`"; ///< compl&eacute;ment &agrave; 1
constexpr T x2           = "``";///< compl&eacute;ment &agrave; 2
constexpr T bitnot       = "!";
constexpr T booland      = "&&";
constexpr T boolor       = "||";
constexpr T openabs      = "|<";// |< expr >|
constexpr T closeabs     = ">|";
constexpr T openpar      = "(";
constexpr T closepar     = ")";
constexpr T openindex    = "[";
constexpr T closeindex   = "]";
constexpr T openbrace    = "{";
constexpr T closebrace   = "}";
constexpr T bcomment     = "/*";
constexpr T ecomment     = "*/";
constexpr T division     = "/";
constexpr T comma        = ",";
constexpr T scope        = "::";
constexpr T semicolon    = ";";
constexpr T colon        = ":";
constexpr T range        = "..";
constexpr T factorial    = "!";
constexpr T positive     = "+";
constexpr T negative     = "-";
constexpr T squote       = "'";  // '
constexpr T dquote       = "\""; // "
constexpr T kternary     = "?";  // ? :
constexpr T hash         = "#";  // #
constexpr T dollard      = "$";
constexpr T dot          = ".";
constexpr T kreturn      = "return";
constexpr T kif          = "if";
constexpr T kpi          = "pi";
constexpr T knumber      = "number";
constexpr T ku8          = "u8";
constexpr T ku16         = "u16";
constexpr T ku32         = "u32";
constexpr T ku64         = "u64";
constexpr T ki8          = "i8";
constexpr T ki16         = "i16";
constexpr T ki32         = "i32";
constexpr T ki64         = "i64";
constexpr T kreal        = "real";
constexpr T kstring      = "string";
constexpr T kthen        = "then";
constexpr T kelse        = "else";
constexpr T kconst       = "const";
constexpr T kinclude     = "include";
constexpr T kmodule      = "module";
constexpr T kat          = "@";
constexpr T kprime       = "`";
constexpr T kdo          = "do";
constexpr T kwhile       = "while";
constexpr T kfor         = "for";
constexpr T kuntil       = "until";
constexpr T krepeat      = "repeat";
constexpr T kswitch      = "switch";
constexpr T kcase        = "until";
constexpr T ktype        = "type";
constexpr T khex         = "0x";
constexpr T kHex         = "0X";
constexpr T kcos         = "cos";
constexpr T kacos        = "acos";
constexpr T ktan         = "tan";// tan(4*a*c)  - sin(4ac) sina
constexpr T katan        = "atan";
constexpr T ksin         = "sin";
constexpr T kasin        = "asin";
constexpr T kobject      = "object";
constexpr T kstatic      = "static";
constexpr T kme          = "me";

lexem::mnemonic LEXERAPI code(lexem::T a_lexem);


}


namespace type{

struct LEXERAPI token_t
{
    lexem::mnemonic c   = lexem::mnemonic::noop;
    T        t  =  null;    ///< Primitive Type bit.
    T        s   = null;	///< Semantic Type bits field  
    lexer::type::delta::T d = delta::noop;
    void* _data  = nullptr;
    
    using collection    = std::vector<token_t>;
    using iterator      = collection::iterator;
    
    
    struct LEXERAPI  location{
        const char* begin = nullptr;
        const char* end = nullptr;
        int L         = -1; ///< line number
        int C         = -1; ///< Column number
        int64_t I     = -1; ///< Absolute Offset from the beginning of the Source Text.
        std::string   operator()();
        std::string   lexem();
        std::string   pos();
    } _loc;
    
    struct LEXERAPI flag {
        int8_t V : 1; ///< Pre-parsed as a value token_t;
        int8_t S : 1; ///< Post parsed as assignable
        int8_t M : 1; ///< Virtual multiplication operator. (...4ac...)
    } _f = {0,0,0};
    
    static token_t _null;
    
    token_t()=default;
    ~token_t() = default;
    
    token_t(lexem::mnemonic aCode, type::T aType, type::T aSem, type::delta::T aDelta, lexem::T aLexem,  int8_t V);
    token_t(lexem::mnemonic aCode, type::T aType, type::T aSem, type::delta::T aDelta, token_t::location aLoc, token_t::flag aFlag, void* aPtr=nullptr);
    token_t(const token_t& aToken);
    token_t(token_t&& aToken)noexcept;
    
    token_t& operator=(token_t&& a_token)noexcept;
    token_t& operator=(const token_t& a_token);
    token_t  operator[](lexem::mnemonic CC);
    //token_t* ptr() { return this; }
    utils::xstr mark();
    
    operator bool() { return _loc.begin != nullptr; }
    bool operator || (type::T aSem);
    
    std::string attribute();
    
    std::string semantic_names();
    std::string type_name();
    
    
    static token_t scan(const char* sPtr);
    
    static std::string typestr(type::T ty);
    bool is_binary()        {return s & type::binary;}
    bool is_unary()         {return s & type::unary;}
    bool is_identifier()    {return s & type::id;}
    bool is_leaf()          {return s & type::leaf;}
    bool is_operator()      {return s & type::oper;}
    bool is_closing_pair()  {return s & type::closepar;}
    bool is_punctuation()   {return s & type::punctuation;}
    bool is_number()        {return s & type::number;}
    bool is_hexadecimal()   {return s & type::hex;}
    bool is_prefix()        {return s & type::prefix;}
    bool is_postfix()       {return s & type::postfix;}
    bool is_instruction()   {return !(s & (type::oper | type::leaf));}
        
    /*
        // Still to do:

    static token_t query(type::t ty);
    static token_t query(lexem::t lx);
    static token_t query(mnemonic a_code);
    static mnemonic strtomnemonic(const std::string &k);

    */
        
    };
}



}


//#endif //PLSC_TOKENT_H
