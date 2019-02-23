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

T xio_api type_name(const std::string & a_names)
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


e_code code(T lex)
{
    std::map<lexem::T, e_code> _ = {
        { knull         ,e_code::knull},
        { lshift        ,e_code::lshift },
        { radical       ,e_code::radical },
        { exponent      ,e_code::exponent },
        { rshift        ,e_code::rshift },
        { decr          ,e_code::decr },
        { incr          ,e_code::incr },
        { assignadd     ,e_code::assignadd },
        { assignsub     ,e_code::assignsub },
        { assignmul     ,e_code::assignmul },
        { assigndiv     ,e_code::assigndiv },
        { assignmod     ,e_code::assignmod },
        { assignand     ,e_code::assignand },
        { assignor      ,e_code::assignor },
        { assignxor     ,e_code::assignxor },
        { assignx1      ,e_code::assignx1 },
        { assignlshift  ,e_code::assignlshift },
        { assignrshift  ,e_code::assignrshift },
        { lteq          ,e_code::leq },
        { gteq          ,e_code::geq },
        { eq            ,e_code::eq },
        { neq           ,e_code::neq },
        { add           ,e_code::add },
        { sub           ,e_code::sub },
        { mul           ,e_code::mul },
        { cppcomment    ,e_code::cppcomment },
        { modulo        ,e_code::modulo },
        { bitxor        ,e_code::bitxor },
        { lt            ,e_code::lt },
        { gt            ,e_code::gt },
        { assign        ,e_code::assign },
        { binand        ,e_code::binand },
        { binor         ,e_code::binor },
        { x1            ,e_code::x1 },
        { x2            ,e_code::x2 },
        { bitnot        ,e_code::bitnot },
        { booland       ,e_code::booland },
        { boolor        ,e_code::boolor },
        { openabs       ,e_code::openabs },
        { closeabs      ,e_code::closeabs },
        { openpar       ,e_code::openpar },
        { closepar      ,e_code::closepar },
        { openindex     ,e_code::openindex },
        { closeindex    ,e_code::closeindex },
        { openbrace     ,e_code::openbrace },
        { closebrace    ,e_code::closebrace },
        { bcomment      ,e_code::bcomment },
        { ecomment      ,e_code::ecomment },
        { division      ,e_code::division },
        { comma         ,e_code::comma },
        { scope         ,e_code::scope },
        { semicolon     ,e_code::semicolon },
        { colon         ,e_code::colon },
        { range         ,e_code::range },
        { factorial     ,e_code::factorial },
        { positive      ,e_code::positive },
        { negative      ,e_code::negative },
        { squote        ,e_code::squote },
        { dquote        ,e_code::dquote },
        { kternary      ,e_code::kternary },
        { hash          ,e_code::hash },
        { dollard       ,e_code::dollard },
        { dot           ,e_code::dot },
        { kreturn       ,e_code::kreturn },
        { kif           ,e_code::kif },
        { kpi           ,e_code::kpi },
        { knumber       ,e_code::knumber },
        { ku8           ,e_code::ku8 },
        { ku16          ,e_code::ku16 },
        { ku32          ,e_code::ku32 },
        { ku64          ,e_code::ku64 },
        { ki8           ,e_code::ki8 },
        { ki16          ,e_code::ki16 },
        { ki32          ,e_code::ki32 },
        { ki64          ,e_code::ki64 },
        { kreal         ,e_code::kreal },
        { kstring       ,e_code::kstring },
        { kthen         ,e_code::kthen },
        { kelse         ,e_code::kelse },
        { kconst        ,e_code::kconst },
        { kinclude      ,e_code::kinclude },
        { kmodule       ,e_code::kmodule },
        { kat           ,e_code::kat },
        { kprime        ,e_code::kprime },
        { kdo           ,e_code::kdo },
        { kwhile        ,e_code::kwhile },
        { kuntil        ,e_code::kuntil },
        { krepeat       ,e_code::krepeat },
        { kswitch       ,e_code::kswitch },
        { kcase         ,e_code::kcase },
        { ktype         ,e_code::ktype },
        { khex          ,e_code::khex },
        { kHex          ,e_code::kHex },
        { kcos          ,e_code::kcos },
        { kacos         ,e_code::kacos },
        { ktan          ,e_code::ktan },
        { katan         ,e_code::katan },
        { ksin          ,e_code::ksin },
        { kasin         ,e_code::kasin },
        { kobject       ,e_code::kobject }
    };
    for (auto i : _) {
        if (std::strcmp(i.first, lex) == 0) return i.second;
    }
    return e_code::knull;
}

}



token_t token_t::token_null;

const char* fmt = "'%s'(%d,%d) %s[%s]";

static token_t::list_t def_table = {
    //
    {e_code::knull       ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::knull           ,0,0,0,-1} ,{1,0} ,0},
    {e_code::lshift      ,type_t::binary      ,0x000C00000  ,opdelta::shift      ,{lexem::lshift          ,0,0,0,-1} ,{1,0} ,0},
    {e_code::openabs     ,type_t::openabs     ,0x080C00000  ,opdelta::par        ,{lexem::openabs         ,0,0,0,-1} ,{1,0} ,0},
    {e_code::closeabs    ,type_t::closeabs    ,0x080C00000  ,opdelta::par        ,{lexem::closeabs        ,0,0,0,-1} ,{1,0} ,0},
    {e_code::radical     ,type_t::binary      ,0x000C00000  ,opdelta::exponent   ,{lexem::radical         ,0,0,0,-1} ,{1,0} ,0},
    {e_code::exponent    ,type_t::binary      ,0x000C00000  ,opdelta::exponent   ,{lexem::exponent        ,0,0,0,-1} ,{1,0} ,0},
    {e_code::rshift      ,type_t::binary      ,0x000400000  ,opdelta::shift      ,{lexem::rshift          ,0,0,0,-1} ,{1,0} ,0},
    {e_code::bitxor      ,type_t::binary      ,0x000C00000  ,opdelta::bits       ,{lexem::bitxor          ,0,0,0,-1} ,{1,0} ,0},
    {e_code::decr        ,type_t::unary       ,0x000580000  ,opdelta::unary      ,{lexem::decr            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::incr        ,type_t::unary       ,0x000580000  ,opdelta::unary      ,{lexem::incr            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assignadd   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignadd       ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assignsub   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignsub       ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assignmul   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignmul       ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assigndiv   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assigndiv       ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assignmod   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignmod       ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assignand   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignand       ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assignor    ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignor        ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assignxor   ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignxor       ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assignx1    ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignx1        ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assignlshift,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignlshift    ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assignrshift,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assignrshift    ,0,0,0,-1} ,{1,0} ,0},
    {e_code::leq         ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::lteq            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::geq         ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::gteq            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::eq          ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::eq              ,0,0,0,-1} ,{1,0} ,0},
    {e_code::neq         ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::neq             ,0,0,0,-1} ,{1,0} ,0},
    {e_code::add         ,type_t::binary      ,0x000C00000  ,opdelta::add        ,{lexem::add             ,0,0,0,-1} ,{1,0} ,0},
    {e_code::sub         ,type_t::binary      ,0x000C00000  ,opdelta::add        ,{lexem::sub             ,0,0,0,-1} ,{1,0} ,0},
    {e_code::mul         ,type_t::binary      ,0x000C00000  ,opdelta::product    ,{lexem::mul             ,0,0,0,-1} ,{1,0} ,0},
    {e_code::cppcomment  ,type_t::punctuation ,0x00000      ,opdelta::noop       ,{lexem::cppcomment      ,0,0,0,-1} ,{0,0} ,0},
    {e_code::modulo      ,type_t::binary      ,0x000C00000  ,opdelta::product    ,{lexem::modulo          ,0,0,0,-1} ,{1,0} ,0},
    {e_code::lt          ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::lt              ,0,0,0,-1} ,{1,0} ,0},
    {e_code::gt          ,type_t::binary      ,0x200C00000  ,opdelta::less       ,{lexem::gt              ,0,0,0,-1} ,{1,0} ,0},
    {e_code::assign      ,type_t::assign      ,0x100C40000  ,opdelta::assign     ,{lexem::assign          ,0,0,0,-1} ,{1,0} ,0},
    {e_code::binand      ,type_t::binary      ,0x000C00000  ,opdelta::bits       ,{lexem::binand          ,0,0,0,-1} ,{1,0} ,0},
    {e_code::binor       ,type_t::binary      ,0x000C00000  ,opdelta::bits       ,{lexem::binor           ,0,0,0,-1} ,{1,0} ,0},
    {e_code::x2          ,type_t::prefix      ,0x000580000  ,opdelta::prefix     ,{lexem::x2              ,0,0,0,-1} ,{1,0} ,0},
    {e_code::x1          ,type_t::prefix      ,0x000580000  ,opdelta::prefix     ,{lexem::x1              ,0,0,0,-1} ,{1,0} ,0},
    {e_code::bitnot      ,type_t::unary       ,0x000580000  ,opdelta::prefix     ,{lexem::bitnot          ,0,0,0,-1} ,{1,0} ,0},
    {e_code::booland     ,type_t::binary      ,0x200C00000  ,opdelta::logand     ,{lexem::booland         ,0,0,0,-1} ,{1,0} ,0},
    {e_code::boolor      ,type_t::binary      ,0x200C00000  ,opdelta::logor      ,{lexem::boolor          ,0,0,0,-1} ,{1,0} ,0},
    {e_code::openpar     ,type_t::leftpar     ,0x880C00000  ,opdelta::par        ,{lexem::openpar         ,0,0,0,-1} ,{1,0} ,0},
    {e_code::closepar    ,type_t::closepar    ,0x1080C00000 ,opdelta::par        ,{lexem::closepar        ,0,0,0,-1} ,{1,0} ,0},
    {e_code::openindex   ,type_t::binary      ,0x2080C00000 ,opdelta::par        ,{lexem::openindex       ,0,0,0,-1} ,{1,0} ,0},
    {e_code::closeindex  ,type_t::binary      ,0x4080C00000 ,opdelta::par        ,{lexem::closeindex      ,0,0,0,-1} ,{1,0} ,0},
    {e_code::openbrace   ,type_t::binary      ,0x1080C00000 ,opdelta::par        ,{lexem::openbrace       ,0,0,0,-1} ,{1,0} ,0},
    {e_code::closebrace  ,type_t::binary      ,0x2080C00000 ,opdelta::par        ,{lexem::closebrace      ,0,0,0,-1} ,{1,0} ,0},
    {e_code::bcomment    ,type_t::punctuation ,0x00000      ,opdelta::noop       ,{lexem::bcomment        ,0,0,0,-1} ,{0,0} ,0},
    {e_code::ecomment    ,type_t::punctuation ,0x00000      ,opdelta::noop       ,{lexem::ecomment        ,0,0,0,-1} ,{0,0} ,0},
    {e_code::division    ,type_t::binary      ,0x000C00000  ,opdelta::product    ,{lexem::division        ,0,0,0,-1} ,{1,0} ,0},
    {e_code::comma       ,type_t::binary      ,0x080C00000  ,opdelta::comma      ,{lexem::comma           ,0,0,0,-1} ,{1,0} ,0},
    {e_code::scope       ,type_t::binary      ,0x080C00000  ,opdelta::scope      ,{lexem::scope           ,0,0,0,-1} ,{1,0} ,0},
    {e_code::semicolon   ,type_t::punctuation ,0x080000000  ,opdelta::noop       ,{lexem::semicolon       ,0,0,0,-1} ,{0,0} ,0},
    {e_code::colon       ,type_t::binary      ,0x000C00000  ,opdelta::noop       ,{lexem::colon           ,0,0,0,-1} ,{0,0} ,0},
    {e_code::range       ,type_t::binary      ,0x080C00000  ,opdelta::scope      ,{lexem::range           ,0,0,0,-1} ,{1,0} ,0},
    {e_code::factorial   ,type_t::postfix     ,0x000580000  ,opdelta::unary      ,{lexem::factorial       ,0,0,0,-1} ,{1,0} ,0},
    {e_code::positive    ,type_t::prefix      ,0x000580020  ,opdelta::unary      ,{lexem::positive        ,0,0,0,-1} ,{1,0} ,0},
    {e_code::negative    ,type_t::prefix      ,0x000580020  ,opdelta::unary      ,{lexem::negative        ,0,0,0,-1} ,{1,0} ,0},
    {e_code::squote      ,type_t::text        ,0x00000      ,opdelta::noop       ,{lexem::squote          ,0,0,0,-1} ,{0,0} ,0},
    {e_code::dquote      ,type_t::text        ,0x00000      ,opdelta::noop       ,{lexem::dquote          ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kternary    ,type_t::keyword     ,0x000C40000  ,opdelta::unary      ,{lexem::kternary        ,0,0,0,-1} ,{1,0} ,0},
    {e_code::hash        ,type_t::prefix      ,0x000580000  ,opdelta::unary      ,{lexem::hash            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::dollard     ,type_t::prefix      ,0x000580000  ,opdelta::unary      ,{lexem::dollard         ,0,0,0,-1} ,{1,0} ,0},
    {e_code::dot         ,type_t::binary      ,0x080C00000  ,opdelta::scope      ,{lexem::dot             ,0,0,0,-1} ,{1,0} ,0},
    {e_code::kreturn     ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kreturn         ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kif         ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kif             ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kpi         ,type_t::keyword     ,0x000040011  ,opdelta::identifier ,{lexem::kpi             ,0,0,0,-1} ,{1,0} ,0},
    {e_code::knumber     ,type_t::prefix      ,0x000540001  ,opdelta::identifier ,{lexem::knumber         ,0,0,0,-1} ,{1,0} ,0},
    {e_code::ku8         ,type_t::prefix      ,0x000540400  ,opdelta::identifier ,{lexem::ku8             ,0,0,0,-1} ,{1,0} ,0},
    {e_code::ku16        ,type_t::prefix      ,0x000540800  ,opdelta::identifier ,{lexem::ku16            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::ku32        ,type_t::prefix      ,0x000541000  ,opdelta::identifier ,{lexem::ku32            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::ku64        ,type_t::prefix      ,0x000542000  ,opdelta::identifier ,{lexem::ku64            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::ki8         ,type_t::prefix      ,0x000540040  ,opdelta::identifier ,{lexem::ki8             ,0,0,0,-1} ,{1,0} ,0},
    {e_code::ki16        ,type_t::prefix      ,0x000540080  ,opdelta::identifier ,{lexem::ki16            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::ki32        ,type_t::prefix      ,0x000540100  ,opdelta::identifier ,{lexem::ki32            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::ki64        ,type_t::prefix      ,0x000540200  ,opdelta::identifier ,{lexem::ki64            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::kreal       ,type_t::prefix      ,0x000544000  ,opdelta::identifier ,{lexem::kreal           ,0,0,0,-1} ,{1,0} ,0},
    {e_code::kstring     ,type_t::prefix      ,0x000540002  ,opdelta::identifier ,{lexem::kstring         ,0,0,0,-1} ,{1,0} ,0},
    {e_code::kthen       ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kthen           ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kelse       ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kelse           ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kconst      ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kconst          ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kinclude    ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kinclude        ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kmodule     ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kmodule         ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kat         ,type_t::prefix      ,0x000580000  ,opdelta::unary      ,{lexem::kat             ,0,0,0,-1} ,{1,0} ,0},
    {e_code::kprime      ,type_t::prefix      ,0x000580000  ,opdelta::unary      ,{lexem::kprime          ,0,0,0,-1} ,{1,0} ,0},
    {e_code::kdo         ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::kdo             ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kwhile      ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::kwhile          ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kuntil      ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::kuntil          ,0,0,0,-1} ,{0,0} ,0},
    {e_code::krepeat     ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::krepeat         ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kswitch     ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::kswitch         ,0,0,0,-1} ,{0,0} ,0},
    {e_code::kcase       ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::kcase           ,0,0,0,-1} ,{0,0} ,0},
    {e_code::ktype       ,type_t::keyword     ,0x000040000  ,opdelta::noop       ,{lexem::ktype           ,0,0,0,-1} ,{0,0} ,0},
    {e_code::khex        ,type_t::hex         ,0x000040011  ,opdelta::noop       ,{lexem::khex            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::kHex        ,type_t::hex         ,0x000040011  ,opdelta::noop       ,{lexem::kHex            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::kcos        ,type_t::prefix      ,0x000580000  ,opdelta::unary      ,{lexem::kcos            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::kacos       ,type_t::prefix      ,0x0005C0000  ,opdelta::unary      ,{lexem::kacos           ,0,0,0,-1} ,{1,0} ,0},
    {e_code::ktan        ,type_t::prefix      ,0x0005C0000  ,opdelta::unary      ,{lexem::ktan            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::katan       ,type_t::prefix      ,0x0005C0000  ,opdelta::unary      ,{lexem::katan           ,0,0,0,-1} ,{1,0} ,0},
    {e_code::ksin        ,type_t::prefix      ,0x0005C0000  ,opdelta::unary      ,{lexem::ksin            ,0,0,0,-1} ,{1,0} ,0},
    {e_code::kasin       ,type_t::prefix      ,0x0005C0000  ,opdelta::unary      ,{lexem::kasin           ,0,0,0,-1} ,{1,0} ,0},
    {e_code::kobject     ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kobject         ,0,0,0,-1}, {0,0}, 0},
    {e_code::kstatic     ,type_t::keyword     ,0x000040000  ,opdelta::identifier ,{lexem::kstatic         ,0,0,0,-1}, {0,0}, 0}
};


token_t::token_t()
{
}

token_t::token_t(e_code a_code, type_t::T a_type, type_t::T a_sem, opdelta::T a_delta, token_t::src a_src, token_t::flags a_flags, void * a_data)
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
        return str;
    }
    std::stringstream ostr;

    ostr << str << '\n' << std::setw(loc.c) << '^';
    return ostr.str();
}

token_t::operator bool() const
{
    return (code != e_code::knull) && (type != type_t::null);
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


token_t token_t::query(e_code a_code)
{
    for (auto token : def_table) {
        if (a_code == token.code) return token;
    }
    return token_t::token_null;
}



}


