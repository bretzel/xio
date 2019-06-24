cmake_minimum_required(VERSION 3.10)
# Project name 

project(XIO++ VERSION 0.1.0 LANGUAGES CXX)


#target_compile_features(XIO++ PUBLIC cxx_std_17)


include(GNUInstallDirs)

message("CMake Module Path:" ${CMAKE_MODULE_PATH})

add_library(MineField SHARED IMPORTED)

message("MineField:" ${MINEFIELD_INCLUDES})

