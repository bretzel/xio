//
// Created by splscsier on 19-11-20.
//

#pragma once

//#pragma warning(once: 4251)





#ifdef _WIN32
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

#ifdef LEXERAPI_EXPORT
#define LEXERAPI __declspec(dllexport)
#else
#define LEXERAPI __declspec(dllimport)
#endif
#else
#define LEXERAPI
#endif

// Mandatory. Globally used where it needs to.

//--------------------------------------------- 

#ifndef DECL_LEXER_ID
#define DECL_LEXER_ID\
	private:\
        static  const  std::string LEXER_CLASSID; \
    public:\
        virtual const std::string& LEXER_CLASSNAME() const noexcept; \
    private:

#define LEXER_CLASSNAtME_IMPL(CLASS) \
         const std::string  CLASS::LEXER_CLASSID = #CLASS;\
         const std::string&  CLASS::LEXER_CLASSNAME() const noexcept{ return CLASS::LEXER_CLASSID; }
#endif

#ifndef _WIN32
#define _strdup  strdup
#include <thread>
#include <cstdint>
#else
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

//#endif //OBSOLETE Include Guard

