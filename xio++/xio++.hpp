#pragma once

#define _CRT_SECURE_NO_WARNING

#ifdef _WIN32
#ifdef XIO_API_EXPORT
#define xio_api __declspec(dllexport)
#else
#define xio_api __declspec(dllimport)
#endif
#else
#define xio_api
#endif

// #ifndef _decl_id
//     #define _decl_id \
//         private:\
//             static const std::string s_leaf_id;\
//         public:\
//             virtual const std::string& leaf_id();\
//         private:
//
//     #define _class_id(CLASS) \
//         const std::string CLASS::s_leaf_id=#CLASS;\
//         const std::string& CLASS::leaf_id(){ return CLASS::s_leaf_id; }
// #endif

#ifndef _WIN32
#define _strdup  strdup
#else
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif


