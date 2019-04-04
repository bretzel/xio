//
// Created by bretzelus on 18-11-27.
//
#pragma once

#include <xio++/string_t.hpp>
#include <xio++/xtypes.hpp>
#include <map>

namespace xio {

// L << Italic << " Je suis en itealique" - Italic 

    enum class TextAttr : int8_t {
        Italic=1,
        Sub,
        Sup,
        Code,
        Strong
    };


    enum class JournalBookSection : int8_t {

    };

class xio_api Journal 
{

public:

    struct xio_api Log
    {

        using LogHandle = type_t::T;  ///< 0xFFFFFFFF = null/invalid;
        using Dictionary = std::map<Journal::Log::LogHandle, Log>;
        using HandleNames = std::map<std::string, Journal::Log::LogHandle>;


        struct xio_api Config
        {
            using Database = std::map<std::string, Journal::Log::Config>;
        };

        ~Log();

        Journal::Log& operator << (TextAttr A);

        template<typename T> Journal::Log& operator << (const T& dt)
        {
            mText << dt;
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

        struct xio_api Article {

        };

        struct xio_api Errors {

        };

        struct xio_api Warnings {

        };

        struct xio_api Debug {

        };

        struct xio_api Infos {

        };



        struct xio_api Chapter 
        {
            using List = std::vector<Journal::Book::Chapter>;
            std::string mId;


        };
    };


    //Journal::Log& operator[](Journal::Log::LogHandle);
    //Journal::Log& operator||(type_t::T);



};




}


xio_api xio::Journal::Log& operator^(xio::Journal::Log&, xio::TextAttr);

