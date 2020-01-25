cmake_minimum_required(VERSION 3.10)
# Project name 

project(xio VERSION 0.1.0 LANGUAGES CXX)

find_package(lus REQUIRED)


INCLUDE_DIRECTORIES(${PROJECT_NAME} include)


include(GNUInstallDirs)

message("CMake Module Path:" ${CMAKE_MODULE_PATH})

add_library(${PROJECT_NAME} SHARED

        include/xio/interpreter.c++
        include/xio/interpreter.h
)

target_compile_features(xio PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} lus ${CMAKE_DL_LIBS})

