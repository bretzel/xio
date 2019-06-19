cmake_minimum_required(VERSION 3.10)
# Project name 
project(xio++ VERSION 0.1.0 LANGUAGES CXX)

add_library(
    xio++
SHARED
    xio++/dlloader.cxx
    xio++/message.cxx
    xio++/object.cxx
    xio++/string_t.cxx
    xio++/time_stamp.cxx
    xio++/xtypes.cxx
    xio++/journal/journal.cxx
    xio++/journal/logger.cxx
    xio++/interpreter/compiler/lexer.cxx
    xio++/interpreter/compiler/grammar.cxx
    xio++/interpreter/kernel/alu.cxx
    #xio++/interpreter/kernel/stack.cxx
    xio++/interpreter/kernel/bloc.cxx
    xio++/interpreter/kernel/xio.cxx
    #xio++/interpreter/module.cxx
    #xio++/interpreter/interpreter.cxx
    xio++/interpreter/kernel/variable.cxx
    xio++/interpreter/compiler/compiler.cxx
    xio++/interpreter/compiler/ast.cxx
    xio++/interpreter/kernel/calculus.cxx
)

# Someone kind enough to tell me how to NOT use file(GLOB here ?
file(GLOB H_INTER xio++/interpreter/*.hpp)
file(GLOB H_COMPILER xio++/interpreter/compiler/*.hpp)
file(GLOB H_KERNEL xio++/interpreter/kernel/*.hpp)
file(GLOB H_JOURNAL xio++/journal/*.hpp)
file(GLOB H_XIO xio++/*.hpp)



target_compile_features(xio++ PUBLIC cxx_std_17)

set_target_properties(xio++
PROPERTIES
VERSION 0.1.0
#PUBLIC_HEADER
#"xio++/dlloader.hpp;xio++/expect.hpp;xio++/message.hpp;xio++/object.hpp;xio++/string_t.hpp;xio++/time_stamp.hpp;xio++/xio++.hpp;xio++/xtypes.hpp;xio++/journal/journal.hpp;xio++/journal/logger.hpp;xio++/interpreter/interpreter.hpp;xio++/interpreter/compiler/lexer.hpp;xio++/interpreter/kernel/alu.hpp;xio++/interpreter/kernel/xio.hpp;xio++/interpreter/kernel/function_t.hpp;xio++/interpreter/kernel/stack.hpp"
)

target_link_libraries(xio++ ${CMAKE_DL_LIBS})

target_include_directories(xio++
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)


include(GNUInstallDirs)


install(TARGETS xio++
    EXPORT xio++
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    PUBLIC_HEADER DESTINATION include/xio++
)

install(FILES ${H_XIO} DESTINATION include/xio++)
install(FILES ${H_COMPILER} DESTINATION include/xio++/interpreter/compiler)
install(FILES ${H_KERNEL} DESTINATION include/xio++/interpreter/kernel)
install(FILES ${H_JOURNAL} DESTINATION include/xio++/journal)
install(FILES ${H_INTER} DESTINATION include/xio++/interpreter)



install(EXPORT xio++
  FILE
    xio++.cmake
  NAMESPACE
    xio::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/xio++
)


add_subdirectory(xio.exe)

