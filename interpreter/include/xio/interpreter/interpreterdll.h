//
// Created by splscsier on 19-11-20.
//

#pragma once

//#pragma warning(once: 4251)





#ifdef _WIN32
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

#ifdef INTERPRETERAPI_EXPORT
#define INTERPRETERAPI __declspec(dllexport)
#else
#define INTERPRETERAPI __declspec(dllimport)
#endif
#else
#define INTERPRETERAPI
#endif

// Mandatory. Globally used where it needs to.

//--------------------------------------------- 

#ifndef DECL_INTERPRETER_ID
#define DECL_INTERPRETER_ID\
	private:\
        static  const  std::string INTERPRETER_CLASSID; \
    public:\
        virtual const std::string& INTERPRETER_CLASSNAME() const noexcept; \
    private:

#define INTERPRETER_CLASSNAME_IMPL(CLASS) \
         const std::string  CLASS::INTERPRETER_CLASSID = #CLASS;\
         const std::string&  CLASS::INTERPRETER_CLASSNAME() const noexcept{ return CLASS::INTERPRETER_CLASSID; }
#endif

#ifndef _WIN32
#define _strdup  strdup
#include <thread>
#include <cstdint>
#else
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

//#endif //OBSOLETE Include Guard

