
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
# Project name
project(libxio VERSION 0.1.0 LANGUAGES CXX)
set(FrameWorkName "xio")

option(BUILD_UTILS "Enable building utils" ON)
option(BUILD_LOGBOOK_API "Enable building logbook api" ON)
option(BUILD_LEXER "Enable building lexer" ON)
option(BUILD_XDB   "Enable building xio db support" ON)
option(BUILD_GRAMMAR "Enable building xio script grammar api" OFF)
option(BUILD_TDD "Enable building tdd" ON)


SET(XIO_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(UTILS_INCLUDE_DIR ${XIO_ROOT}/utils/include})
SET(LOGBOOK_INCLUDE_DIR ${XIO_ROOT}/logbook/include})
SET(LEXER_INCLUDE_DIR ${XIO_ROOT}/lexer/include})
SET(SQLITEDB_INCLUDE_DIR ${XIO_ROOT}/xdb/sqlite3/include})
SET(GRAMMAR_INCLUDE_DIR ${XIO_ROOT}/grammar/include})
#SET(XIO_SCRIPTENGINE_INCLUDE_DIR ${XIO_ROOT}/scriptengine/include})

# testing noswap in nvim...
if(BUILD_UTILS)
    add_subdirectory(utils)
endif()
if(BUILD_LEXER)
    add_subdirectory(lexer)
endif()
if(BUILD_GRAMMAR)
    add_subdirectory(grammar)
endif()
if(BUILD_LOGBOOK_API)
    add_subdirectory(logbook)
endif()
if(BUILD_TDD)
    add_subdirectory(tdd)
endif()
if(BUILD_XDB)
    add_subdirectory(xdb)
endif()
message ("Project SourceDir:" ${PROJECT_SOURCE_DIR})



