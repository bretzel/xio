#pragma once
#include <xio++/xtypes.hpp>
#include <xio++/expect.hpp>
//#include <xio++/interpreter/kernel/xio.hpp>
#include <map>
#include <stack>
#include <memory>




/************************************* NOTE *****************************************************
//---------------------------- xio++ GRAMMAR RULES ------------------------------------------------
 AFK
statement       : declare instruction  expression;  *stmt.
stmt            : ';' statement.
stack           : '{' statement '}'.
list_init       : '{' expression *expr_list '}'';' .
expr_list       : ',' expression.
instruction     : if|switch|do|while|return|repeat|until|for|assign.
assign          : lvalue assign expression .
lvalue          : declare | identifier.
//declare       : *typename identifier. >>>>>>>>>>> NE PAS OUBLIER DE PRENDRE EN CHARGE LA NOTATION SCIENTIFIQUE!!! <<<<<<<<<<<
declvar         : *typename identifier; | typename id ASSIGN expression;.
declfunc        : declfn ( *params );.
implfunc        : function_id ( *params ) stack.
params          : declare *param.
param           : , params.
objccall        : [identifier *objcparams].
objcparams      : identifier  expression *objcparamseq.
objcparamseq    : ,objcparams.
functioncall    : function_id ( *objcparams ) | [identifier *objcparams]| identifier.
Operator        : Binary | Unary .
Binary          :  term BINARY term .
Unary           :  PREFIX term | term POSTFIX .
expression      : term | Binary | Unary .
term            : functioncall | (typename)expression |  (expression) | expression | value .
identifier      :  function_id,  object_id,  variable_id ..
 

 //----if statement as xio++ interpreter:------------

 

 stmts          : +statement.
 statement      : assign-stmt ;, expression ;, instruction, ;.
 assign-stmt    : declvar assign expression, var_identifier assign expression.
 declvar        : *?type var-identifier-new.
 type           : *i8 *u8 *i16 *u16 *i32 *u32 *i64 *u64 *real *string *text *object-id.
 
 if : kif expression if-body
 bloc :  { stmts }.
 true-bloc : *kthen bloc, *kthen statement.
 else-bloc : *else bloc, *else statement.
 if-body   : true-bloc else-bloc, true-bloc.
 cond-expr : expression.

 var-identifier.
 var-identifier-new
 object-identifier.
 function-identifier.




 //--------------------------------------------------

 one-of : *t *t2 *t3 *t4 *t5 *t6
 
 */

namespace xio{




struct xio_api attr {
    int8_t z : 1; ///< Zero or one (optional * )
    int8_t r : 1; ///< Repeat      (        + )
    int8_t l : 1; ///< List        (one of  ~ ?)
    int8_t x : 1; ///< Exclude     (        ! ) 
    int8_t s : 8; ///< Litteral List Separator
    attr& operator | (attr const & a)
    {
        z |= a.z;
        r |= a.r;
        l |= a.l;
        x |= a.x;
        //S = A.S;
        return *this;
    }
    attr& operator +() { r = 1; return *this; }
    attr& operator *() { z = 1; return *this; }
    attr& operator !() { x = 1; z = r = 0; return *this; }
    attr& operator ~() { l = 1; return *this; }
    void reset() { z = r = l = x = s = 0; }
    std::string operator()();

};


struct xio_api rule_t;
class xio_api xio_grammar;


struct xio_api term_t {
    attr a = { 0,0,0,0,0 }; ///< default : punctual, strict match

    enum class type :uint8_t {
        //term,
        rule = 0,
        sem,
        code,
        nil
    };


    term_t::type _type = term_t::type::nil;

    union mem_t {
        rule_t* r;
        type_t::T sem;
        e_code c;
    }mem = { nullptr };

    using list_t = std::vector<term_t>;
    using iterator = list_t::iterator;
    using const_iterator = list_t::const_iterator;

    // pas le choix... 'y faut un parser....

    term_t();
    term_t(rule_t* r, attr a_ = { 0,0,0,0,0 });
    term_t(type_t::T a_sem, attr a_ = { 0,0,0,0,0 });
    term_t(e_code a_code, attr a_ = { 0,0,0,0,0 });

    term_t(const std::string& a_lexem);


    term_t(term_t&& _t);
    term_t(const term_t& _t);

    term_t& operator = (term_t&& _t);
    term_t& operator = (const term_t& _t);

    ~term_t();

    term_t& operator *() { *a; return *this; }
    term_t& operator +() { +a; return *this; }
    term_t& operator !() { !a; return *this; }
    term_t& operator ~() { ~a; return *this; }

    std::string operator()();


    static term_t query(const char*);
    static term_t query(type_t::T);
    static term_t query(e_code);

};

//struct xio_api term_list_t {
//    attr a = { 0,0,0,0,0 }; ///< default : punctual, strict match
//    term_t::list_t terms;
//    term_t::iterator t;
//
//    void push_back(term_t _t) {
//        terms.push_back(_t);
//    }
//
//    term_t::iterator begin() { return terms.begin(); }
//
//};

struct  xio_api seq_t {


    attr a = { 0,0,0,0,0 }; ///< default : punctual, strict match

    //using data = std::vector<term_list_t>;
    
    term_t::list_t terms;

    using list_t = std::vector<seq_t>;
    using const_iterator = list_t::const_iterator;
    using iterator = list_t::iterator;
    using stack = std::stack<iterator>;

    seq_t() = default;

    term_t::const_iterator begin() const { return terms.cbegin(); }

    bool end(term_t::const_iterator t) const {
        return terms.cend() == t;
    }

    ~seq_t() {
        terms.clear();
    }
    seq_t& operator << (term_t a_t);
    
    // Emplace_back:
    seq_t& operator << (type_t::T a_t);
    seq_t& operator << (e_code a_t);
    seq_t& operator << (rule_t* a_t);

};


struct xio_api  rule_t {

    seq_t::list_t   lists;
    // ---------------------------------------
    seq_t::iterator seq; // Temporary held for building this rule.
    // ---------------------------------------
    //seq_t::stack seq_stack;

    using list_t = std::map<std::string, rule_t*>;
    using iterator = list_t::const_iterator;
    
    attr a = { 0,0,0,0,0 }; /// Volatile attribute infos. ( Copied into the rule's recursion context  ) 
    attr inject = { 0,0,0,0,0 };

    std::string _id;

    rule_t() = default;
    rule_t(const std::string& a_id);

    rule_t(int) {}
    ~rule_t();
    bool empty() const { return lists.empty() ? true : lists.begin()->terms.empty(); }
    void inject_attr(attr a_a) { inject = a_a; }
    rule_t& new_sequence();
    rule_t& operator |(rule_t* _r);
    //rule_t& operator |(const char*   _t);
    rule_t& operator |(type_t::T _t);
    rule_t& operator |(e_code _t);

    seq_t::const_iterator begin() const { return lists.cbegin(); }
    bool end(seq_t::const_iterator s) const { return s == lists.cend();}
};


class xio_api xio_grammar
{
public:
    using result = expect<message::code>; // accepted, rejected.
    xio_grammar();
    ~xio_grammar();
    string_t& text() { return _text; }
    xio_grammar::result build();
    void dump();
    
private:

    enum state_mac {
        st_begin=0,   ///< about to define new rule: cursor on rule id.
        st_init_rule, ///< initialize sequence of terms: cursor on ':'.
        st_seq,       ///< back and forth between st_seq<->st_init_term. 
        st_option,    ///< one of ' *+? ' option state.
        st_init_term, ///< query {rule | type semantic | e_code | litteral} : cursor on identifier.
        st_add_seq,   ///< terminate current sequence and start a new sequence on the rule: cursor on ','.
        st_end_rule   ///< terminate rule : cursor on '.'.
    };

    attr a = { 0,0,0,0,0 };///< default : punctual, strict match

    state_mac _state = xio_grammar::st_begin;

    int init();
    static rule_t::list_t _rules;
    rule_t*        _rule = nullptr;
    rule_t*        query_rule(const std::string& a_id);
    
    using grammar_t = xio_grammar::result(xio_grammar::*)(string_t::iterator&);
    string_t::word::list  tokens;
    string_t             _text;

    using dictionary_t = std::map < char, xio_grammar::grammar_t>;
    static dictionary_t grammar_dictionary;
public :
    
    const rule_t* operator[](const std::string& r_id) { 
        return _rules[r_id];
        //rule_t* r = _rules[r_id]; return (const rule_t*)r; 
    }

    static bool built() { return _rules.size() != 0; }

private:
    xio_grammar::dictionary_t::iterator _rule_it;

    //--------------- Rules builders -------------------

    
    xio_grammar::result parse_identifier (string_t::iterator& crs);
    xio_grammar::result enter_rule_def   (string_t::iterator &crs);
    xio_grammar::result new_sequence     (string_t::iterator& crs);
    xio_grammar::result end_rule         (string_t::iterator& crs);
    xio_grammar::result set_repeat       (string_t::iterator& crs);
    xio_grammar::result set_optional     (string_t::iterator& crs);
    xio_grammar::result enter_litteral   (string_t::iterator& crs); 
    xio_grammar::result set_oneof        (string_t::iterator& crs);

};

}



