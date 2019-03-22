#include "grammar.hpp"
//#include "../xio_module.hpp"

#include "../../journal/logger.hpp"

namespace xio{



xio_grammar::dictionary_t xio_grammar::grammar_dictionary = {
    
    {':', &xio_grammar::enter_rule_def      },
    {',', &xio_grammar::new_sequence        },
    {'.', &xio_grammar::end_rule            },
    {'+', &xio_grammar::set_repeat          },
    {'*', &xio_grammar::set_optional        },
    {'?', &xio_grammar::set_oneof           }, // One of
    {'\'',&xio_grammar::enter_litteral      },
    {'"' ,&xio_grammar::enter_litteral      }
    
};

rule_t::list_t xio_grammar::_rules;
static bool built = false;
// Rough, first esquisse / sketch of my rules set for the x.i.o language interpreter.
std::string grammar_txt =
"stmts              : +statement.\n"
"statement          : assignstmt ';', declvar ';', expression ';', instruction ';', var_id ';', ';'.\n"
"assignstmt         : declvar assign expression, var_id assign expression.\n"
"declvar            : *typename new_var.\n"
"funcsig            : *typename function_id '(' *params ')'.\n"
"declfunc           : funcsig ';', funcsig bloc.\n"
"paramseq           : ',' param.\n"
"param              : *typename identifier.\n"
"params             : param *+paramseq.\n"
"objcarg            : identifier ':' expression.\n"
"arg                : objcarg, expression.\n"
"argseq             : ',' arg.\n"
"args               : arg *+argseq.\n"
"typename           : *'static' ?'i8' ?'u8' ?'i16' ?'u16' ?'i32' ?'u32' ?'i64' ?'u64' ?'real' ?'number' ?'string' ?objectid.\n"
"instruction        : ?'if' ?'switch' ?'for' ?'while' ?'repeat' ?'until' ?'do'.\n" // to be continued.
"if                 : 'if' condexpr ifbody, 'if' '(' condexpr ')' ifbody.\n"
"bloc               :  '{' stmts '}'.\n"
"truebloc           : *'then' bloc, *'then' statement.\n"
"elsebloc           : *'else' bloc, *'else' statement.\n"
"ifbody             : truebloc elsebloc, truebloc.\n"
"condexpr           : assignstmt, expression.\n"
"expression         : +value.\n" // ???
"value              : ?var_id ?objectid ?function_id ?number ?string.\n" // ??? 
"var_id             .\n"
"new_var            : identifier.\n"
"objectid           .\n"
"function_id        : '::' functionid, objectid '::' functionid, var_id '.' function_id.\n"
"objcfncall         : '[' function_id  *args ']'.";
//"function_id        .";



xio_grammar::result xio_grammar::build()
{
    //if (_text.empty())
    //    return {
    //        (message::push(message::xclass::error), "rules source code is empty!")
    //    };

    logdebug
        << logger::HCyan << __FUNCTION__
        << logger::White << ": [ Building rules :" << logger::brk()
        << logger::Yellow << grammar_txt
        << logger::White << ']'
        << Ends;

    if (built()) return {(
        message::push(message::xclass::warning),
        "x.i.o. grammar is already built"
    )};

    _text = grammar_txt;
    std::size_t count = _text.words(tokens, ":;,|.+*?", true);
    string_t::list list;
    for (auto s : tokens) list.push_back(s());
    if (!count) 
        return { (message::push(message::xclass::error),"rules source code is empty!" ) };
    
    auto s = list.begin();
    _state = st_begin;
    do{
//         logdebug
//             << logger::HCyan << __FUNCTION__
//             << logger::White << "do-loop: ["
//             << logger::Yellow << *s
//             << logger::White << ']'
//             << Ends;

        result r;
        auto p = xio_grammar::grammar_dictionary.find((*s)[0]);

        if (p != xio_grammar::grammar_dictionary.end()) {
            r = (this->*(p->second))(s);
        }
        else {
            r = parse_identifier(s);
        }
        if (!r)
            return r;

    } while (s != list.end());
    dump();
    return { message::code::accepted };
}

void xio_grammar::dump()
{
    loginfopfnx << Ends;
    
    loginfo << logger::HBlue << "mnemonic" << logger::Yellow << ',' <<
        logger::HRed << "rule" << logger::Yellow << ',' << 
        logger::HGreen << "semantic" << logger::Reset << ":" <<  Ends;

    for (auto rule : _rules) {
        loginfo << logger::HCyan << rule.second->_id << logger::White << ':';
        for (auto seq : rule.second->lists) {
            logger() << logger::HCyan << "{ " << logger::Yellow;// << Ends;
            for (auto t : seq.terms) {
                logger() << logger::Yellow << t() << ' ';// << Ends;
            }
            logger() << logger::HCyan << " }" << logger::White;// << Ends;
        }
        logger() << logger::brk();
    }
}


xio_grammar::result xio_grammar::parse_identifier(string_t::iterator & crs)
{
    //logdebugfn << logger::White << " : token: '" << logger::Yellow << *crs << logger::White << "':" << Ends;
    rule_t* r = query_rule(*crs);
    //logdebugfn << logger::White << " rule: " << logger::Yellow << (r ? r->_id : "null")  << logger::White << ":" << Ends;
    switch (_state) {
        case st_begin:
            if (r) {
                if(!r->empty())
                    return { (message::push(message::xclass::error), " rule, named: ", *crs, " already exists in the context of a new rule definition.") };
                _rule = r;
            }
            else {
                _rule = new rule_t(*crs);
                _rules[*crs] = _rule;
                //logdebugfn << logger::White << " : st_begin : rule '" << logger::Yellow << _rule->_id << logger::White << "' created. next machine-state : st_init_rule..." << Ends;
            }
            a.reset();
            _state = st_init_rule; //  expect ':' as next token in main loop.
            break;
        case st_init_rule:
            _state = st_seq;
            break;
        case st_option:
        case st_seq:
            _state = st_seq;
            // lexem::T ?
            /*mnemonic c = lexem::code(crs->c_str());
            if( c != mnemonic::knull ) {
                _rule->a = a;
                (*_rule) | c;
                a.reset();
                break;
            }*/


            type_t::T t = type_t::strtotype(*crs);
            if (t!=type_t::bloc) // Quick and dirty hack about bypassing the type_t::bloc type: 
            {
                if (t != 0) {
                    _rule->a = a;
                    (*_rule) | t;
                    a.reset();
                    break;
                }
            }
            
            //logdebug << " ***code: " << static_cast<uint64_t>(c) << " ***" << Ends;
            if (r) {
                _rule->a = a;
                (*_rule) | r;
                a.reset();
                break;
            }
            else{ 
                r = new rule_t(*crs);
                _rules[*crs] = r;
                _rule->a = a;
                _state = st_seq; //  expect ':' as next token in main loop.
                (*_rule) | r;
                a.reset();
            }
            break;
            //return { (message::push(message::xclass::error), "identifier '", *crs, "' is invalid in this context") };       
    }
    ++crs;
    return { message::code::accepted };
}


xio_grammar::result xio_grammar::enter_rule_def(string_t::iterator &crs)
{
//     logdebug
//         << logger::HCyan << __FUNCTION__
//         << logger::White << ": ["
//         << logger::Yellow << *crs
//         << logger::White << ']'
//         << Ends;
    if (_state != st_init_rule) {
        return { (message::push(message::xclass::error), "syntax error '", *crs, "' is invalid in this context") };
    }
    _state = st_seq;
    a.reset();
    ++crs;
    return { message::code::accepted };
}

xio_grammar::result xio_grammar::new_sequence(string_t::iterator & crs)
{
//     logdebug
//         << logger::HCyan << __FUNCTION__
//         << logger::White << ": ["
//         << logger::Yellow << *crs
//         << logger::White << ']'
//         << Ends;

    if(_state == st_option)
        return { (message::push(message::xclass::error), "syntax error '", *crs, "' is invalid in this context") };

    _rule->new_sequence();
    _state = st_seq;
    a.reset();
    ++crs;
    return { message::code::accepted };
}


xio_grammar::result xio_grammar::end_rule(string_t::iterator & crs)
{
//     logdebug
//         << logger::HCyan << __FUNCTION__
//         << logger::White << ": ["
//         << logger::Yellow << *crs
//         << logger::White << ']'
//         << Ends;
    _state = st_begin;
    ++crs;
    return { message::code::accepted };
}

xio_grammar::result xio_grammar::set_repeat(string_t::iterator & crs)
{
//     logdebug
//         << logger::HCyan << __FUNCTION__
//         << logger::White << ": ["
//         << logger::Yellow << *crs
//         << logger::White << ']'
//         << Ends;
    _state = st_option;
    +a;
    ++crs;
    return { message::code::accepted };
}

xio_grammar::result xio_grammar::set_optional(string_t::iterator & crs)
{
//     logdebug
//         << logger::HCyan << __FUNCTION__
//         << logger::White << ": ["
//         << logger::Yellow << *crs
//         << logger::White << ']'
//         << Ends;
    *a;
    ++crs;
    _state = st_option;
    return { message::code::accepted };
}





xio_grammar::result xio_grammar::enter_litteral(string_t::iterator & crs)
{

//     logdebug
//         << logger::HCyan << __FUNCTION__
//         << logger::White << ": ["
//         << logger::Yellow << *crs
//         << logger::White << ']'
//         << Ends;

    if((_state != st_seq) && (_state != st_option))
        return {(
            message::push(message::xclass::error), 
            "syntax error '", 
            *crs, 
            "' is not a valid xio++ grammar token in context", 
            "(state machine:",(int)_state,
            ")"
        )};

    string_t::iterator i = crs;
    //logdebugfn << logger::HBlue << "token: '" << logger::HRed << *i << logger::HBlue << "'" << Ends;
    ++i;
    // logdebugfn << logger::HBlue << "token[++i]: '" << logger::HRed << *i << logger::HBlue << "'" << Ends;
    if((*i=="'") || (*i=="\""))
        return { (message::push(message::xclass::error), "error: litteral x.i.o grammar element cannot be empty") };

    //logdebugfn << logger::White << " Checking token: '" << logger::Yellow << *i << logger::White << "'" << Ends;
    token_t token = token_t::scan(i->c_str());
    if(token){
        _rule->a = a;
        (*_rule) | token.code;
        a.reset();
    }
    else
        return {(
            message::push(message::xclass::error), 
            "syntax error '", 
            *i, 
            "' is not a valid xio++ grammar token"
        )};

//      logdebugfn << logger::White << "term_t : '" << logger::Yellow << *i << logger::White << "':" << Ends;
    crs = i;
    ++crs;
    if((*crs=="'") || (*crs=="\""))
        ++crs;
    //++crs; // will be on the next token.

    return {message::code::accepted};

}


xio_grammar::result xio_grammar::set_oneof(string_t::iterator & crs)
{
//     logdebug
//         << logger::HCyan << __FUNCTION__
//         << logger::White << ": ["
//         << logger::Yellow << *crs
//         << logger::White << ']'
//         << Ends;
    ~a;
    ++crs;
    return { message::code::accepted };
}



xio_grammar::xio_grammar()
{
}


xio_grammar::~xio_grammar()
{
}

int xio_grammar::init()
{
    return 0;
}

rule_t * xio_grammar::query_rule(const std::string & a_id)
{
    auto i = _rules.find(a_id);
    return i == _rules.end() ? nullptr : i->second;
}


term_t::term_t()
{
}

term_t::term_t(rule_t * r, attr a_)
{
    a = a_;
    mem.r = r;
    _type = term_t::type::rule;
}



term_t::term_t(type_t::T a_sem, attr a_)
{
    a = a_;
    mem.sem = a_sem;
    _type = term_t::type::sem;
    
}

term_t::term_t(mnemonic a_code, attr a_)
{
    a = a_;
    mem.c = a_code;
    _type = term_t::type::code;
}


term_t::term_t(const std::string & a_lexem)
{
    _type = term_t::type::code;
    mem.c = lexem::code(a_lexem.c_str());
}


term_t::term_t(term_t && _t)
{
//     logdebugfn << ":" << Ends;
    using std::swap;
    swap(mem, _t.mem);
    _type = _t._type;
    swap(a, _t.a);

}

term_t::term_t(const term_t & _t)
{
    mem    = _t.mem;
    _type  = _t._type;
    a      = _t.a;
}

term_t & term_t::operator=(term_t && _t)
{
    using std::swap;
    swap(mem, _t.mem);
    _type =  _t._type;    
    swap(a, _t.a);
    return *this;
}

term_t & term_t::operator=(const term_t & _t)
{
    mem = _t.mem;
    _type = _t._type;
    a = _t.a;
    return *this;
}

bool term_t::operator==(const term_t& t) const
{
    if (_type != t._type)
        return false;

    switch (_type) {
        case type::code:
            return mem.c == t.mem.c;
        case type::rule:
            return mem.r == t.mem.r;
        case type::sem:
            return (mem.sem & t.mem.sem) != 0;
        case type::nil:
            return false;
    }
    return false;
}

bool term_t::operator==(const token_t& t) const
{

    switch (_type) {
    case type::code:
        return mem.c == t.code;
    case type::sem:
        return (mem.sem & t.sem) != 0;
    case type::nil:
        return false;
    default:
        return false;
    }
    return false;
}

bool term_t::operator!=(const token_t& t) const
{
    switch (_type) {
    case type::code:
        return mem.c != t.code;
    case type::sem:
        return (mem.sem & t.sem) == 0;
    case type::nil:
        return true;
    default:
        return true;
    }
    return true;
}


term_t::~term_t()
{

}

std::string term_t::operator()() const
{
    string_t str;
    str << a();
    
    std::map<term_t::type, std::string> _{
        {term_t::type::rule, logger::attribute(logger::HRed)},
        {term_t::type::sem,  logger::attribute(logger::HGreen)},
        {term_t::type::code, logger::attribute(logger::HBlue)}
    };

    str << _[_type];
    switch (_type) {
        case term_t::type::code:
        {
            token_t tok = token_t::query(mem.c);
            str << tok.attribute();

        }
        break;
        case term_t::type::rule:
            // Can't happen but we never know: (nullptr)
            if(mem.r)
                str <<  mem.r->_id;
            break;
        case term_t::type::sem:
            str << type_t::name(mem.sem);
            break;
        default:
            str << "nil";
            break;
    }
    
    return str();
}



rule_t::rule_t(const std::string& a_id)
{
    _id = a_id;
    lists.push_back(seq_t());
    seq = lists.begin();
}

rule_t::~rule_t()
{
    lists.clear();
    _id.clear();
}

rule_t & rule_t::new_sequence()
{
    lists.push_back(seq_t());
    seq = --lists.end();
    a.reset();
    return *this;
}


rule_t & rule_t::operator|(rule_t * _r)
{
    term_t t = term_t(_r);
    t.a = a;
    a.reset();
    *seq << t;
    return *this;
}

rule_t & rule_t::operator|(type_t::T _t)
{
    term_t t = term_t(_t);
    t.a = a;
    a.reset();
    *seq << t;
    return *this;
}

rule_t & rule_t::operator|(mnemonic _t)
{
    term_t t = term_t(_t);
    t.a = a;
    a.reset();
    *seq << t;
    return *this;
}

term_t seq_t::next(term_t::const_iterator& it) const
{
    if (it != terms.end())
        ++it;
    return *it;
}

seq_t & seq_t::operator<<(term_t a_t)
{
    terms.push_back(a_t);
    return *this;
}

seq_t & seq_t::operator<<(type_t::T a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

seq_t & seq_t::operator<<(mnemonic a_t)
{
    terms.emplace_back(a_t);
    return *this;

}

seq_t & seq_t::operator<<(rule_t * a_t)
{
    terms.emplace_back(a_t);
    return *this;

}


std::string attr::operator()()
{
    string_t str;
    if (z)
        str << "*";
    if (r)
        str << "+";
    if (l)
        str << "?";
    if (x)
        str << "!";
    return str();
}

}
