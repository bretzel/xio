#pragma once


#define _CRT_SECURE_NO_WARNING

#ifdef _WIN32
#   ifdef XIOSQLITE3API_EXPORT
#       define xiosqlite3_api __declspec(dllexport)
#   else
#       define xiosqlite3_api __declspec(dllimport)
#   endif
#else
#   define xiosqlite3_api
#endif

#ifndef _WIN32
#define _strdup  strdup
#else
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif


