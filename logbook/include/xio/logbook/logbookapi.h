
#pragma once

#define _CRT_SECURE_NO_WARNING

#ifdef _WIN32
#ifdef LOGBOOKAPI_EXPORT
#define logbook_api __declspec(dllexport)
#else
#define logbook_api __declspec(dllimport)
#endif
#else
#define logbook_api
#endif

#ifndef _WIN32
#define _strdup  strdup
#else
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif


