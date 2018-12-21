#pragma once
#include <xio++/xio++.hpp>
#include <xio++/expect.hpp>
#include <xio++/xtypes.hpp>
#include <map>


namespace xio {


class xio_api lexer_t
{
    token_t::list_t*    tokens = nullptr;
    const char*         src = nullptr;

    struct xio_api cursor_t {
        const char* c = nullptr; ///< Current char pointer position in source.
        const char* e = nullptr; ///< Char pointer on the last character in source.
        const char* b = nullptr; ///< Char pointer on the beginning of the source.
        int l = -1;      ///< Current line number in the source.
        int col = -1;      ///< Current Coloumn number in source.

        bool operator ++();
        bool operator ++(int);
        
        
        // -- Disabled:
        bool operator --();
        bool operator --(int);
        //-----------------------------------

        bool skip_ws();
        //bool BackSkip(); ///< Danger : not tested; Disabled.
        bool eof(const char* CC = nullptr); ///< End Of File test. Default is testing with the current `C` pointer.

        std::string scantoeol();
        //std::string ScanTo(const std::string& TargetStr);

        std::string line();
        std::string mark();
        std::string location();
        bool f = false;

    }cursor;

public:
    using result = expect<message::xcode>;
    

    using scanner_ptr_t = lexer_t::result(lexer_t::*)(token_t&);
    using scanner_table = std::map <type_t::T, scanner_ptr_t>;
    scanner_ptr_t ss = nullptr;
private:
    scanner_table scanners;


    lexer_t::result scan_number(token_t& token);
    lexer_t::result scan_string(token_t& token);
    lexer_t::result scan_identifier(token_t& token);
    lexer_t::result scan_hex(token_t& token);
    lexer_t::result scan_unary(token_t& token);
    lexer_t::result scan_sign(token_t& token);
    lexer_t::result scan_keyword(token_t& token);
    lexer_t::result scan_prefix(token_t& token);
    lexer_t::result scan_postfix(token_t& token);
    lexer_t::result scan_binary(token_t& token);
    lexer_t::result scan_punctuation(token_t& token);
    lexer_t::result scan_assign(token_t& token);
    lexer_t::result scan_factor(token_t& token);
    lexer_t::result push_tail(token_t& token);
    lexer_t::result scan_assign_rs(token_t& token);
    lexer_t::result scan_unexpected(token_t& token);
    lexer_t::result scan_leftpar(token_t& token);

    //struct xio_api flags {
    //    int8_t s : 1; ///< 1 = factor mode....
    //}f;


public:
    lexer_t() = default;
    ~lexer_t();

    bool empty();

    lexer_t& operator()(token_t::list_t*);
    lexer_t::result operator[](const char*);
    lexer_t::result exec();

    void init_scanners();
    token_t::list_t*& token_stream() { return tokens; }
    const char* text() { return src;  }
    std::string mark(token_t& token);
};

}