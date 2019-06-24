cmake_minimum_required(VERSION 3.10)
# Project name 
project(XIO++ VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

message("CMake Module Path:" ${CMAKE_MODULE_PATH})

add_library(MineField SHARED IMPORTED)

message("MineField:" ${MINEFIELD_INCLUDES})


# 
# target_link_libraries(xio++ ${CMAKE_DL_LIBS})
# 
# target_include_directories(xio++
# PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#     $<INSTALL_INTERFACE:include>
# )
# 
# 
# include(GNUInstallDirs)
# 
# 
# install(TARGETS xio++
#     EXPORT xio++
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    PUBLIC_HEADER DESTINATION include/xio++
#)

# install(FILES ${H_XIO} DESTINATION include/xio++)
# install(FILES ${H_COMPILER} DESTINATION include/xio++/interpreter/compiler)
# install(FILES ${H_KERNEL} DESTINATION include/xio++/interpreter/kernel)
# install(FILES ${H_JOURNAL} DESTINATION include/xio++/journal)
# install(FILES ${H_INTER} DESTINATION include/xio++/interpreter)



# install(EXPORT xio++
#   FILE
#     xio++.cmake
#   NAMESPACE
#     xio::
#   DESTINATION
#     ${CMAKE_INSTALL_LIBDIR}/cmake/xio++
# )


# add_subdirectory(xio.exe)

