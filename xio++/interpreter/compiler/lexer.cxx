#include "lexer.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <xio++/journal/logger.hpp>


namespace xio {


// 
//     {type_t::number      ,&lexer_t::scan_number      },
//     {type_t::text        ,&lexer_t::scan_string      },
//     {type_t::keyword     ,&lexer_t::scan_keyword     },
//     {type_t::unary       ,&lexer_t::scan_unary       },
//     {type_t::prefix      ,&lexer_t::scan_prefix      }, // -- (-a)
//     {type_t::postfix     ,&lexer_t::scan_postfix     },
//     {type_t::binary      ,&lexer_t::scan_binary      },
//     {type_t::leftpar     ,&lexer_t::scan_binary      },
//     {type_t::punctuation ,&lexer_t::scan_hex         }
// };



void lexer_t::init_scanners()
{
    if (scanners.empty()) {
        scanners[type_t::number]      = &lexer_t::scan_number;
        scanners[type_t::text]        = &lexer_t::scan_string;
        scanners[type_t::keyword]     = &lexer_t::scan_keyword;
        scanners[type_t::unary]       = &lexer_t::scan_unary;
        scanners[type_t::prefix]      = &lexer_t::scan_prefix;
        scanners[type_t::postfix]     = &lexer_t::scan_postfix;
        scanners[type_t::binary]      = &lexer_t::scan_binary;
        scanners[type_t::leftpar]     = &lexer_t::scan_leftpar;
        scanners[type_t::closepar]    = &lexer_t::scan_binary;
        scanners[type_t::openabs]     = &lexer_t::scan_binary;
        scanners[type_t::closeabs]    = &lexer_t::scan_binary;
        scanners[type_t::assign]      = &lexer_t::scan_binary;
        scanners[type_t::hex]         = &lexer_t::scan_hex;
        scanners[type_t::assign]      = &lexer_t::scan_assign_rs;
        scanners[type_t::punctuation] = &lexer_t::scan_punctuation;
    }


    //int a, b, c;
    //a = a * (b = 3);
}


/*

            =
           / \
          a   *
             / \
            a   = 
               / \
              b   3

*/

std::string lexer_t::mark(token_t & token)
{
    cursor.c = token.loc.b;


    return std::string();
}

lexer_t::result lexer_t::scan_number(xio::token_t & Token)
{
    const char* bc = cursor.c;
    bool real = false;

    if ((!isdigit(*bc)) && (*bc != '.'))
        return { };

    while ((bc <= cursor.e) && isdigit(*bc)) ++bc;

    if ((*bc == 'x') || (*bc == 'X')) return { };

    if (*bc == '.') {
        if (!real) {
            real = true;

            ++bc;
            if (*bc == '.') {
                --bc;
                goto done_number;
            }
            if (!isdigit(*bc)) goto bail;
        }
        else {
        bail:
            return {};
        }
    }
    while ((bc <= cursor.e) && isdigit(*bc)) ++bc;
    if(bc<=cursor.b) return {};
    --bc;
done_number:
    //
    //    String number;
    //    for(const char* i = _cursor.c; i< bc; i++)  number << *i;
    if (!empty()) {
        token_t vt = tokens->back();
        if (vt.is_leaf()) {
            return {
                (
                    message::push(message::xclass::error),
                    "[expression pre-parsing: in scan_number]",
                    cursor.location(),
                    "Unexpected numeric constant:\n"+cursor.mark()
                )
            };
        }
    }
    Token = {
        mnemonic::noop, type_t::number, type_t::number | type_t::leaf | (real ? type_t::real : type_t::u64), // u64: Par defaut. Mais les parsers pourront "squizer" en evaluant la valeur et la comparer avec les limtes u8, u/i16;32;64...
        opdelta::identifier,
        {cursor.c, bc,cursor.l,cursor.col, static_cast<int>(cursor.c - cursor.b)},
        {1,0},
        0
    };

    return { message::code::accepted };
}


lexer_t::result lexer_t::scan_string(xio::token_t & token)
{
    const char* i = cursor.c;
    const char q = *i;
    const char* c;
    std::string litteral;
    if (i == nullptr)
        return {};
    //if (result r; !(r = scan_unexpected(token))) return r;

    if (*i != '\'' && *i != '"') return {};
    ++i; c = i;
    while (!cursor.eof(i) && (*i && (*i != q))) {
        if (*i == '\\') {
            ++i;
            if (cursor.eof(i)) {
                std::cout << " eof in \"" << litteral << "\"....\n";
                return {};
            }
            switch (*i) {
            case 'n':
                litteral += (char)10;
                ++cursor.l;
                cursor.col = 1;
                break;
            case 'r':
                litteral += (char)13;
                ++cursor.l;
                cursor.col = 1;
                break;
            case 't':
                litteral += (char)8;
                break;
            case 'a':
                break;//++i; ignore bell; if you want bell, just listen to tubular bells! or run with the shadow on the wall!
            case 'c':
                litteral += (char)3;
                break;
            case '0':
                litteral += (char)0;
                break;
            case 'e':
                litteral += (char)27;
                break;
            default:
                litteral += *i;
                break;
            }
            ++i;
            if (cursor.eof(i)) {
                std::cout << " eof in \"" << litteral << "\"....\n";
                return { };
            }
            continue;
        }
        if (cursor.eof(i)) {
            std::cout << " eof in \"" << litteral << "\"....\n";
            return {  };
        }

        litteral += *i;
        ++i;
        if (cursor.eof(i)) {
            std::cout << " eof in \"" << litteral << "\"....\n";
            return {  };
        }
    }

    if (*i && (*i == q)) {
        token.loc.b = cursor.c+1;
        token.loc.e = i-1;
        token.code = mnemonic::noop;
        token.type = type_t::text;
        token.sem = type_t::text | type_t::leaf;
        return { message::code::accepted };
    }
    return {};
}



lexer_t::result lexer_t::scan_identifier(xio::token_t & token)
{
    const char* i = cursor.c;
    if (isspace(*i)) return {};
    if (!isalpha(*i) && (*i != '_')) return {};

    while (*i && (isalnum(*i) || (*i == '_'))) ++i;
    int sz = static_cast<int>(i - cursor.c);
    --i;

    if (!(*i)) return {};

    token = {
        mnemonic::noop, // id's at the lexer phase haven't been qualified to anything, yet.
        type_t::id, type_t::leaf|type_t::id, opdelta::identifier,
        // i - 1 => ?
        {cursor.c, i, cursor.l, cursor.col, static_cast<int>(cursor.c - cursor.b)},
        {1,0},
        nullptr
    };
    return { message::code::accepted };
}


#define m_hex(ah) ( (((ah) >= '0') && ((ah) <= '9')) || ((((ah) >= 'a') && ((ah) <= 'f')) || (((ah) >= 'A') && ((ah) <= 'F'))) )

lexer_t::result lexer_t::scan_hex(xio::token_t & token)
{
    //LFnl << ":\n";
    if (cursor.eof()) return {}; // or token_t();

    const char* ptr = cursor.c;
    ptr += 2;
    // 0xabcdef
    //   ^
    // finalement, ici on ne se casse plus la tete avec les minuscules et les majuscules.
    // => 0xabcdef | 0xabcdef , etc... sont valides et seront convertis en u64
    // => attention: quoi faire avec 0xffd2g 'g' => l,k,m,g,p sous reserve.
    while (*ptr && m_hex(*ptr)) ++ptr;
    if (ptr == cursor.c)
        return{ };

    //charray hstr;
    //for (const char *c = cursor.c; c < ptr; c++)   hstr += *c;
    --ptr;
    token.sem |= type_t::leaf | type_t::u64 | type_t::hex;
    token.loc.b = cursor.c;
    token.loc.e = ptr;

    return { message::code::accepted }; // return {}; // oops!
    
}


lexer_t::result lexer_t::scan_unary(xio::token_t & token)
{
    if ((token.code == mnemonic::add) || (token.code == mnemonic::sub))
        if (!scan_sign(token)) return { message::code::accepted };

    if (empty()) {
        token.type = type_t::prefix;
        token.sem = type_t::prefix | type_t::unary | type_t::oper;

        return { message::code::accepted };
    }

    token_t prev = tokens->back();

    if (prev.is_operator()) {
        token.type = type_t::prefix;
        token.sem = type_t::oper | type_t::prefix | type_t::unary;
        return { message::code::accepted };
    }

    token.type = type_t::postfix;
    token.sem = type_t::oper | type_t::postfix | type_t::unary;
    return { message::code::accepted };
}



lexer_t::result lexer_t::scan_sign(xio::token_t & token)
{
    if (empty()) {
        //... systematically, we have signed unary prefix operator here.
        token.code = token.code == mnemonic::add ? mnemonic::positive : mnemonic::negative;
        token.type = type_t::prefix;
        token.sem = type_t::oper | type_t::prefix | type_t::unary | type_t::sign;
        token.delta = opdelta::prefix;

        return{ message::code::accepted };
    }

    token_t b_token = tokens->back();
    // ici, le token en queu de liste est teste si le type : prefix, binaire
    // ex.: +++a; ++(+a) 1 - +a; etc...
    if (b_token.is_closingpair())
        return {  };
    if (b_token.is_binary()) {
        token.code = token.code == mnemonic::add ? mnemonic::positive : mnemonic::negative;
        token.type = type_t::prefix;
        token.sem = type_t::oper | type_t::prefix | type_t::unary | type_t::sign;
        token.delta = opdelta::prefix;

        return { message::code::accepted };
    }
    return {  };
}


lexer_t::result lexer_t::scan_keyword(xio::token_t & token)
{
    //if (!empty()) {
    //    token_t& prev = tokens->back();

    //    if (!token.f.v) {
    //        if (prev.is_operator())
    //            return { (message::push(message::xclass::unexpected), "unhandled keyword '", token.attribute(), "':\n",  cursor.mark()) };
    //    }
    //}
    return { message::code::accepted };
}


lexer_t::result lexer_t::scan_prefix(xio::token_t & token)
{

    if (!empty()) {
        token_t* prev = &tokens->back();
        if (prev->is_leaf() || prev->is_identifier())
            return {  };
    }
    return { message::code::accepted };
}

lexer_t::result lexer_t::scan_postfix(xio::token_t & token)
{
    if (!empty()) {
        token_t& prev = tokens->back();
        if (prev.is_unary() || (prev.is_binary() && !prev.is_closingpair()))
            return {  };
    }
    return { message::code::accepted };
}


lexer_t::result lexer_t::scan_binary(xio::token_t & token)
{
    if ((token.code == mnemonic::add) || (token.code == mnemonic::sub)) (void)scan_sign(token);
    return { message::code::accepted };
}


lexer_t::result lexer_t::scan_punctuation(xio::token_t & token)
{
    return { message::code::accepted };
}

lexer_t::result lexer_t::scan_assign(xio::token_t & token)
{
    return { message::code::accepted };
}

/*!
 * 
 * @code 4.45abc(2^a);
 */

lexer_t::result lexer_t::scan_factor(xio::token_t & token)
{
    token_t vt;
    if (cursor.f) {
        // classic factor mode:
        // token expected to be of type id or leftpar:
        if (!(token.type &(type_t::id|type_t::leftpar))) {
            cursor.f = 0;
            return {  };
        }
    accepted:
        token.loc.e = cursor.c = token.loc.b;
        vt = token_t::query(mnemonic::mul);
        //logdebugfn << "accepted: token informations: " << token.informations() << Ends;
        vt.loc = token.loc;
        vt.f.m = 1;
        tokens->push_back(vt);
        std::cerr << "[" << vt.informations() << "]\n";
        return { message::code::accepted };
    }
    // token is an id or openning par.
    if (!(token.type &(type_t::id | type_t::leftpar))) {
        cursor.f = 0;
        return {  };
    }
    vt = tokens->back();
    if (!(vt.is_number())) {
        cursor.f = 0;
        return { message::code::accepted };
    }
    cursor.f = 1;
    goto accepted;
}


lexer_t::result lexer_t::push_tail(xio::token_t & token)
{
    if (!token) return { (message::push(message::xclass::error) , " will not push a null token")};

    std::size_t sz = token.loc.e - token.loc.b + 1;
    if (token.type&(type_t::text)) sz += 2;

    //if (token.loc.p < 0)
    token.loc.p = (int)(token.loc.b - cursor.b);

    cursor.c += sz;
    cursor.col += sz;
    tokens->push_back(token); // copie finale en place; placement of final copy.
    cursor.skip_ws();
    std::cerr << "[" << token.informations() << "]\n";
    return { message::code::accepted };
}


lexer_t::result lexer_t::scan_assign_rs(token_t &)
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
    token_t prev = empty() ? token_t::token_null : tokens->back();
    if ((!prev) || cursor.f) 
        return { (message::push(message::xclass::error), cursor.l, ",", cursor.col, "Unexpected assign operator:\n" + cursor.mark()) };
    
    return { message::code::accepted };
}


/*
lexer_t::result lexer_t::scan_unexpected(token_t & token)
{
    //token_t prev = empty() ? token_t::token_null : tokens->back();
    //if (!prev) return { message::xcode::accepted };
    //if (!(prev.is_number()||prev.is_operator()) && token.is_leaf() && !cursor.f) {
    //    return { (message::push(message::xclass::unexpected), cursor.l, ",", cursor.col, ": unexpected non-operator token") };
//    }
    return {message::code::accepted};
}
*/

lexer_t::result lexer_t::scan_leftpar(token_t & token)
{
    token.loc.b = token.loc.e = cursor.c;
    token.loc.p = cursor.c - cursor.b;
    token.loc.c = cursor.col;
    token.loc.l = cursor.l;

    (void)scan_factor(token);
    return scan_binary(token);
}


lexer_t::~lexer_t()
{
    scanners.clear();
}

bool lexer_t::empty()
{
    return tokens ? tokens->size() < 2 : true;
}

lexer_t & lexer_t::operator()(token_t::list_t * a_tokens)
{
    tokens = a_tokens;
    return *this;
}

lexer_t::result lexer_t::operator[](const char * a_src)
{
    init_scanners();
    src = a_src;
    return exec();
}

//lexer_t::result lexer_t::exec()
//{
//    cursor.b = cursor.c = src;
//    cursor.e = cursor.b + std::strlen(src) - 1; // important to substract 1 to set e to points at the last valid character.
//    if (!tokens)
//        return { (message::push(message::xclass::error), " address of the tokens stream is null") };
//    if (cursor.e < cursor.b)
//        return { (message::push(message::xclass::eof), " -> source code is empty") };
//
//    cursor.l = cursor.col = 1;
//    tokens->clear();
//    cursor.f = 0;
//    tokens->emplace_back(token_t{ e_code::noop, type_t::null, type_t::null, opdelta::noop, {cursor.b,cursor.e,0}, {0,0} });
//
//    if (!cursor.skip_ws())
//        return { (message::push(message::xclass::eof), " unexpected " ) };
//
//    const char* cc = nullptr;
//    do {
//        if (cc == cursor.c)
//            return  { (message::push(message::xclass::null), "\n" + cursor.mark(), "\n break infinite loop") };
//
//        cc = cursor.c;
//        //std::cerr << " cc has advanced\n";
//        token_t token = token_t::scan(cursor.c);
//
//        if (!token) {
//            if (!scan_number(token)) {
//                if (!scan_identifier(token))
//                    return { ( message::push(message::xclass::error), '(', cursor.l, ',',cursor.col, ')',"lexical analysis : unhandled token\n" + cursor.mark()) };
//                else
//                    (void)scan_factor(token); // 4ac(4ac[4ac{
//            }
//        }
//        else {
//            scanner_ptr_t scanner = lexer_t::scanners[token.type];
//            if (!scanner)
//                return  { };
//
//            if (result r; !(r = (this->*scanner)(token)))
//                return r; ///@todo handle error...
//
//            cursor.f = 0;
//            token.loc.l = cursor.l;
//            token.loc.c = cursor.col;
//            token.loc.p = static_cast<int>(token.loc.b - cursor.b);
//        }
//        push_tail(token);
//    } while (!cursor.eof());
//
//    return { message::xcode::accepted };
//}

lexer_t::result lexer_t::exec()
{
    cursor.b = cursor.c = src;
    cursor.e = cursor.b + std::strlen(src) - 1; // important to substract 1 to set e to points at the last valid character.
    if (!tokens)
        return { (message::push(message::xclass::error), " address of the tokens stream is null") };
    if ( (cursor.e < cursor.b) || (!src) || (!std::strlen(src)))
        return { (message::push(message::xclass::error), "[eof] -> source code is empty") };

    cursor.l = cursor.col = 1;
    tokens->clear();
    cursor.f = 0;
    tokens->emplace_back(token_t{ mnemonic::noop, type_t::null, type_t::null, opdelta::noop, {cursor.b,cursor.e,0}, {0,0} });

    if (!cursor.skip_ws())
        return { (message::push(message::xclass::error), " unexpected end of file" ) };

    const char* cc = nullptr;
    do {
        if (cc == cursor.c)
            return  { (message::push(message::xclass::error), "\n" + cursor.mark(), "\n break infinite loop") };

        cc = cursor.c;
        //std::cerr << " cc has advanced\n";
        token_t token;
        if (! scan_number(token)) {
            token = token_t::scan(cursor.c);
            if (!token) {
                if (lexer_t::result r; !(r = scan_identifier(token)))
                    return { (r.notice(), "\n",cursor.mark()) };
                    //return { ( message::push(message::xclass::error), '(', cursor.l, ',',cursor.col, ')',"lexical analysis : unhandled token\n" + cursor.mark()) };
                else
                    (void)scan_factor(token); // 4ac(4ac[4ac{
            }
            else {
                //logdebug << "lexer: checking scanner for type" << logger::Yellow << type_t::name(token.type) << Ends;
                auto i = lexer_t::scanners.find(token.type);
                if (i == lexer_t::scanners.end()) {
                    return  { (message::push(message::xclass::error), '(', cursor.l, ',',cursor.col, ')',"lexical analysis : no parser found for  token\n" + cursor.mark())  };
                }
                scanner_ptr_t scanner = i->second;
                if (!scanner)
                    return  { (message::push(message::xclass::error), '(', cursor.l, ',',cursor.col, ')',"lexical analysis : no parser assigned to the token table!\n" + cursor.mark()) };

                if (result r; !(r = (this->*scanner)(token)))
                    return r; ///@todo handle error...

                cursor.f = 0;
                token.loc.l = cursor.l;
                token.loc.c = cursor.col;
                token.loc.p = static_cast<int>(token.loc.b - cursor.b);
            }
        }
        (void)push_tail(token);
    } while (!cursor.eof());

    return { message::code::accepted };
}

bool lexer_t::cursor_t::operator++()
{
    if (c > e) return false;
    ++c;
    if (c < e) ++col;
    return skip_ws(); // do handle control characters.
}

bool lexer_t::cursor_t::operator++(int)
{
    return ++(*this);
}

bool lexer_t::cursor_t::operator--()
{
    return false;
}

bool lexer_t::cursor_t::operator--(int)
{
    return false;
}

bool lexer_t::cursor_t::skip_ws()
{
    if (this->eof()) return false;

    while (!this->eof() && (c <= e) && isspace(*c)) {
        switch (*c) {
        case '\n':
            ++c;
            ++l;
            f = 0;
            if (this->eof()) return false;
            if (*c == '\r') {
                ++c;
                ++l;
            }
            col = 1;
            f = 0;
            break;
        case '\r':
            ++c;
            ++l;
            if (this->eof()) return false;
            if (*c == '\n') {
                ++c;
                ++l;
            }
            f = 0;
            col = 1;
            break;
        case '\t':  // pass through the archaic tab!!!!
            if (!(*this)++) return false;
            break;
        case 0x20:
            ++c;
            ++col;
            f = 0;
            break;
        default: break;
        }
    }
    return !this->eof();

}

bool lexer_t::cursor_t::eof(const char * cc)
{
    if (!cc)
        return ((c > e) || (!*c));

    return ((cc > e) || (!*cc));
}

std::string lexer_t::cursor_t::scantoeol()
{
    const char* p = c;
    std::string line;

    while ((p < e) && ((*p != '\n') && (*p != '\r'))) ++p;

    if (p < e)
        for (; c != p; c++) line += *c;
    else
        for (; c < e; c++) line += *c;
    return line;
}


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

std::string lexer_t::cursor_t::location()
{
    string_t str;
    str = "[%d,%d]";
    str << l << col;
    return str();
}


}
