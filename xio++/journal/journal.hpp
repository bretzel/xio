//
// Created by bretzelus on 18-11-27.
//
#pragma once

#include <xio++/string_t.hpp>
#include <xio++/xtypes.hpp>
#include <map>
#include <stack>
//#include <string_view>
#include <xio++/journal/w3-colors-metro.hpp>
#include <fstream>

namespace xio {

// L << Italic << " Je suis en italique" - Italic 

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
            Journal::Log::LogHandle LogHandle=0;
        public:
            TOC();
        
            ~TOC() {}
        private:
        };

        struct xio_api Article 
        {
            using List = std::map<std::string, Article>;

        };

        struct xio_api Errors : public Journal::Log{

        };

        struct xio_api Warnings : public Journal::Log {

        };

        struct xio_api Debug : public Journal::Log {

        };

        struct xio_api Infos : public Journal::Log {

        };

        struct xio_api Exceptions : public Journal::Log {

        };

        struct xio_api Chapter : public Journal::Log
        {
            using List = std::vector<Journal::Book::Chapter>;
            std::string mId;


        };

        struct xio_api Logs
        {

            Logs() = default;
            ~Logs();

            Journal::Log::LogHandle mLogHandle=0xFFFFFFFFFFFFFFFF;
            std::ofstream mOut;
        };
    };

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
private:
    
    using Loggers = std::map<std::string, Journal::Log>;
    Journal::Loggers mLoggers;
    static w3c::W3ClassMap _W3cMetro;
    static Journal* _Instance;
    
};




}


