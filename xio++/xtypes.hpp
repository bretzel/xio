#pragma once
#include <xio++/xio++.hpp>
#include <string>

#include <cstdint>
#include <vector>
namespace xio {

enum class e_code : uint64_t {
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



/*
-----------------------------------------------------------------------------
1	::	Résolution de portée ---------------------------------Gauche à droite
2	++   --	Incrémentation et décrementation suffixe/postfixe
()	Appel de fonction
[]	Accès dans un tableau
.	Sélection membre par référence
−>	Sélection membre par pointeur
-----------------------------------------------------------------------------
3	++   --	Incrementation et décrementation préfixe ---------Droite à gauche
    +   −	Plus et moins unaires
    !   ~	NON logique et NON binaire
    (type)	Transtypage 
    *	Indirection (déréférence)
    &	Adresse

    sizeof	Taille
    new, new[]	Allocation dynamique de la mémoire
    delete, delete[]	Libération dynamique de la mémoire
-----------------------------------------------------------------------------
4	.*   ->*	Pointeur vers un membre	----------------------Gauche à droite
5	*   /   %	Multiplication, division et reste
6	+   −	Addition et soustraction
7	<<   >>	Décalage binaire à gauche et à droite
8	<   <=	Respectivement pour les opérateurs de comparaison < et ≤
    >   >=	Respectivement pour les opérateurs de comparaison > et ≥
9	==   !=	Respectivement pour les comparaisons = et ≠
10	&	ET binaire
11	^	XOR binaire (ou exclusif)
12	|	OU binaire (ou inclusif)
13	&&	ET logique
14	||	OU logique
-----------------------------------------------------------------------------
15	?:	opérateur conditionnel ternaire	----------------------Droite à gauche
    =	Affectation directe (fourni par défaut pour les classes C++)
    +=   −=	Affectation par somme ou différence
    *=   /=   %=	Affectation par produit, division ou reste
    <<=   >>=	Affectation par décalage binaire à gauche ou à droite
    &=   ^=   |=	Affectation par ET, XOR ou OU binaire
16	throw	opérateur Throw (pour les exceptions)
-----------------------------------------------------------------------------
----------------------------------------------------         Gauche à droite
17  ,
---------------------------------------------------------------------------







*/

namespace opdelta {
using T                     = uint8_t;
constexpr T scope           = 0;
constexpr T assign_rs       = 1;
constexpr T unary           = 2;
constexpr T prefix          = 3;
constexpr T deref           = 4;
constexpr T par             = 5;
constexpr T exponent        = 6;
constexpr T product         = 7;
constexpr T bits            = 8;
constexpr T add             = 9;
constexpr T shift           = 10;
constexpr T less            = 11;
constexpr T comp            = 12;
constexpr T logand          = 13;
constexpr T logxor          = 14;
constexpr T logor           = 15;
constexpr T boolean         = 16;
constexpr T ternary         = 17;
constexpr T comma           = 18;
constexpr T identifier      = 19;
constexpr T assign          = 20;
constexpr T noop            = 21;
}


namespace type_t {

using T = uint64_t;

constexpr T  null           = 0x000000000;
constexpr T  number         = 0x000000001;
constexpr T  text           = 0x000000002;
constexpr T  voidptr        = 0x000000004;
constexpr T  any            = 0x000000008;
constexpr T  leaf           = 0x000000010;
constexpr T  sign           = 0x000000020;
constexpr T  i8             = 0x000000040;
constexpr T  i16            = 0x000000080;
constexpr T  i32            = 0x000000100;
constexpr T  i64            = 0x000000200;
constexpr T  u8             = 0x000000400;
constexpr T  u16            = 0x000000800;
constexpr T  u32            = 0x000001000;
constexpr T  u64            = 0x000002000;
constexpr T  real           = 0x000004000;
constexpr T  var            = 0x000008000;
constexpr T  bloc           = 0x000010000;
constexpr T  filo           = 0x000020000;
constexpr T  keyword        = 0x000040000;
constexpr T  unary          = 0x000080000;
constexpr T  prefix         = 0x000100000;
constexpr T  postfix        = 0x000200000;
constexpr T  oper           = 0x000400000;
constexpr T  binary         = 0x000800000;
constexpr T  func           = 0x001000000;
constexpr T  funcptr        = 0x002000000;
constexpr T  obj            = 0x004000000;
constexpr T  pointer        = 0x008000000;
constexpr T  type           = 0x010000000;
constexpr T  id             = 0x020000000;
constexpr T  ref            = 0x040000000;
constexpr T  punctuation    = 0x080000000;
constexpr T  assign         = 0x100000000;
constexpr T  boolean        = 0x200000000;
constexpr T  hex            = 0x400000000;
constexpr T  leftpar        = 0x800000000;
constexpr T  closepar       = 0x1000000000;
constexpr T  openabs        = 0x2000000000;
constexpr T  closeabs       = 0x4000000000;
constexpr T  static_t       = 0x8000000000; ///< Flag set at the parser phase when applicable.

std::string     xio_api name(T ty);
T               xio_api type_name(const std::string& names);
uint64_t        xio_api integral(T Ty);

}

namespace lexem {
using T                             = const char*;
constexpr const char* knull         = "null";
constexpr const char* lshift        = "<<";
constexpr const char* radical       = "^/";
constexpr const char* exponent      = "^";
constexpr const char* rshift        = ">>";
constexpr const char* decr          = "--";
constexpr const char* incr          = "++";
constexpr const char* assignadd     = "+=";
constexpr const char* assignsub     = "-=";
constexpr const char* assignmul     = "*=";
constexpr const char* assigndiv     = "/=";
constexpr const char* assignmod     = "%=";
constexpr const char* assignand     = "&=";
constexpr const char* assignor      = "|=";
constexpr const char* assignxor     = "><=";
constexpr const char* assignx1      = "`=";
constexpr const char* assignlshift  = "<<=";
constexpr const char* assignrshift  = ">>=";
constexpr const char* lteq          = "<=";
constexpr const char* gteq          = ">=";
constexpr const char* eq            = "==";
constexpr const char* neq           = "!=";  // != <>
constexpr const char* add           = "+";
constexpr const char* sub           = "-";
constexpr const char* mul           = "*";
constexpr const char* cppcomment    = "//";
constexpr const char* modulo        = "%";
constexpr const char* bitxor        = "><";
constexpr const char* lt            = "<";
constexpr const char* gt            = ">";
constexpr const char* assign        = "=";
constexpr const char* binand        = "&";
constexpr const char* binor         = "|";
constexpr const char* x1            = "`"; ///< compl&eacute;ment &agrave; 1
constexpr const char* x2            = "``";///< compl&eacute;ment &agrave; 2
constexpr const char* bitnot        = "!";
constexpr const char* booland       = "&&";
constexpr const char* boolor        = "||";
constexpr const char* openabs       = "|<";// |< expr >|
constexpr const char* closeabs      = ">|";
constexpr const char* openpar       = "(";
constexpr const char* closepar      = ")";
constexpr const char* openindex     = "[";
constexpr const char* closeindex    = "]";
constexpr const char* openbrace     = "{";
constexpr const char* closebrace    = "}";
constexpr const char* bcomment      = "/*";
constexpr const char* ecomment      = "*/";
constexpr const char* division      = "/";
constexpr const char* comma         = ",";
constexpr const char* scope         = "::";
constexpr const char* semicolon     = ";";
constexpr const char* colon         = ":";
constexpr const char* range         = "..";
constexpr const char* factorial     = "!";
constexpr const char* positive      = "+";
constexpr const char* negative      = "-";
constexpr const char* squote        = "'";  // '
constexpr const char* dquote        = "\""; // "
constexpr const char* kternary      = "?";  // ? :
constexpr const char* hash          = "#";  // #
constexpr const char* dollard       = "$";
constexpr const char* dot           = ".";
constexpr const char* kreturn       = "return";
constexpr const char* kif           = "if";
constexpr const char* kpi           = "pi";
constexpr const char* knumber       = "number";
constexpr const char* ku8           = "u8";
constexpr const char* ku16          = "u16";
constexpr const char* ku32          = "u32";
constexpr const char* ku64          = "u64";
constexpr const char* ki8           = "i8";
constexpr const char* ki16          = "i16";
constexpr const char* ki32          = "i32";
constexpr const char* ki64          = "i64";
constexpr const char* kreal         = "real";
constexpr const char* kstring       = "string";
constexpr const char* kthen         = "then";
constexpr const char* kelse         = "else";
constexpr const char* kconst        = "const";
constexpr const char* kinclude      = "include";
constexpr const char* kmodule       = "module";
constexpr const char* kat           = "@";
constexpr const char* kprime        = "`";
constexpr const char* kdo           = "do";
constexpr const char* kwhile        = "while";
constexpr const char* kuntil        = "until";
constexpr const char* krepeat       = "repeat";
constexpr const char* kswitch       = "switch";
constexpr const char* kcase         = "until";
constexpr const char* ktype         = "type";
constexpr const char* khex          = "0x";
constexpr const char* kHex          = "0X";
constexpr const char* kcos          = "cos";
constexpr const char* kacos         = "acos";
constexpr const char* ktan          = "tan";
constexpr const char* katan         = "atan";
constexpr const char* ksin          = "sin";
constexpr const char* kasin         = "asin";
constexpr const char* kobject       = "object";
constexpr const char* kstatic       = "static";
constexpr const char* kme           = "me";

e_code xio_api code(lexem::T lex);

}




struct xio_api token_t final {

    e_code     code = e_code::knull;
    type_t::T  type = type_t::null;
    type_t::T  sem  = type_t::null;
    opdelta::T delta = opdelta::identifier;
    void*      data = nullptr;

    using list_t    = std::vector<token_t>;
    using cursor    = list_t::iterator;

    struct xio_api src {
        const char* b = nullptr;
        const char* e = nullptr;
        int         l = -1; ///< Line number
        int         c = -1; ///< Column number;
        int         p = -1; ///< Offset from the beginning.
    }loc;

    struct xio_api flags {
        int8_t v : 1; ///< Pre-parsed as a value token;
        int8_t s : 1; ///< Post parsed as assignable  
        int8_t m : 1; ///< Virtual multiplication operator.
    }f;


    static token_t token_null;
    token_t();// {};

    token_t(
        e_code  a_code,
        type_t::T a_type,
        type_t::T a_sem,
        opdelta::T a_delta,
        token_t::src a_src,
        token_t::flags a_flags,
        void* APtr = nullptr
    );

    token_t(const token_t& a_token);
    token_t(token_t&& a_token);

    virtual ~token_t();

    token_t& operator = (token_t&& a_token);
    token_t& operator = (const token_t& a_token);
    token_t* me() { return this; }
    std::string mark();
    operator bool() const;


    bool is_binary();
    bool is_unary();
    bool is_identifier();
    bool is_leaf();
    bool is_operator();
    bool is_closingpair();
    bool is_punctuation();
    bool is_number();
    bool is_hexadecimal();
    bool is_prefix();
    bool is_postfix();
    bool is_instruction();

    std::string attribute();
    std::string informations();

    static token_t scan(const char* src);
    static token_t query(type_t::T ty);
    static token_t query(lexem::T lx);
    static token_t query(e_code a_code);
    

};



}

