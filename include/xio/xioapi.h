//
// Created by slussier on 19-11-20.
//

#pragma once

//#pragma warning(once: 4251)





#ifdef _WIN32
#pragma warning(disable : 4996)
	//class __declspec(dllimport) std::thread;
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS

#ifdef XIO_EXPORT
#define xioapi __declspec(dllexport)
#else
#define xioapi __declspec(dllimport)
#endif
#else
#define xioapi
#endif

// Mandatory. Globally used where it needs to.

//---------------------------------------------

//#ifndef xiodecl_id
//#define xiodecl_id\
//	private:\
//        static  const  std::string LUS_CLASSID; \
//    public:\
//        virtual const std::string& LUS_CLASSNAME() const noexcept; \
//    private:
//
//#define LUS_CLASSNAME_IMPL(CLASS) \
//         const std::string  CLASS::LUS_CLASSID = #CLASS;\
//         const std::string&  CLASS::LUS_CLASSNAME() const noexcept{ return CLASS::LUS_CLASSID; }
//#endif

#ifndef _WIN32
#define _strdup  strdup
#include <thread>
#include <cstdint>
#else
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

// #endif //XIO_XIOAPI_H
