add_executable(xio.exe ansi.cxx Calulator.cxx)
target_link_libraries(xio.exe xio++)

install(TARGETS xio.exe RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
