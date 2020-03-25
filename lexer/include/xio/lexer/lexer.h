//
// Created by bretzel on 20-02-03.
//

//#ifndef PLSC_LEXICALSCANNERS_H
//#define PLSC_LEXICALSCANNERS_H

#pragma once

#include <xio/utils/expect>
#include <xio/lexer/token_t.h>
#include <map>
#include <functional>



namespace xio::lexer
{

class LEXERAPI lexscanners
{
    type::token_t::collection _tokens;
    const char*      _source = nullptr;
    struct internal_cursor{
        const char* b = nullptr; ///> Absolute Beginning of the source text.
        const char* e = nullptr; ///> Absolute End of the soure text.
        const char* c = nullptr;
        
        bool operator ++();
        bool operator ++(int);
        bool SkipWS();
        bool Eof(const char* P=nullptr);
        int Index();
        int l = -1;
        int col = -1;
        std::string scan_to_eol();
        std::string line();
        std::string mark();
        std::string location();
        bool _F = false;
        utils::notification::code scan_to(const char *substr);
    }_cursor;
    
    struct num
    {
        const char* b;
        const char* e;
        const char* c;
        const char* eos;
        bool  real = false;
        
        bool  literal = true; ///< scan literal digits - do not validate hexadecimal...
        
        enum numbase
        {
            none, bin, oct,dec,hex,/* ..., */ //  FUCK!!!
        }n = none;

        enum state
        {
            good, bad
        }st = state::bad;
        
        
        num() = default;
        num(const char* _c, const char* _eos);
        
        ~num() = default;
        
        //bool operator++();
        bool operator++(int);
        bool ok(bool l);
        operator bool () { return (st==good) || (c>b); }
        state update_state();
        
        num::numbase numeric_base();
        lexer::type::T operator()();
    };

public:
    using code = utils::expect<utils::notification::code>;
    using token_code = utils::expect<type::token_t>;
    
    using fn_t = std::function<void(type::token_t&)>;
    using receiver_fn_t = std::function<void(type::token_t&, code)>;
    lexscanners() = default;
    //lexscanners(type::token_t::collection& token_stream);
    ~lexscanners();
    
    bool Empty();
    lexscanners::code Scan();
    lexscanners::code operator[](const char* aSrc);
    
    code debug(fn_t fn);
    const char*& source() { return _source; }
    type::token_t::collection& tokens() { return _tokens; }
private:
    using ScannerFnPtr  = lexscanners::code(lexscanners::*)(type::token_t&);
    using ScannersTable = std::map<type::T, ScannerFnPtr>;
    
    ScannerFnPtr  _ScannerPtr = nullptr;
    static ScannersTable _Scanners;
    // Scanners:
    code __Number(type::token_t&);
    code __String(type::token_t&);
    code __Identifier(type::token_t& aTk);
    code __Hex(type::token_t& aTk);
    code __Unary(type::token_t& aTk);
    code __Sign(type::token_t& aTk);
    code __Keyword(type::token_t& aTk);
    code __Prefix(type::token_t& aTk);
    code __Postfix(type::token_t& aTk);
    code __Binary(type::token_t& aTk);
    code __Punctuation(type::token_t& token_t);
    code __Assign(type::token_t& aTk);
    code __Factor(type::token_t& aTk);
public:
    code __Push(type::token_t& aTk);
private:
    code __Assign_rs(type::token_t& aTk);
    code __Leftpar(type::token_t& aTk);
    code __CppComment(type::token_t& a_token);
    
    
public:
    void       reset();
    token_code scan_number       ();
    token_code scan_string       ();
    token_code scan_identifier   ();
    token_code scan_hex          ();
    token_code scan_unary        ();
    token_code scan_sign         ();
    token_code scan_keyword      ();
    token_code scan_prefix       ();
    token_code scan_postfix      ();
    token_code scan_binary       ();
    token_code scan_punctuation  (lexem::mnemonic mm);
    token_code scan_assign       ();
    token_code scan_factor       ();
    token_code scan_push         ();
    token_code scan_assign_rs    ();
    token_code scan_leftpar      ();
    token_code scan_cppComment   ();
    bool eof();
    token_code scan_types(type::T sem,receiver_fn_t receiver_fn);
    code finalize(type::token_t& token);
    
    
};
}



//#endif //PLSC_LEXICALSCANNERS_H
