//
// Created by slussier on 19-08-25.
//

#pragma once
#include <xio/interpreter/interpreterdll.h>

#include <xio/lexer/lexer.h>
#include <xio/utils/expect>
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
//declare       : *typename identifier. >>>>>>>>>>> NE PAS OUBLIER DE PRENDRE EN CHARGE LA NOTATION E+/E- !!! <<<<<<<<<<<
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

namespace teacc::parsers
{
struct INTERPRETERAPI attr
{
    int8_t z : 1; ///< Zero or one (optional * )
    int8_t r : 1; ///< Repeat      (        + )
    int8_t l : 1; ///< List        (one of  ~ ?)
    int8_t x : 1; ///< directive    ( ast direcive )
    int8_t s : 8; ///< Litteral List Separator
    attr &operator|(attr const &a)
    {
        z |= a.z;
        r |= a.r;
        l |= a.l;
        x |= a.x;
        //S = A.S;
        return *this;
    }
    
    attr &operator+()
    {
        r = 1;
        return *this;
    }
    attr &operator*()
    {
        z = 1;
        return *this;
    }
    attr &operator!()
    {
        x = 1;
        return *this;
    }
    attr &operator~()
    {
        l = 1;
        return *this;
    }
    void reset() { z = r = l = x = s = 0; }
    std::string operator()();
    bool is_opt() const { return z != 0; }
    bool is_one_of() const { return l != 0; }
    bool is_strict() const { return z == 0 && l == 0 && r == 0; }
    bool is_repeat() const { return r != 0; }
    bool is_accepted() const { return x != 0; }
    void accept() { x = 1; }
    void reject() { x = 0; }
};

struct rule_t;
class teacc_grammar;

struct INTERPRETERAPI term_t
{
    mutable attr a = {0, 0, 0, 0, 0}; ///< default : punctual, strict match

    enum class type : uint8_t
    {
        //term,
        rule = 0,
        sem,
        code,
        nil
    };

    void accept() { a.accept(); }
    void reject() { a.reject(); }

    term_t::type _type = term_t::type::nil;

    union mem_t {
        rule_t *r;
        lexer::type::T sem;
        lexer::lexem::mnemonic c;
    } mem = {nullptr};

    using collection = std::vector<term_t>;
    using iterator = collection::iterator;
    using const_iterator = collection::const_iterator;

    // pas le choix... 'y faut un parser....

    term_t();
    term_t(rule_t *r, attr a_ = {0, 0, 0, 0, 0});
    term_t(lexer::type::T a_sem, attr a_ = {0, 0, 0, 0, 0});
    term_t(lexer::lexem::mnemonic a_code, attr a_ = {0, 0, 0, 0, 0});

    term_t(const std::string &a_lexem);

    term_t(term_t &&_t);
    term_t(const term_t &_t);

    term_t &operator=(term_t &&_t);
    term_t &operator=(const term_t &_t);

    bool operator==(const term_t &t) const;
    bool operator==(const lexer::type::token_t &t) const;
    bool operator!=(const lexer::type::token_t &t) const;

    operator bool() { return _type != type::nil; }
    ~term_t();

    term_t &operator*()
    {
        *a;
        return *this;
    }
    term_t &operator+()
    {
        +a;
        return *this;
    }
    term_t &operator!()
    {
        !a;
        return *this;
    }
    term_t &operator~()
    {
        ~a;
        return *this;
    }

    std::string operator()() const;

    bool is_rule() const { return _type == type::rule; }
    bool is_semantic() const { return _type == type::sem; }
    bool is_mnemonic() const { return _type == type::code; }
    bool is_null() const { return _type == type::nil; }

    static term_t query(const char *);
    static term_t query(lexer::type::T);
    static term_t query(lexer::lexem::mnemonic);
};

//struct  term_list_t {
//    attr a = { 0,0,0,0,0 }; ///< default : punctual, strict match
//    term_t::collection terms;
//    term_t::iterator t;
//
//    void push_back(term_t _t) {
//        terms.push_back(_t);
//    }
//
//    term_t::iterator begin() { return terms.begin(); }
//
//};

struct INTERPRETERAPI seq_t
{

    attr a = {0, 0, 0, 0, 0}; ///< default : punctual, strict match

    //using data = std::vector<term_list_t>;

    term_t::collection terms;

    using collection = std::vector<seq_t>;
    using const_iterator = collection::const_iterator;
    using iterator = collection::iterator;
    using stack = std::stack<iterator>;

    seq_t() = default;

    term_t::const_iterator begin() const { return terms.cbegin(); }

    bool end(term_t::const_iterator t) const
    {
        return terms.cend() == t;
    }

    term_t next(term_t::const_iterator &it) const;

    ~seq_t()
    {
        terms.clear();
    }
    seq_t &operator<<(term_t a_t);

    // Emplace_back:
    seq_t &operator<<(lexer::type::T a_t);
    seq_t &operator<<(lexer::lexem::mnemonic a_t);
    seq_t &operator<<(rule_t *a_t);
};

struct INTERPRETERAPI rule_t
{

    seq_t::collection sequences;
    // ---------------------------------------
    seq_t::iterator seq; // Temporary held for building this rule.
    // ---------------------------------------
    //seq_t::stack seq_stack;

    using collection = std::map<std::string, rule_t *>;
    using iterator = collection::const_iterator;

    attr a = {0, 0, 0, 0, 0}; /// Volatile attribute infos. ( Copied into the rule's recursion context  )
    attr inject = {0, 0, 0, 0, 0};

    std::string _id;

    rule_t() = default;
    rule_t(const std::string &a_id);

    rule_t(int) {}
    ~rule_t();
    bool empty() const { return sequences.empty() ? true : sequences.begin()->terms.empty(); }
    void inject_attr(attr a_a) { inject = a_a; }
    rule_t &new_sequence();
    rule_t &operator|(rule_t *_r);
    //rule_t& operator |(const char*   _t);
    rule_t &operator|(lexer::type::T _t);
    rule_t &operator|(lexer::lexem::mnemonic _t);

    seq_t::const_iterator begin() const { return sequences.cbegin(); }
    bool end(seq_t::const_iterator s) const { return s == sequences.cend(); }
};



class INTERPRETERAPI teacc_grammar
{
public:
    using result = utils::expect<utils::notification::code>; // accepted, rejected.
    teacc_grammar();
    ~teacc_grammar();
    utils::xstr &text() { return _text; }
    utils::result_code build();
    void dump();

private:
    enum state_mac
    {
        st_begin = 0, ///< about to define new rule: cursor on rule id.
        st_init_rule, ///< initialize sequence of terms: cursor on ':'.
        st_seq,       ///< back and forth between st_seq<->st_init_term.
        st_option,    ///< one of ' *+? ' option state.
        st_init_term, ///< query {rule | type semantic | e_code | litteral} : cursor on identifier.
        st_add_seq,   ///< terminate current sequence and start a new sequence on the rule: cursor on ','.
        st_end_rule   ///< terminate rule : cursor on '.'.
    };

    attr a = {0, 0, 0, 0, 0}; ///< default : punctual, strict match

    state_mac _state = teacc_grammar::st_begin;

    int init();
    static rule_t::collection _rules;
    rule_t *_rule = nullptr;
    rule_t *query_rule(const std::string &a_id);

    using grammar_t = teacc_grammar::result (teacc_grammar::*)(utils::xstr::iterator &);
    utils::xstr::word::collection tokens;
    utils::xstr _text;

    using dictionary_t = std::map<char, teacc_grammar::grammar_t>;
    static dictionary_t grammar_dictionary;

public:
    const rule_t *operator[](const std::string &r_id) const
    {
        return _rules[r_id];
        //rule_t* r = _rules[r_id]; return (const rule_t*)r;
    }

    static bool built() { return _rules.size() != 0; }

private:
    teacc_grammar::dictionary_t::iterator _rule_it;

    //--------------- Rules builders -------------------

    teacc_grammar::result parse_identifier(utils::xstr::iterator &crs);
    teacc_grammar::result enter_rule_def(utils::xstr::iterator &crs);
    teacc_grammar::result new_sequence(utils::xstr::iterator &crs);
    teacc_grammar::result end_rule(utils::xstr::iterator &crs);
    teacc_grammar::result set_repeat(utils::xstr::iterator &crs);
    teacc_grammar::result set_optional(utils::xstr::iterator &crs);
    teacc_grammar::result enter_litteral(utils::xstr::iterator &crs);
    teacc_grammar::result set_oneof(utils::xstr::iterator &crs);
    teacc_grammar::result set_directive(utils::xstr::iterator &crs);
};

} // namespace teacc
