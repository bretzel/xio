//
// Created by bretzel on 20-02-03.
//

#include <xio/lexer/lexer.h>
#include <stdint.h>
#include <array>

using xio::utils::xstr;
using xio::utils::notification;

namespace xio::lexer
{


lexscanners::ScannersTable lexscanners::_Scanners=
    {
        {type::number,       &lexscanners::__Number},
        {type::text,         &lexscanners::__String},
        {type::keyword,      &lexscanners::__Keyword},
        {type::unary,        &lexscanners::__Unary},
        {type::prefix,       &lexscanners::__Prefix},
        {type::postfix,      &lexscanners::__Postfix},
        {type::binary,       &lexscanners::__Binary},
        {type::leftpar,      &lexscanners::__Leftpar},
        {type::closepar,     &lexscanners::__Binary},
        {type::openabs,      &lexscanners::__Binary},
        {type::closeabs,     &lexscanners::__Binary},
        {type::assign,       &lexscanners::__Binary},
        {type::hex,          &lexscanners::__Hex},
        {type::assign,       &lexscanners::__Assign_rs},
        {type::punctuation,  &lexscanners::__Punctuation},
        {type::id,           &lexscanners::__Identifier},
        {type::number,       &lexscanners::__Number}
        
    };


/*

            =
           / \
          a   *
             / \
            a   =
               / \
              b   3

*/



//lexscanners::lexscanners(type::token_t::collection& token_stream)
//{
//
//}

lexscanners::~lexscanners()
{
    
}

bool lexscanners::Empty()
{
    return (_tokens.empty());
}

lexscanners::code lexscanners::Scan()
{
    _cursor.b = _cursor.c = _source;
    _cursor.e = _cursor.b + std::strlen(_source) - 1; // important to substract 1 to set e to points at the last valid character.
    if (_tokens.empty())
        return { (notification::push(notification::type::error), " address of the tokens stream is null") };
    if ((_cursor.e < _cursor.b) || (!_source) || (!std::strlen(_source)))
        return { (notification::push(notification::type::error), "[eof] -> source code is empty") };
    
    _cursor.l = _cursor.col = 1;
    _tokens.clear();
    _cursor._F = 0;
    _tokens.emplace_back(type::token_t{lexem::mnemonic::noop, type::null, type::null, type::delta::noop, {_cursor.c, _cursor.e, -1, -1, -1}, {0, 0, 0}, 0 });
    
    if (!_cursor.SkipWS())
        return { (notification::push(notification::type::error), " unexpected end of file") };
    
    const char* cc = nullptr;
    do {
        if (cc == _cursor.c)
            return  { (notification::push(notification::type::error), "\n" + _cursor.mark(), "\n break infinite loop") };
        
        cc = _cursor.c;
        //std::cerr << " cc has advanced\n";
        type::token_t token_t;
        if (!__Number(token_t)) {
            token_t = type::token_t::scan(_cursor.c);
            if (!token_t) {
                if (code r; !(r = __Identifier(token_t)))
                    return { (r.note(), "\n", _cursor.mark()) };
                else{
                    if(*(_cursor.c-1))
                    {
                        const char* pc = _cursor.c-1;
                        if(!isalnum(*pc))
                        {
                            _cursor._F = false;
                            
                        }
                        else
                            (void)__Factor(token_t); // 4ac(4ac[4ac{
                    }
                }
            }
            else {
                auto i = lexscanners::_Scanners.find(token_t.t);
                if (i == lexscanners::_Scanners.end()) {
                    return  { (notification::push(notification::type::error), '(', _cursor.l, ',',_cursor.col, ')', "lexical analysis : no parser found for  token_t\n" + _cursor.mark()) };
                }
                ScannerFnPtr scanner = i->second;
                if (!scanner)
                    return  { (notification::push(notification::type::error), '(', _cursor.l, ',',_cursor.col, ')', "lexical analysis : no parser assigned to the token_t table!\n" + _cursor.mark()) };
                
                if (code r; !(r = (this->*scanner)(token_t))) return r;
                _cursor._F = 0;
                switch(token_t.c)
                {
                    case lexem::mnemonic::cppcomment:
                    case lexem::mnemonic::bcomment:
                    case lexem::mnemonic::ecomment:
                        // skip this token_t.
                        _cursor.SkipWS();
                        continue;
                    default:
                        break;
                }
            }
        }
        (void)__Push(token_t);
    } while (!_cursor.Eof());
    
    return { notification::code::accepted };
}

lexscanners::code lexscanners::operator[](const char* aSrc)
{
    if (_tokens.empty()) {
        return {(
                    notification::push(),
                        notification::type::error,
                        notification::code::null,
                        " Lexical scanners invoked without type::token_t stream. "
                )};
    }
    _source = aSrc;
    return Scan();
}




lexscanners::num::num(const char *_c, const char *_eos): b(_c),e(_c),c(_c), eos(_eos) {}


bool lexscanners::num::operator++()
{
    ++c;
    if(c > eos) return false;
    if(!isalnum(*c))
    {
        if(c<eos)
        {
            if(*c !='.')
            {
                st = state::bad;
                return false;
            }
            // else *c is on '.' :
            if(!real)
            {
                e = c;
                st = state::good;
                real = true;
                return true;
            }
        }
        st = state::bad;
        return false;
    }
    e = c;
    return true;
}


bool lexscanners::num::operator++(int)
{
    ++c;
    if(c > eos) return false;
    
    if(!isalnum(*c))
    {
        if(c<eos)
        {
            if(*c !='.')
            {
                st = state::bad;
                return false;
            }
            // else *c is on '.' :
            if(!real)
            {
                e = c;
                st = state::good;
                real = true;
                return true;
            }
        }
        st = state::bad;
        return false;
    }
    e = c;
    return true;
}


lexscanners::num::state lexscanners::num::update_state()
{
    
    numbase base = numeric_base();
    if(base == numbase::none)
    {
        std::cout << __PRETTY_FUNCTION__ << ": No digit '" << *c << "'\n";
        state::bad;
        return st;
    }
    
    if(n < base) n=base;
    st = state::good;
    return st;
}


lexscanners::num::numbase  lexscanners::num::numeric_base()
{
    
    if(isdigit(*c))
    {
        int cv = *c -'0';
        if (cv <= 1)
            return numbase::dec;
        if(cv<=7)
            return numbase::dec;
        
        return numbase::dec;
    }
    
    if(isalpha(*c))
    {
        return ((*c >= 'A') && (*c <= 'F')) || ( (*c >= 'a') && (*c <= 'f') )  ? numbase::hex : numbase::none;
    }
    
    return none;
}


lexer::type::T lexscanners::num::operator()()
{
    switch(n)
    {
        case bin:
            return type::bin;
        case oct:
            return type::octal;
        case hex:
            return type::hex;
        case dec:
            return type::number;
        default:
            //if (e==b)
                return type::number;
            
    }
    return type::null;
}


bool lexscanners::num::ok(){ return ((c<=eos) && (update_state() == state::good)); }

/*!
 * @brief ...
 * @param a_token
 * @return
 */
lexscanners::code lexscanners::__Number(type::token_t& a_token)
{
    
    type::T btype=type::u64;
    
    num number(_cursor.c, _cursor.e);
    while(number.ok()) number++;
    if(number)
        a_token.s = number() | (number.real ? type::real : 0);

    btype = number();
    
    ///@todo Determiner le num value-size
    a_token = {
        lexem::mnemonic::noop, type::number,
        type::number | type::leaf | btype | (number.real ? type::real : type::u64), // u64: Par defaut. Mais les parsers pourront "squizer" en evaluant la valeur et la comparer avec les limtes u8, u/i16;32;64...
        type::delta::identifier,
        {_cursor.c, number.e, _cursor.l, _cursor.col, _cursor.c - _cursor.b},
        {1,0,0},
        nullptr
    };
    
    if( !(a_token.s & type::real))
    {
        xstr str;
        str << a_token.attribute();
        uint64_t D=0;
        std::istringstream i(str.c_str());
        switch(number.n)
        {
            case num::bin:
                //????????? ah!
                break;
            case num::oct:
                i >> std::oct >> D;
                break;
            case num::dec:
                i >> D;
                break;
            case num::hex:
                i >> std::hex >> D;
                break;
            default:
                str >> D;
                break;
        }
        
        //std::cout << __PRETTY_FUNCTION__ << " Parsed number:" << D << '\n';
        uint64_t n = 0;
        std::array<uint64_t,3> R = {0x100,0x10000,0x100000000};
        while(D >= R[n])
            ++n;
        
        switch(n)
        {
            case 0:a_token.s = (a_token.s & ~type::u64) | type::u8;
                break;
            case 1:a_token.s = (a_token.s & ~type::u64) | type::u16;
                break;
            case 2:a_token.s = (a_token.s & ~type::u64) | type::u32;
                break;
            default:a_token.s = (a_token.s & ~type::u64) | type::u64;
                break;
        }
    }
    ///@todo SCAN SCIENTIFIC NOTATION !!!!!!
    
    return { notification::code::accepted };
}

lexscanners::code lexscanners::__String(type::token_t& a_token)
{
    
    const char* i = _cursor.c; // i on the quote lexem
    const char q = *i;
    const char* c;
    std::string litteral;
    //LogDebugFn << " Cursor on " << i << Ends;
    ///*if (i == nullptr)
    //    return {};*/
    //if (result r; !(r = scan_unexpected(a_token))) return r; // Sous reserve
    ++i; c = i;
    a_token._loc.begin = _cursor.c;
    while (!_cursor.Eof(i) && (*i && (*i != q))) {
        std::cerr << "i on '" << *i << "'\n";
        if (*i == '\\') {
            ++i;
            if (_cursor.Eof(i)) {
                std::cout << " eof in \"" << litteral << "\"....\n";
                return {};
            }
            switch (*i) {
                case 'n':
                    litteral += (char)10; ++i;
                    ++_cursor.l;
                    _cursor.col = 1;
                    break;
                case 'r':
                    litteral += (char)13; ++i;
                    ++_cursor.l;
                    _cursor.col = 1;
                    break;
                case 't':
                    litteral += (char)8; ++i;
                    break;
                case 'a':
                    ++i;
                    break;//++i; ignore bell; if you want bell, just listen then Tubular Bells! or run with the Shadow on the Wall!
                case 'c':
                    litteral += (char)3; ++i;
                    break;
                case '0':
                    litteral += (char)0; ++i;
                    break;
                case 'e':
                    litteral += (char)27; ++i;
                    break;
                    ///@Todo  Continuer d'analyser les characteres de controle.
                default:
                    litteral += *i++;
                    break;
            }// end switch
            continue;
        } // end if '\\';
        litteral += *i++;
    }
    if (_cursor.Eof(i)) {
        std::cout << " eof ->  scanned: \"" << litteral << "\"....\n";
        goto UnterminatedError;
    }
    //--i;
    std::cerr << "end : i on '" << *i << "'\n";
    //if (/* *i && (*/*i == q/*)*/) {
    a_token._loc.begin = _cursor.c; // +1;
    a_token._loc.end = i;
    a_token._loc.L = _cursor.l;
    a_token._loc.C = _cursor.col;
    
    a_token.c = lexem::mnemonic::noop;
    a_token.t = type::text;
    a_token.s = type::text | type::leaf;
    std::cerr << "Litteral accepted:token_t[" << a_token.attribute() << "]\n";
    return { notification::code::accepted };
    //}
    UnterminatedError:
    a_token._loc.begin = _cursor.c;
    a_token._loc.end = _cursor.c + 4;
    a_token._loc.I = _cursor.c - _cursor.b;
    a_token._loc.L = _cursor.l;
    a_token._loc.C = _cursor.col;
    
    return {(
                notification::push(),
                    notification::type::error,": ",
                    notification::code::unterminated_litteral,'\n',
                    a_token.mark()
            )};
}

lexscanners::code lexscanners::__Identifier(type::token_t& a_token)
{
    const char* C = _cursor.c;
    if( !(std::isalpha(*C++)) && (*C != '_') ) return {};
    while (std::isalnum(*C) || (*C=='_')) ++C;
    
    if (C <= _cursor.c) return {};
    --C;
    a_token._loc.begin = _cursor.c;
    a_token._loc.end = C;
    a_token._loc.C = _cursor.col;
    a_token._loc.L = _cursor.l;
    a_token._loc.I = _cursor.Index(); ///@todo Reduce overhead.
    a_token.c = lexem::mnemonic::noop;
    a_token.t = type::id;
    a_token._f = { 1,0,0 };
    a_token.s = type::leaf | type::id;
    a_token.d = type::delta::identifier;
    //LogDebugFn << " Attribute: [" << a_token.attribute() << ']' << Ends;
    return { notification::code::accepted };
}

lexscanners::code lexscanners::__Hex(type::token_t& a_token)
{
    
    #define m_hex(ah) ( (((ah) >= '0') && ((ah) <= '9')) || ((((ah) >= 'a') && ((ah) <= 'f')) || (((ah) >= 'A') && ((ah) <= 'F'))) )
    
    const char* ptr = _cursor.c;
    if(ptr[0] == '0' && ( (ptr[1] == 'x') || (ptr[1] == 'X')))  ptr += 2;
    // 0xabcdef
    //   ^
    // finalement, ici on ne se casse plus la tete avec les minuscules et les majuscules.
    // => 0xAbCDEf | 0xabcdef , etc... sont valides et seront convertis en u64
    // => attention: quoi faire avec 0xffd2g 'g' => l,k,m,g,p sous reserve.
    while (*ptr && m_hex(*ptr)) ++ptr;
    if ((ptr == _cursor.c) || ((_cursor.c[0] == '0' && ((_cursor.c[1] == 'x') || (_cursor.c[1] == 'X'))) && (ptr == _cursor.c + 2) ))
        return{(
                   notification::push(),
                       notification::code::syntax,
                       notification::type::error, " (lexscanners::__Hex): ", _cursor.mark()
        
               )};
    
    //charray hstr;
    //for (const char *c = cursor.c; c < ptr; c++)   hstr += *c;
    --ptr;
    a_token.s |= type::leaf | type::u64 | type::hex;
    a_token._loc.begin = _cursor.c;
    a_token._loc.end = ptr;
    
    return { notification::code::accepted }; // return {}; // oops!
}

lexscanners::code lexscanners::__Unary(type::token_t& a_token)
{
    if ((a_token.c == lexem::mnemonic::add) || (a_token.c == lexem::mnemonic::sub))
        if (!__Sign(a_token)) return { notification::code::accepted };
    
    if (Empty()) {
        a_token.t = type::prefix;
        a_token.s = type::prefix | type::unary | type::oper;
        a_token._loc.L = _cursor.l;
        //a_token._loc.begin = _cursor.c;
        
        a_token._loc.C = _cursor.col;
        return { notification::code::accepted };
    }
    
    type::token_t prev = _tokens.back();
    
    if (prev.is_operator()) {
        a_token.t = type::prefix;
        a_token.s = type::oper | type::prefix | type::unary;
        return { notification::code::accepted };
    }
    
    a_token.t = type::postfix;
    a_token.s = type::oper | type::postfix | type::unary;
    return { notification::code::accepted };
}

lexscanners::code lexscanners::__Sign(type::token_t& a_token)
{
    if (Empty()) {
        //... systematically, we have signed unary prefix operator here.
        a_token.c = a_token.c == lexem::mnemonic::add ? lexem::mnemonic::positive : lexem::mnemonic::negative;
        a_token.t = type::prefix;
        a_token.s = type::oper | type::prefix | type::unary | type::sign;
        a_token.d = type::delta::prefix;
        
        return{ notification::code::accepted };
    }
    
    type::token_t b_token = _tokens.back();
    // ici, le a_token en queu de liste est teste si le type : prefix, binaire
    // ex.: +++a; ++(+a) 1 - +a; etc...
    if (b_token.is_closing_pair() || b_token.is_leaf() || b_token.is_postfix())
        return {  };
    
    a_token.c = a_token.c == lexem::mnemonic::add ? lexem::mnemonic::positive : lexem::mnemonic::negative;
    a_token.t = type::prefix;
    a_token.s = type::oper | type::prefix | type::unary | type::sign;
    a_token.d = type::delta::prefix;
    return {notification::code::accepted};
}

lexscanners::code lexscanners::__Keyword(type::token_t& a_token)
{
    if (!Empty()) {
        type::token_t& pre = _tokens.back();
        if (!a_token._f.V) {
            if (pre.is_operator()) {
                return { (
                             notification::push(),
                                 notification::type::error,
                                 notification::code::unexpected,
                                 "non-operand on right hand side of an operator.\n",
                                 a_token.mark()
                         ) };
            }
        }
    }
    return { notification::code::accepted };
}

lexscanners::code lexscanners::__Prefix(type::token_t& a_token)
{
    if (!Empty()) {
        type::token_t* prev = &_tokens.back();
        if (prev->is_leaf() || prev->is_identifier())
            return {  };
    }
    return { notification::code::accepted };
}

lexscanners::code lexscanners::__Postfix(type::token_t& a_token)
{
    if (!Empty()) {
        type::token_t& prev = _tokens.back();
        if (prev.is_unary() || (prev.is_binary() && !prev.is_closing_pair()))
            return {  };
    }
    return { notification::code::accepted };
}

lexscanners::code lexscanners::__Binary(type::token_t& a_token)
{
    if ((a_token.c == lexem::mnemonic::add) || (a_token.c == lexem::mnemonic::sub)) (void)__Sign(a_token);
    return { notification::code::accepted };
}




/*!
 * @brief
 *
 * @note Rough testing... I totally forgot cpp/c comments!!!!!
 * @param token_t
 * @return
 */
lexscanners::code lexscanners::__Punctuation(type::token_t& token_t)
{
    switch(token_t.c)
    {
        case lexem::mnemonic::cppcomment:
            (void)_cursor.scan_to_eol();
            return notification::code::accepted;
        case lexem::mnemonic::bcomment:_cursor.scan_to(lexem::ecomment);
            return notification::code::accepted;
        
        case lexem::mnemonic::ecomment:
            // skip this token_t.
            _cursor.SkipWS();
            return notification::code::accepted;
        default:
            break;
    }
    
    token_t._loc = {_cursor.c, _cursor.c, _cursor.l, _cursor.col, _cursor.c - _cursor.b };
    return { notification::code::accepted };
}

lexscanners::code lexscanners::__Assign(type::token_t& a_token)
{
    a_token._loc = {_cursor.c, _cursor.c, _cursor.l, _cursor.col, _cursor.c - _cursor.b };
    return { notification::code::accepted };
}

lexscanners::code lexscanners::__Factor(type::token_t& a_token)
{
    type::token_t vt;
    //LogDebugPFn << "Arg: mark: " << BR << a_token.mark() << Ends;
    if (_cursor._F) {
        // classic factor mode:
        // a_token expected to be of type id or leftpar:
        //LogDebugFn << " Factor flag was set:" << Ends;
        
        if(*(_cursor.c-1))
        {
            const char* pc = _cursor.c-1;
            if(!isalnum(*pc))
            {
                _cursor._F = false;
                return {};
            }
        }
        if (!(a_token.t | type::id | type::leftpar))
        {
            //LogDebugFn << " Not Factor sequence." << Ends;
            _cursor._F = false;
            return {  };
        }
        accepted:
        a_token._loc.end = _cursor.c = a_token._loc.begin;
        vt = type::token_t()[lexem::mnemonic::mul];
        
        vt._loc = a_token._loc;
        //LogDebug << "Arg: " << a_token._loc.pos() << ", VToken: " << vt._loc.pos() << Ends;
        vt._f.M = 1;
        _tokens.push_back(vt);
        //std::cerr << "[" << vt.attribute() << "] is a Virtual Multiply Operator.\n";
//        //LogDebugFn
//            << " Pushed virtual multiply op on '"
//            << lus:://Log::color::Yellow << a_token.attribute()
//            << lus:://Log::color::Reset << '\'' << BR
//            << vt.mark()
//            << Ends;
        
        return { notification::code::accepted };
    }
    // a_token is an id or openning par.
    if (!(a_token.t & (type::id | type::leftpar))) {
        _cursor._F = false;
        return {  };
    }
    
    vt = _tokens.back();
    if (!(vt.is_number())) {
        _cursor._F = false;
        return { notification::code::accepted };
    }
    _cursor._F = true;
    goto accepted;
}


lexscanners::code lexscanners::__Push(type::token_t& a_token)
{
    if (!a_token) return { (notification::push(notification::type::error) , " will not push a null token_t") };
    a_token._loc.L = _cursor.l;
    a_token._loc.C = _cursor.col;
    std::size_t sz = a_token._loc.end - a_token._loc.begin + 1;
    //if (a_token.t & (type::text)) sz += 2;
    
    //if (token_t.loc.p < 0)
    a_token._loc.I = (int)(a_token._loc.begin - _cursor.b);
    
    _cursor.c += sz;
    _cursor.col += sz;
    //LogDebugFn << " '" << lus:://Log::color::Yellow << a_token.attribute() << lus:://Log::color::Reset << "'" << Ends;
    _tokens.push_back(a_token); // copie finale en place; placement of final copy.
    _cursor.SkipWS();
    //std::cerr << "[" << token_t.informations() << "]\n";
    return { notification::code::accepted };
}

lexscanners::code lexscanners::__Assign_rs(type::token_t& a_token)
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
    type::token_t prev = Empty() ? type::token_t::_null : _tokens.back();
    if ((!prev) || _cursor._F) {
        return { (
                     notification::push(),
                         notification::type::error,", ",
                         _cursor.l, ",", _cursor.col, "Unexpected assign operator:\n", _cursor.mark()
                 ) };
    }
    a_token._loc = {_cursor.c, _cursor.c, _cursor.l, _cursor.col, _cursor.c - _cursor.b };
    return { notification::code::accepted };
}

lexscanners::code lexscanners::__Leftpar(type::token_t& token_t)
{
    token_t._loc = {_cursor.c, _cursor.c, _cursor.l, _cursor.col, _cursor.c - _cursor.b };
    
    (void)__Factor(token_t);
    return __Binary(token_t);
}

lexscanners::code lexscanners::__CppComment(type::token_t &a_token)
{
    return lexscanners::code();
}
lexscanners::code lexscanners::debug(lexscanners::fn_t fn)
{
    if(!fn)
        return
        {(
            notification::push(), notification::type::warning, " - tokens stream is empty"
        )};
    
    for(auto tk : _tokens) fn(tk);
    
    return notification::code::ok;
}






// -------------------------- CALLABLE SCANNERS --------------

lexscanners::token_code lexscanners::scan_number()
{
    type::token_t token;
    code c;
    _cursor.SkipWS();
    if( (c = __Number(token))) return token;
    return {c.note()};
}


lexscanners::token_code lexscanners::scan_string()
{
    type::token_t token;
    code c;
    if ( (c = __String(token)) ) return token;
    return { c.note() };    
}

lexscanners::token_code lexscanners::scan_identifier()
{
    type::token_t token;
    code c;
    _cursor.SkipWS();
    if ( (c = __Identifier(token)) ) return token;
    return { c.note() };
}


lexscanners::token_code lexscanners::scan_hex()
{
    type::token_t token;
    code c;
    if ( (c = __Hex(token)) ) return token;
    return { c.note() };
}


lexscanners::token_code lexscanners::scan_unary()
{
    type::token_t token;
    code c;
    if ( (c = __Unary(token)) ) return token;
    return { c.note() };
}


lexscanners::token_code lexscanners::scan_sign()
{

    return {( notification::push(), notification::code::implement )};
}


lexscanners::token_code lexscanners::scan_keyword()
{
    type::token_t token;
    code c;
    if ( (c = __Keyword(token)) ) return token;
    return { c.note() };

}
lexscanners::token_code lexscanners::scan_prefix()
{
    type::token_t token;
    code c;
    if ( (c = __Prefix(token)) ) return token;
    return { c.note() };

}
lexscanners::token_code lexscanners::scan_postfix()
{
    type::token_t token;
    code c;
    if ( (c = __Postfix(token)) ) return token;
    return { c.note() };

}
lexscanners::token_code lexscanners::scan_binary()
{
    type::token_t token;
    code c;
    if ( (c = __Binary(token)) ) return token;
    return { c.note() };

}
lexscanners::token_code lexscanners::scan_punctuation(lexem::mnemonic mm)
{
    type::token_t token=token[mm];
    code c;
    if(!token)
        return {(
            notification::push(notification::type::error), " Lexical scanners: Invalid punctuation code"
                )};
    if ( (c = __Punctuation(token)) ) return token;
    return { c.note() };

}
lexscanners::token_code lexscanners::scan_assign()
{
    type::token_t token;
    code c;
    if ( (c = __Assign_rs(token)) ) return token;
    return { c.note() };

}
lexscanners::token_code lexscanners::scan_factor()
{
    type::token_t token;
    code c;
    if ( (c = __Factor(token)) ) return token;
    return { c.note() };

}
lexscanners::token_code lexscanners::scan_push()
{
    type::token_t token;
    code c;
    if ( (c = __Push(token)) ) return token;
    return { c.note() };
}



lexscanners::token_code lexscanners::scan_cppComment()
{
    return {( notification::push(), notification::code::implement )};
}
bool lexscanners::eof()
{
    return _cursor.Eof();    
}

lexscanners::token_code lexscanners::scan_types(type::T sem, receiver_fn_t receiver_fn)
{
    type::token_t token;
    std::vector<type::T> bits;
    type::T bit;
    uint64_t numbers = 0x7F40;
    for(uint64_t x = 0; x < 64 ; x++)
    {
        bit = (uint64_t)1 << x;
        if ( bit & sem )
        {
            if ( (bit & 0x7F40) && (! (numbers & 1)) )
            {
                numbers |= type::number;
                bits.push_back(type::number);
            }
            else
                bits.push_back(bit);
        }
    }
    code r;
    if ( bit );
    for(auto T: bits)
    {
        auto i = lexscanners::_Scanners.find(T);
        ScannerFnPtr scanner = nullptr;
        if ( (i == lexscanners::_Scanners.end()))
            r = {(
                    notification::push(notification::type::warning),
                    '(', _cursor.l, ',', _cursor.col, ')',
                    "lexical analysis : no scanner found for token\n",
                    _cursor.mark() // ca va passer ici...testons donc cela. :)}
                 )};
        else
            scanner = i->second;
        if (!scanner)
        {
            r = {(
                    notification::push(notification::type::warning), '(', _cursor.l, ',', _cursor.col, ')',
                    "lexical analysis : no scanner assigned to the lexical scanners table (nullptr):\n - aborting scan - \n",
                    _cursor.mark()
                )};
            continue;
        }
        
        if(receiver_fn)
        {
            receiver_fn(token, (r = (this->*scanner)(token)));
            _cursor._F = false;
            if(r)
                break;
        }
        
    }
    if(r)
        return token;
    return r.note();
}


lexscanners::code lexscanners::finalize(type::token_t &token)
{
    return __Push(token);
}
void lexscanners::reset()
{
    _cursor.b = _cursor.c = _source;
    _cursor.e = _cursor.b + std::strlen(_source) - 1; // important to substract 1 to set e to points at the last valid character.
    if (_tokens.empty())
    {
        notification::push(notification::type::error), " address of the tokens stream is null";
        return;
    }
    if ((_cursor.e < _cursor.b) || (!_source) || (!std::strlen(_source)))
    {
        notification::push(notification::type::error), "[eof] -> source code is empty";
        return;
    }
    _cursor.l = _cursor.col = 1;
    _tokens.clear();
    _cursor._F = 0;
    _tokens.emplace_back(type::token_t{lexem::mnemonic::noop, type::null, type::null, type::delta::noop, {_cursor.c, _cursor.e, -1, -1, -1}, {0, 0, 0}, 0 });
    
    if (!_cursor.SkipWS())
        notification::push(notification::type::error), " unexpected end of file";
}



bool lexscanners::internal_cursor::operator++()
{
    if (c > e) return false;
    ++c;
    if (c < e) ++col;
    return SkipWS(); // do handle control characters.
}

bool lexscanners::internal_cursor::operator++(int)
{
    return ++(*this);
}

bool lexscanners::internal_cursor::SkipWS()
{
    if (Eof()) return false;
    
    while (!Eof() && (c <= e) && isspace(*c)) {
        switch (*c) {
            case '\n':
                ++c;
                ++l;
                _F = 0;
                if (Eof()) return false;
                if (*c == '\r') {
                    ++c;
                    ++l;
                }
                col = 1;
                _F = 0;
                break;
            case '\r':
                ++c;
                ++l;
                if (Eof()) return false;
                if (*c == '\n') {
                    ++c;
                    ++l;
                }
                _F = 0;
                col = 1;
                break;
            case '\t':  // pass through the archaic tab!!!!
                if (!(*this)++) return false;
                break;
            case 0x20:
                ++c;
                ++col;
                _F = 0;
                break;
            default: break;
        }
    }
    return !Eof();
    
}

bool lexscanners::internal_cursor::Eof(const char* P)
{
    if (!P)
        return ((c > e) || (!*c));
    
    return ((P > e) || (!*P));
}

int lexscanners::internal_cursor::Index()
{
    ///@todo verify pointers...
    return static_cast<int>(c - b);
}

std::string lexscanners::internal_cursor::scan_to_eol()
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

std::string lexscanners::internal_cursor::line()
{
    const char* b = c;
    std::string str;
    while ((b > b) && (*b != '\n') && (*b != '\r')) --b;
    const char* e = b;
    if(e > b) ++e;
    while ((e < e) && (*e != '\n') && (*e != '\r')) str += *e++;
    return str;
}

std::string lexscanners::internal_cursor::mark()
{
    xstr Str = line();
    std::string str;
    str.insert(str.begin(), static_cast<int>((c - 1) - b), 0x20);
    Str << '\n' << str << '^';
    return Str();
}


std::string lexscanners::internal_cursor::location()
{
    xstr str;
    str << l << ',' << col;
    return str();
}


notification::code lexscanners::internal_cursor::scan_to(const char *substr)
{
    
    std::size_t pos = std::string(c).find(substr);
    if(pos == std::string::npos)
        return notification::code::rejected;
    
    c += pos;
    return notification::code::rejected;
}


}
