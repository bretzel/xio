//
// Created by bretzelus on 18-11-27.
//
#pragma once

#include <map>
#include <stack>
#include <xio++/string_t.hpp>
#include <xio++/xtypes.hpp>

//#include <string_view>
#include <xio++/journal/w3-colors-metro.hpp>
#include <fstream>

/*
    Journal Classes:
    PascalCase.
    
*/

namespace xio {


enum class TextAttr : int8_t {
    Nan = 0,
    Italic,
    Sub,
    Sup,
    Code,
    Strong,
    End
};



enum class JournalBookSection : int8_t {

};

class xio_api Journal 
{


    struct OutStr
    {
         std::vector<std::string> _Lines;
         OutStr();
         ~OutStr();
         template<typename T> Journal::OutStr& operator << (const T& dt);

    };


public:

    struct xio_api Log
    {

        
        using LogHandle     = type_t::T;  ///< 0xFFFFFFFF = null/invalid;
        using Dictionary    = std::map<Journal::Log::LogHandle, Log>;
        using HandleNames   = std::map<std::string, Journal::Log::LogHandle>;
        using AttrStack     = std::stack<TextAttr>;
        
        private: static std::map<TextAttr, std::string_view> _AttrText;
        AttrStack mAttrStack;
        
        TextAttr PopAttr();
        void     PushAttr(TextAttr AAttr);
public:        
        struct xio_api Config
        {
            using Database = std::map<std::string, Journal::Log::Config>;
        };

        ~Log();

        Journal::Log& operator << (TextAttr A);
                
        std::string operator[](W3CMetro K);


        template<typename T> Journal::Log& operator << (const T& dt)
        {
            mText << dt;
            std::cout << mText() << '\n';
            return *this;
        }
        string_t mText;
    };

   

    struct xio_api Book 
    {

        struct xio_api TOC
        {

        private:
            std::ofstream _Out;
            struct xio_api Item
            {
                std::string mDescription;
                std::string mPGLink;
                int         mPgNum = -1;
                using item = std::map<std::string, std::string>;
                using List = std::vector<Journal::Book::TOC::Item>;
            public:
                Item() {}

                ~Item() {}
            };


            Journal::Book::TOC::Item::List mEntries;
            Journal::Log::LogHandle LogHandle = 0;
        public:
            TOC();

            ~TOC() {}
        private:
        }_TOC;

        struct xio_api Article 
        {
            using List = std::map<std::string, Journal::Book::Article>;
            Journal::OutStr _InLine;
        };

        struct xio_api Errors {
            Journal::OutStr _InLine;
        };

        struct xio_api Warnings {
            using List = std::map<std::string, Journal::Book::Warnings>;
        };

        struct xio_api Debug  {
            using List = std::map<std::string, Journal::Book::Debug>;
        };

        struct xio_api Infos  {
            using List = std::map<std::string, Journal::Book::Infos>;
        };

        struct xio_api Exception {
            using List = std::map<std::string, Journal::Book::Exception>;
        };

        struct xio_api Chapter : public Journal::Log
        {
            using List = std::vector<Journal::Book::Chapter>;
            std::string mId;


        };

        struct xio_api Logs
        {
        private:
            Logs() = default;
            ~Logs();

            Journal::Log::LogHandle mLogHandle=0xFFFFFFFFFFFFFFFF;
            std::ofstream _Out;
        }_Logs;
    }_Book;

private:
    Journal();
public:
    ~Journal();

    std::string_view operator[](W3CMetro K);
    //   /Book/Stream
    //Journal::Log& operator[](Journal::Log::LogHandle);
    Journal::Log& operator[](const std::string& APath);

    //Journal::Log& operator||(type_t::T);

    static Journal& Instance();
    static Journal& Init();
    static int      Close();
private:
    
    using Loggers = std::map<std::string, Journal::Log>;
    Journal::Loggers mLoggers;
    static w3c::W3ClassMap _W3cMetro;
    static Journal* _Instance;
    
};




template<typename T>
inline Journal::OutStr& Journal::OutStr::operator<<(const T& dt)
{
    string_t str;
    str << dt;
    _Lines.push_back(str());
    return *this;
}

}


/*
BACKUP
  <div class="card spc" style="width: 60rem;">
    <i class="card-img-top fas fa-angle-double-up"></i>
    <!-- <img src="..." class="card-img-top" alt="..."> -->
    <div class="card-body">
      <h5 class="card-title">Lexical Analyser:</h5>
      <p class="card-text">
        XIO's lexical and language elements. &lt;fake article...&gt;
      </p>
      <a href="https://github.com/bretzel/xio" class="btn btn-primary fab fa-linode txt">
        <span class="icon-txt">The Source Code</span>
      </a>
    </div>
  </div>

  <div class="card spc" style="width: 60rem;">
    <!-- <img src="..." class="card-img-top" alt="..."> -->
    <div class="card-body">
      <h5 class="card-title">The Syntax Analyser (AST):</h5>
      <p class="card-text">(Abstract Syntax Tree)</p>

      <p>
        The XIO::AST involves seven classes/structs in two pairs of header/implementation files:
      </p>

      <ul>
        <li>ast.cxx: xioast</li>
        <li>ast.cxx: astnode</li>
        <li>grammar.cxx: xio_grammar</li>
        <li>grammar.cxx: rule_t</li>
        <li>grammar.cxx: seq_t</li>
        <li>grammar.cxx: term_t</li>
        <li>grammar.cxx: attr</li>
      </ul>
      <a href="https://github.com/bretzel/xio" class="btn btn-primary fab fa-github txt">
        <span class="icon-txt">Go to the Discussion</span>
      </a>
    </div>
  </div>
</div>

<div class="container" style="margin-top:50px">
  <div class="card spc" style="width: 60rem;">
    <i class="card-img-top fas fa-angle-double-up"></i>
    <!-- <img src="..." class="card-img-top" alt="..."> -->
    <div class="card-body">
      <h5 class="card-title">Lexical Analyser:</h5>
      <p class="card-text">
        XIO's lexical and language elements. &lt;fake article...&gt;
      </p>
      <a href="https://github.com/bretzel/xio" class="btn btn-primary fab fa-linode txt">
        <span class="icon-txt">The Source Code</span>
      </a>
    </div>
  </div>

  <div class="card spc" style="width: 60rem;">
    <!-- <img src="..." class="card-img-top" alt="..."> -->
    <div class="card-body">
      <h5 class="card-title">The Syntax Analyser (AST):</h5>
      <p class="card-text">(Abstract Syntax Tree)</p>

      <p>
        The XIO::AST involves seven classes/structs in two pairs of header/implementation files:
      </p>

      <ul>
        <li>ast.cxx: xioast</li>
        <li>ast.cxx: astnode</li>
        <li>grammar.cxx: xio_grammar</li>
        <li>grammar.cxx: rule_t</li>
        <li>grammar.cxx: seq_t</li>
        <li>grammar.cxx: term_t</li>
        <li>grammar.cxx: attr</li>
      </ul>
      <a href="https://github.com/bretzel/xio" class="btn btn-primary fab fa-github txt">
        <span class="icon-txt">Go to the Discussion</span>
      </a>
    </div>
  </div>



*/