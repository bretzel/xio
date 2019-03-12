#include "xtypes.hpp"

#include <cstring>

#include "string_t.hpp"
#include "journal/logger.hpp"
#include <map>

namespace xio {




namespace type_t {




std::string xio_api name(T ty)
{
    std::string str;
    if (ty == null) return "null";

    if (ty&number) {
        if (!str.empty()) str += ':'; str += "number";
    }
    if (ty&text) {
        if (!str.empty()) str += ':'; str += "text";
    }
    if (ty&voidptr) {
        if (!str.empty()) str += ':'; str += "voidptr";
    }
    if (ty&any) {
        if (!str.empty()) str += ':'; str += "any";
    }
    if (ty&leaf) {
        if (!str.empty()) str += ':'; str += "leaf";
    }
    if (ty&sign) {
        if (!str.empty()) str += ':'; str += "sign";
    }
    if (ty&real) {
        if (!str.empty()) str += ':'; str += "real";
    }
    if (ty&i8) {
        if (!str.empty()) str += ':'; str += "i8";
    }
    if (ty&i16) {
        if (!str.empty()) str += ':'; str += "i16";
    }
    if (ty&i32) {
        if (!str.empty()) str += ':'; str += "i32";
    }
    if (ty&i64) {
        if (!str.empty()) str += ':'; str += "i64";
    }
    if (ty&u8) {
        if (!str.empty()) str += ':'; str += "u8";
    }
    if (ty&u16) {
        if (!str.empty()) str += ':'; str += "u16";
    }
    if (ty&u32) {
        if (!str.empty()) str += ':'; str += "u32";
    }
    if (ty&u64) {
        if (!str.empty()) str += ':'; str += "u64";
    }
    if (ty&var) {
        if (!str.empty()) str += ':'; str += "var";
    }
    if (ty&bloc) {
        if (!str.empty()) str += ':'; str += "bloc";
    }
    if (ty&filo) {
        if (!str.empty()) str += ':'; str += "stack";
    }
    if (ty&keyword) {
        if (!str.empty()) str += ':'; str += "keyword";
    }
    if (ty&unary) {
        if (!str.empty()) str += ':'; str += "unary";
    }
    if (ty&prefix) {
        if (!str.empty()) str += ':'; str += "prefix";
    }
    if (ty&postfix) {
        if (!str.empty()) str += ':'; str += "postfix";
    }
    if (ty&oper) {
        if (!str.empty()) str += ':'; str += "operator";
    }
    if (ty&binary) {
        if (!str.empty()) str += ':'; str += "binary";
    }
    if (ty&func) {
        if (!str.empty()) str += ':'; str += "function";
    }
    if (ty&funcptr) {
        if (!str.empty()) str += ':'; str += "function pointer";
    }
    if (ty&obj) {
        if (!str.empty()) str += ':'; str += "object";
    }
    if (ty&pointer) {
        if (!str.empty()) str += ':'; str += "pointer";
    }
    if (ty&type) {
        if (!str.empty()) str += ':'; str += "type";
    }
    if (ty&id) {
        if (!str.empty()) str += ':'; str += "identifier";
    }
    if (ty&punctuation) {
        if (!str.empty()) str += ':'; str += "punctuation";
    }
    if (ty&ref) {
        if (!str.empty()) str += ':'; str += "reference";
    }
    if (ty&assign) {
        if (!str.empty()) str += ':'; str += "assign";
    }
    if (ty&hex) {
        if (!str.empty()) str += ':'; str += "hex";
    }
    if (ty&leftpar) {
        if (!str.empty()) str += ':'; str += "leftpar";
    }
    if (ty&closepar) {
        if (!str.empty()) str += ':'; str += "closepar";
    }
    if (ty&openabs) {
        if (!str.empty()) str += ':'; str += "leftabs";
    }
    if (ty&closeabs) {
        if (!str.empty()) str += ':'; str += "closeabs";
    }

    return str;

}

T xio_api strtotype(const std::string & a_names)
{
    string_t::word::list list;
    string_t names = a_names;

    std::map<std::string, T> _ = {
        {"null"          ,null       },
        {"number"        ,number     },
        {"text"          ,text       },
        {"string"        ,text       },
        {"voidptr"       ,voidptr    },
        {"void-pointer"  ,voidptr    },
        {"any"           ,any        },
        {"leaf"          ,leaf       },
        {"sign"          ,sign       },
        {"i8"            ,i8         },
        {"i16"           ,i16        },
        {"i32"           ,i32        },
        {"i64"           ,i64        },
        {"u8"            ,u8         },
        {"u16"           ,u16        },
        {"u32"           ,u32        },
        {"u64"           ,u64        },
        {"real"          ,real       },
        {"var"           ,var        },
        {"variable"      ,var        },
        {"bloc"          ,bloc       },
        {"filo"          ,filo       },
        {"stack"         ,filo       },
        {"keyword"       ,keyword    },
        {"unary"         ,unary      },
        {"prefix"        ,prefix     },
        {"postfix"       ,postfix    },
        {"oper"          ,oper       },
        {"operator"      ,oper       },
        {"binary"        ,binary     },
        {"binary-op"     ,binary     },
        {"func"          ,func       },
        {"function"      ,func       },
        {"funcptr"       ,funcptr    },
        {"function-pointer",funcptr    },
        {"obj"           ,obj        },
        {"object"         ,obj        },
        {"pointer"       ,pointer    },
        {"type"          ,type       },
        {"id"            ,id         },
        {"identifier"    ,id         },
        {"ref"           ,ref        },
        {"reference"     ,ref        },
        {"punctuation"   ,punctuation},
        {"assign"        ,assign     },
        {"boolean"       ,boolean    },
        {"hex"           ,hex        },
        {"leftpar"      ,leftpar    },
        {"closepar"      ,closepar   },
        {"openabs"      ,openabs    },
        {"closeabs"      ,closeabs   },
    };

    T result = 0;
    std::size_t count = names.words(list, string_t::use_default_separators(), false);
    if (count > 0) {
        //logdebugfn << " names:[" << a_names << "]:"<< Ends;
        for (auto s : list) {
            std::string txt = s();
            result |= _[txt];
        //    logdebugfn << '`' << txt << "`" <<   Ends;
        }
    }
    
    return result;
}

uint64_t xio_api integral(T Ty)
{
    return static_cast<uint64_t>(Ty);
}

}



namespace lexem
{


mnemonic code(T lex)
{
    std::map<lexem::T, mnemonic> _ = {
        { knull         ,mnemonic::knull},
        { lshift        ,mnemonic::lshift },
        { radical       ,mnemonic::radical },
        { exponent      ,mnemonic::exponent },
        { rshift        ,mnemonic::rshift },
        { decr          ,mnemonic::decr },
        { incr          ,mnemonic::incr },
        { assignadd     ,mnemonic::assignadd },
        { assignsub     ,mnemonic::assignsub },
        { assignmul     ,mnemonic::assignmul },
        { assigndiv     ,mnemonic::assigndiv },
        { assignmod     ,mnemonic::assignmod },
        { assignand     ,mnemonic::assignand },
        { assignor      ,mnemonic::assignor },
        { assignxor     ,mnemonic::assignxor },
        { assignx1      ,mnemonic::assignx1 },
        { assignlshift  ,mnemonic::assignlshift },
        { assignrshift  ,mnemonic::assignrshift },
        { lteq          ,mnemonic::leq },
        { gteq          ,mnemonic::geq },
        { eq            ,mnemonic::eq },
        { neq           ,mnemonic::neq },
        { add           ,mnemonic::add },
        { sub           ,mnemonic::sub },
        { mul           ,mnemonic::mul },
        { cppcomment    ,mnemonic::cppcomment },
        { modulo        ,mnemonic::modulo },
        { bitxor        ,mnemonic::bitxor },
        { lt            ,mnemonic::lt },
        { gt            ,mnemonic::gt },
        { assign        ,mnemonic::assign },
        { binand        ,mnemonic::binand },
        { binor         ,mnemonic::binor },
        { x1            ,mnemonic::x1 },
        { x2            ,mnemonic::x2 },
        { bitnot        ,mnemonic::bitnot },
        { booland       ,mnemonic::booland },
        { boolor        ,mnemonic::boolor },
        { openabs       ,mnemonic::openabs },
        { closeabs      ,mnemonic::closeabs },
        { openpar       ,mnemonic::openpar },
        { closepar      ,mnemonic::closepar },
        { openindex     ,mnemonic::openindex },
        { closeindex    ,mnemonic::closeindex },
        { openbrace     ,mnemonic::openbrace },
        { closebrace    ,mnemonic::closebrace },
        { bcomment      ,mnemonic::bcomment },
        { ecomment      ,mnemonic::ecomment },
        { division      ,mnemonic::division },
        { comma         ,mnemonic::comma },
        { scope         ,mnemonic::scope },
        { semicolon     ,mnemonic::semicolon },
        { colon         ,mnemonic::colon },
        { range         ,mnemonic::range },
        { factorial     ,mnemonic::factorial },
        { positive      ,mnemonic::positive },
        { negative      ,mnemonic::negative },
        { squote        ,mnemonic::squote },
        { dquote        ,mnemonic::dquote },
        { kternary      ,mnemonic::kternary },
        { hash          ,mnemonic::hash },
        { dollard       ,mnemonic::dollard },
        { dot           ,mnemonic::dot },
        { kreturn       ,mnemonic::kreturn },
        { kif           ,mnemonic::kif },
        { kpi           ,mnemonic::kpi },
        { knumber       ,mnemonic::knumber },
        { ku8           ,mnemonic::ku8 },
        { ku16          ,mnemonic::ku16 },
        { ku32          ,mnemonic::ku32 },
        { ku64          ,mnemonic::ku64 },
        { ki8           ,mnemonic::ki8 },
        { ki16          ,mnemonic::ki16 },
        { ki32          ,mnemonic::ki32 },
        { ki64          ,mnemonic::ki64 },
        { kreal         ,mnemonic::kreal },
        { kstring       ,mnemonic::kstring },
        { kthen         ,mnemonic::kthen },
        { kelse         ,mnemonic::kelse },
        { kconst        ,mnemonic::kconst },
        { kinclude      ,mnemonic::kinclude },
        { kmodule       ,mnemonic::kmodule },
        { kat           ,mnemonic::kat },
        { kprime        ,mnemonic::kprime },
        { kdo           ,mnemonic::kdo },
        { kwhile        ,mnemonic::kwhile },
        { kuntil        ,mnemonic::kuntil },
        { krepeat       ,mnemonic::krepeat },
        { kswitch       ,mnemonic::kswitch },
        { kcase         ,mnemonic::kcase },
        { ktype         ,mnemonic::ktype },
        { khex          ,mnemonic::khex },
        { kHex          ,mnemonic::kHex },
        { kcos          ,mnemonic::kcos },
        { kacos         ,mnemonic::kacos },
        { ktan          ,mnemonic::ktan },
        { katan         ,mnemonic::katan },
        { ksin          ,mnemonic::ksin },
        { kasin         ,mnemonic::kasin },
        { kobject       ,mnemonic::kobject }
    };
    for (auto i : _) {
        if (std::strcmp(i.first, lex) == 0) return i.second;
    }
    return mnemonic::knull;
}

}



token_t token_t::token_null;

const char* fmt = "'%s'(%d,%d) %s[%s]";

static token_t::list_t def_table = {
    //
    {mnemonic::knull       ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::knull           ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::lshift      ,type_t::binary      ,0x000C00000  ,opdelta::shift      ,{lexem::lshift          ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::openabs     ,type_t::openabs     ,0x080C00000  ,opdelta::par        ,{lexem::openabs         ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::closeabs    ,type_t::closeabs    ,0x080C00000  ,opdelta::par        ,{lexem::closeabs        ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::radical     ,type_t::binary      ,0x000C00000  ,opdelta::exponent   ,{lexem::radical         ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::exponent    ,type_t::binary      ,0x000C00000  ,opdelta::exponent   ,{lexem::exponent        ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::rshift      ,type_t::binary      ,0x000400000  ,opdelta::shift      ,{lexem::rshift          ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::bitxor      ,type_t::binary      ,0x000C00000  ,opdelta::bits       ,{lexem::bitxor          ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::decr        ,type_t::unary       ,0x000580000  ,opdelta::unary      ,{lexem::decr            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::incr        ,type_t::unary       ,0x000580000  ,opdelta::unary      ,{lexem::incr            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assignadd   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignadd       ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assignsub   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignsub       ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assignmul   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignmul       ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assigndiv   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assigndiv       ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assignmod   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignmod       ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assignand   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignand       ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assignor    ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignor        ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assignxor   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignxor       ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assignx1    ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignx1        ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assignlshift,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignlshift    ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assignrshift,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignrshift    ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::leq         ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::lteq            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::geq         ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::gteq            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::eq          ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::eq              ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::neq         ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::neq             ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::add         ,type_t::binary      ,0x000C00000  ,opdelta::add        ,{lexem::add             ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::sub         ,type_t::binary      ,0x000C00000  ,opdelta::add        ,{lexem::sub             ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::mul         ,type_t::binary      ,0x000C00000  ,opdelta::product    ,{lexem::mul             ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::cppcomment  ,type_t::punctuation ,0x00000      ,opdelta::noop       ,{lexem::cppcomment      ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::modulo      ,type_t::binary      ,0x000C00000  ,opdelta::product    ,{lexem::modulo          ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::lt          ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::lt              ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::gt          ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::gt              ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::assign      ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assign          ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::binand      ,type_t::binary      ,0x000C00000  ,opdelta::bits       ,{lexem::binand          ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::binor       ,type_t::binary      ,0x000C00000  ,opdelta::bits       ,{lexem::binor           ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::x2          ,type_t::prefix      ,0x000580000  ,opdelta::prefix     ,{lexem::x2              ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::x1          ,type_t::prefix      ,0x000580000  ,opdelta::prefix     ,{lexem::x1              ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::bitnot      ,type_t::unary       ,0x000580000  ,opdelta::prefix     ,{lexem::bitnot          ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::booland     ,type_t::binary      ,0x200C00000  ,opdelta::logand     ,{lexem::booland         ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::boolor      ,type_t::binary      ,0x200C00000  ,opdelta::logor      ,{lexem::boolor          ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::openpar     ,type_t::leftpar     ,0x880C00000  ,opdelta::par        ,{lexem::openpar         ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::closepar    ,type_t::closepar    ,0x1080C00000 ,opdelta::par        ,{lexem::closepar        ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::openindex   ,type_t::binary      ,0x2080C00000 ,opdelta::par        ,{lexem::openindex       ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::closeindex  ,type_t::binary      ,0x4080C00000 ,opdelta::par        ,{lexem::closeindex      ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::openbrace   ,type_t::binary      ,0x1080C00000 ,opdelta::par        ,{lexem::openbrace       ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::closebrace  ,type_t::binary      ,0x2080C00000 ,opdelta::par        ,{lexem::closebrace      ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::bcomment    ,type_t::punctuation ,0x00000      ,opdelta::noop       ,{lexem::bcomment        ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::ecomment    ,type_t::punctuation ,0x00000      ,opdelta::noop       ,{lexem::ecomment        ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::division    ,type_t::binary      ,0x000C00000  ,opdelta::product    ,{lexem::division        ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::comma       ,type_t::binary      ,0x080C00000  ,opdelta::comma      ,{lexem::comma           ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::scope       ,type_t::binary      ,0x080C00000  ,opdelta::scope      ,{lexem::scope           ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::semicolon   ,type_t::punctuation ,0x080000000  ,opdelta::noop       ,{lexem::semicolon       ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::colon       ,type_t::binary      ,0x000C00000  ,opdelta::noop       ,{lexem::colon           ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::range       ,type_t::binary      ,0x080C00000  ,opdelta::scope      ,{lexem::range           ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::factorial   ,type_t::postfix     ,0x000580000  ,opdelta::unary      ,{lexem::factorial       ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::positive    ,type_t::prefix      ,0x000580020  ,opdelta::unary      ,{lexem::positive        ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::negative    ,type_t::prefix      ,0x000580020  ,opdelta::unary      ,{lexem::negative        ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::squote      ,type_t::text        ,0x00000      ,opdelta::noop       ,{lexem::squote          ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::dquote      ,type_t::text        ,0x00000      ,opdelta::noop       ,{lexem::dquote          ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kternary    ,type_t::keyword     ,0x000C40000  ,opdelta::unary      ,{lexem::kternary        ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::hash        ,type_t::prefix      ,0x000580000  ,opdelta::unary      ,{lexem::hash            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::dollard     ,type_t::prefix      ,0x000580000  ,opdelta::unary      ,{lexem::dollard         ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::dot         ,type_t::binary      ,0x080C00000  ,opdelta::scope      ,{lexem::dot             ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::kreturn     ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kreturn         ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kif         ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kif             ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kpi         ,type_t::keyword     ,0x000040011  ,opdelta::identifier ,{lexem::kpi             ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::knumber     ,type_t::prefix      ,0x000540001  ,opdelta::identifier ,{lexem::knumber         ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::ku8         ,type_t::prefix      ,0x000540400  ,opdelta::identifier ,{lexem::ku8             ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::ku16        ,type_t::prefix      ,0x000540800  ,opdelta::identifier ,{lexem::ku16            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::ku32        ,type_t::prefix      ,0x000541000  ,opdelta::identifier ,{lexem::ku32            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::ku64        ,type_t::prefix      ,0x000542000  ,opdelta::identifier ,{lexem::ku64            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::ki8         ,type_t::prefix      ,0x000540040  ,opdelta::identifier ,{lexem::ki8             ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::ki16        ,type_t::prefix      ,0x000540080  ,opdelta::identifier ,{lexem::ki16            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::ki32        ,type_t::prefix      ,0x000540100  ,opdelta::identifier ,{lexem::ki32            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::ki64        ,type_t::prefix      ,0x000540200  ,opdelta::identifier ,{lexem::ki64            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::kreal       ,type_t::prefix      ,0x000544000  ,opdelta::identifier ,{lexem::kreal           ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::kstring     ,type_t::prefix      ,0x000540002  ,opdelta::identifier ,{lexem::kstring         ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::kthen       ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kthen           ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kelse       ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kelse           ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kconst      ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kconst          ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kinclude    ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kinclude        ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kmodule     ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kmodule         ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kat         ,type_t::prefix      ,0x000580000  ,opdelta::unary      ,{lexem::kat             ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::kprime      ,type_t::prefix      ,0x000580000  ,opdelta::unary      ,{lexem::kprime          ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::kdo         ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::kdo             ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kwhile      ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::kwhile          ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kfor        ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::kfor            ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kuntil      ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::kuntil          ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::krepeat     ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::krepeat         ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kswitch     ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::kswitch         ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::kcase       ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::kcase           ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::ktype       ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::ktype           ,0,0,0,-1} ,{0,0} ,0},
    {mnemonic::khex        ,type_t::hex         ,0x000040011  ,opdelta::noop       ,{lexem::khex            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::kHex        ,type_t::hex         ,0x000040011  ,opdelta::noop       ,{lexem::kHex            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::kcos        ,type_t::prefix      ,0x000580000  ,opdelta::unary      ,{lexem::kcos            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::kacos       ,type_t::prefix      ,0x0005C0000  ,opdelta::unary      ,{lexem::kacos           ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::ktan        ,type_t::prefix      ,0x0005C0000  ,opdelta::unary      ,{lexem::ktan            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::katan       ,type_t::prefix      ,0x0005C0000  ,opdelta::unary      ,{lexem::katan           ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::ksin        ,type_t::prefix      ,0x0005C0000  ,opdelta::unary      ,{lexem::ksin            ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::kasin       ,type_t::prefix      ,0x0005C0000  ,opdelta::unary      ,{lexem::kasin           ,0,0,0,-1} ,{1,0} ,0},
    {mnemonic::kobject     ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kobject         ,0,0,0,-1}, {0,0}, 0},
    {mnemonic::kstatic     ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kstatic         ,0,0,0,-1}, {0,0}, 0},
    {mnemonic::kme         ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kme             ,0,0,0,-1}, {1,0}, 0}
};


token_t::token_t()
{
}

token_t::token_t(mnemonic a_code, type_t::T a_type, type_t::T a_sem, opdelta::T a_delta, token_t::src a_src, token_t::flags a_flags, void * a_data)
{
    code = a_code;
    type = a_type;
    sem = a_sem;
    delta = a_delta;
    loc = a_src;
    f = a_flags;
    data = a_data;
}


token_t::token_t(const token_t & a_token)
{
    code = a_token.code;
    type = a_token.type;
    sem = a_token.sem;
    delta = a_token.delta;
    data = a_token.data;
    loc = a_token.loc;
    f = a_token.f;
}

token_t::token_t(token_t && a_token)
{
    using std::swap;
    swap(code, a_token.code);
    swap(type, a_token.type);
    swap(sem, a_token.sem);
    swap(delta, a_token.delta);
    swap(data, a_token.data);
    swap(loc, a_token.loc);
    swap(f, a_token.f);
}

token_t::~token_t()
{
}

token_t & token_t::operator=(token_t && a_token)
{
    using std::swap;
    swap(code, a_token.code);
    swap(type, a_token.type);
    swap(sem, a_token.sem);
    swap(delta, a_token.delta);
    swap(data, a_token.data);
    swap(loc, a_token.loc);
    swap(f, a_token.f);
    return *this;
}


token_t & token_t::operator=(const token_t & a_token)
{
    code = a_token.code;
    type = a_token.type;
    sem = a_token.sem;
    delta = a_token.delta;
    data = a_token.data;
    loc = a_token.loc;
    f = a_token.f;
    return *this;
}




/*
std::string lexer_t::cursor_t::line()
{
    std::string str;
    int l = 1;
    const char* cc = c;
    // localiser le debut de la ligne;
    while (*cc && (cc > b) && (*cc != '\n') && (*cc != '\r')) --cc;
    // debut de la ligne ou de la source:
    if (cc >= b) {
        if ((*cc == '\n') || (*cc == '\r')) ++cc;
        while (*cc && (*cc != '\n') && (*cc != '\r')) str += *cc++;
        return str;
    }
    return std::string(b);
}



std::string lexer_t::cursor_t::mark()
{
    std::stringstream str;

    str << line() << '\n' << std::setw(col) << '^';
    return str.str();
}

*/
std::string token_t::mark()
{
    const char* b = loc.b - loc.p;
    std::string str;
    int l = 1;
    const char* cc = loc.b;
    // localiser le debut de la ligne;
    while (*cc && (cc > b) && (*cc != '\n') && (*cc != '\r')) --cc;
    // debut de la ligne ou de la source:
    if (cc >= b) {
        if ((*cc == '\n') || (*cc == '\r')) ++cc;
        while (*cc && (*cc != '\n') && (*cc != '\r')) str += *cc++;
        //return str;
    }
    //std::stringstream ostr;

    string_t tstr;
    tstr << str << '\n' << std::setw(loc.c) << '^';
    return tstr();
}

token_t::operator bool() const
{
    return (code != mnemonic::knull) && (type != type_t::null);
}

bool token_t::is_binary()
{   
    return sem & type_t::binary;
}

bool token_t::is_unary()
{
    return sem & type_t::unary;
}

bool token_t::is_identifier()
{
    return sem & type_t::id;
}

bool token_t::is_leaf()
{
    return sem & type_t::leaf;
}

bool token_t::is_operator()
{
    return sem & (type_t::binary | type_t::unary);
}

bool token_t::is_closingpair()
{
    return type & (type_t::closeabs | type_t::closepar);
}

bool token_t::is_punctuation()
{
    return sem & type_t::punctuation;
}

bool token_t::is_number()
{
    return sem & type_t::number;
}

bool token_t::is_hexadecimal()
{
    return type & type_t::hex;
}

bool token_t::is_prefix()
{
    return type & type_t::prefix;
}

bool token_t::is_postfix()
{
    return type & type_t::postfix;
}

bool token_t::is_instruction()
{
    return !(is_operator() | is_leaf());
}

std::string token_t::attribute()
{
    if (loc.b == nullptr)
        return std::string();

    std::string str;
    const char* ptr = loc.b;

    str += *ptr++;
    if (!loc.e) {
        while (*ptr) str += *ptr++;
        return str;
    }
    while (ptr <= loc.e) str += *ptr++;
    return str;
}

std::string token_t::informations()
{
    if (!(*this)) return "Start (hidden source text)";
    std::string str = fmt;
    char buf[128];

    std::snprintf(buf, 90, fmt, (f.m ? "< * >" : attribute().c_str()), loc.c, loc.l, type_t::name(type).c_str(), type_t::name(sem).c_str());
    return buf;
}



token_t token_t::scan(const char* src)
{
    int unicode = 0;
    int index = 0;

    for (auto token : def_table) {
        const char* crs = src;
        const char* rtxt = token.loc.b;
        std::size_t sz = std::strlen(rtxt);
        if (*crs != *token.loc.b) {
            ++index;
            continue;
        }
        ++index;

        while ((*crs  && *rtxt) && (*crs == *rtxt)) {
            //std::cout << *crs <<  *rtxt << ">>>";
            if (*crs < 0)
                ++unicode;
            ++crs; ++rtxt;
            //std::cout << *crs << *rtxt << ',';
        }

        if (*rtxt == 0) {
            //std::cout << "\nDebug Trace - TokenUnit::Scan: rtxt=[" << token.Loc.B << "]\n";
            if ((*crs && (isalnum(*(crs)))) && (token.sem & type_t::keyword) && !(token.sem & type_t::oper) && !(token.type == type_t::hex) )
                continue;

            token.loc.b = src;
            token.loc.e = token.loc.b + sz - 1;
            //--------------------------------------------------------
            return token;
        }
    }
    return token_t::token_null;
}

token_t token_t::query(type_t::T ty)
{
    for (auto token : def_table) {
        if (ty & token.sem) return token;
    }

    return token_t::token_null;
}

token_t token_t::query(lexem::T lex)
{
    for (auto token : def_table) {
        if (std::strncmp(lex, token.loc.b,25) == 0) return token;
    }

    return token_t::token_null;
}


token_t token_t::query(mnemonic a_code)
{
    for (auto token : def_table) {
        if (a_code == token.code) return token;
    }
    return token_t::token_null;
}

mnemonic token_t::strtomnemonic(const std::string& k)
{
    std::map<lexem::T, mnemonic> _=
    {
        { lexem::knull            ,mnemonic::knull  },
        { lexem::lshift           ,mnemonic::lshift },
        { lexem::radical          ,mnemonic::radical },
        { lexem::exponent         ,mnemonic::exponent },
        { lexem::rshift           ,mnemonic::rshift },
        { lexem::decr             ,mnemonic::decr },
        { lexem::incr             ,mnemonic::incr },
        { lexem::assignadd        ,mnemonic::assignadd },
        { lexem::assignsub        ,mnemonic::assignsub },
        { lexem::assignmul        ,mnemonic::assignmul },
        { lexem::assigndiv        ,mnemonic::assigndiv },
        { lexem::assignmod        ,mnemonic::assignmod },
        { lexem::assignand        ,mnemonic::assignand },
        { lexem::assignor         ,mnemonic::assignor },
        { lexem::assignxor        ,mnemonic::assignxor },
        { lexem::assignx1         ,mnemonic::assignx1 },
        { lexem::assignlshift     ,mnemonic::assignlshift },
        { lexem::assignrshift     ,mnemonic::assignrshift },
        { lexem::lteq              ,mnemonic::leq },
        { lexem::gteq              ,mnemonic::geq },
        { lexem::eq               ,mnemonic::eq },
        { lexem::neq              ,mnemonic::neq },
        { lexem::add              ,mnemonic::add },
        { lexem::sub              ,mnemonic::sub },
        { lexem::mul              ,mnemonic::mul },
        { lexem::cppcomment       ,mnemonic::cppcomment },
        { lexem::modulo           ,mnemonic::modulo },
        { lexem::lt               ,mnemonic::bitxor },
        { lexem::gt               ,mnemonic::lt },
        { lexem::assign           ,mnemonic::gt },
        { lexem::binand           ,mnemonic::assign },
        { lexem::binor            ,mnemonic::binand },
        { lexem::bitxor           ,mnemonic::binor },
        { lexem::x1               ,mnemonic::x1 },
        { lexem::x2               ,mnemonic::x2 },
        { lexem::bitnot           ,mnemonic::bitnot },
        { lexem::booland          ,mnemonic::booland },
        { lexem::boolor           ,mnemonic::boolor },
        { lexem::openabs          ,mnemonic::openabs },
        { lexem::closeabs         ,mnemonic::closeabs },
        { lexem::openpar          ,mnemonic::openpar },
        { lexem::closepar         ,mnemonic::closepar },
        { lexem::openindex        ,mnemonic::openindex },
        { lexem::closeindex       ,mnemonic::closeindex },
        { lexem::openbrace        ,mnemonic::openbrace },
        { lexem::closebrace       ,mnemonic::closebrace },
        { lexem::bcomment         ,mnemonic::bcomment },
        { lexem::ecomment         ,mnemonic::ecomment },
        { lexem::division         ,mnemonic::division },
        { lexem::comma            ,mnemonic::comma },
        { lexem::scope            ,mnemonic::scope },
        { lexem::semicolon        ,mnemonic::semicolon },
        { lexem::colon            ,mnemonic::colon },
        { lexem::range            ,mnemonic::range },
        { lexem::factorial        ,mnemonic::factorial },
        { lexem::positive         ,mnemonic::positive },
        { lexem::negative         ,mnemonic::negative },
        { lexem::squote           ,mnemonic::squote },
        { lexem::dquote           ,mnemonic::dquote },
        { lexem::kternary         ,mnemonic::kternary },
        { lexem::hash             ,mnemonic::hash },
        { lexem::dollard          ,mnemonic::dollard },
        { lexem::dot              ,mnemonic::dot },
        { lexem::kreturn          ,mnemonic::kreturn },
        { lexem::kif              ,mnemonic::kif },
        { lexem::kpi              ,mnemonic::kpi },
        { lexem::knumber          ,mnemonic::knumber },
        { lexem::ku8              ,mnemonic::ku8 },
        { lexem::ku16             ,mnemonic::ku16 },
        { lexem::ku32             ,mnemonic::ku32 },
        { lexem::ku64             ,mnemonic::ku64 },
        { lexem::ki8              ,mnemonic::ki8 },
        { lexem::ki16             ,mnemonic::ki16 },
        { lexem::ki32             ,mnemonic::ki32 },
        { lexem::ki64             ,mnemonic::ki64 },
        { lexem::kreal            ,mnemonic::kreal },
        { lexem::kstring          ,mnemonic::kstring },
        { lexem::kthen            ,mnemonic::kthen },
        { lexem::kelse            ,mnemonic::kelse },
        { lexem::kconst           ,mnemonic::kconst },
        { lexem::kinclude         ,mnemonic::kinclude },
        { lexem::kmodule          ,mnemonic::kmodule },
        { lexem::kat              ,mnemonic::kat },
        { lexem::kprime           ,mnemonic::kprime },
        { lexem::kdo              ,mnemonic::kdo },
        { lexem::kwhile           ,mnemonic::kwhile },
        { lexem::kfor             ,mnemonic::kfor },
        { lexem::kuntil           ,mnemonic::kuntil },
        { lexem::krepeat          ,mnemonic::krepeat },
        { lexem::kswitch          ,mnemonic::kswitch },
        { lexem::kcase            ,mnemonic::kcase },
        { lexem::ktype            ,mnemonic::ktype },
        { lexem::khex             ,mnemonic::khex },
        { lexem::kHex             ,mnemonic::kHex },
        { lexem::kcos             ,mnemonic::kcos },
        { lexem::kacos            ,mnemonic::kacos },
        { lexem::ktan             ,mnemonic::ktan },
        { lexem::katan            ,mnemonic::katan },
        { lexem::ksin             ,mnemonic::ksin },
        { lexem::kasin            ,mnemonic::kasin },
        { lexem::kobject          ,mnemonic::kobject },
        { lexem::kstatic          ,mnemonic::kstatic },
        { lexem::kme              ,mnemonic::kme },
        { "noop"                  ,mnemonic::noop }
    };

    return _[k.c_str()];
}



}


