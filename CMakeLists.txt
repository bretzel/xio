
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
# Project name
project(libxio VERSION 0.1.0 LANGUAGES CXX)
set(FrameWorkName "xio")

option(BUILD_UTILS "Enable building utils" ON)
option(BUILD_LEXER "Enable building lexer" OFF)
option(BUILD_XDB   "Enable building xio.db drivers" ON)
option(BUILD_GRAMMAR "Enable building grammar" OFF)
option(BUILD_LOGBOOK_API "Enable building logbook api" ON)
option(BUILD_TDD "Enable building tdd" OFF)

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

