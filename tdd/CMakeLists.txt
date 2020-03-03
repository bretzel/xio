# Project name
#project(tdd VERSION 0.1.0 LANGUAGES CXX)


#include(GNUInstallDirs)
#FILE(GLOB HDRS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB SRCS *.cc)
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

#target_compile_definitions(${PROJECT_NAME} PUBLIC "${PROJECT_NAME}_DEBUG=$<CONFIG:Debug>")
#target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#include(GenerateExportHeader)
#add_dependencies(tdd journal grammar lexer utils)

add_executable(tdd.out ${SRCS})
add_dependencies(tdd.out utils sqlitedb logbook )

list(APPEND XIO_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/include" "${XIO_ROOT}/utils/include" "${XIO_ROOT}/xdb/sqlite3/include" "${XIO_ROOT}/lexer/include")
target_include_directories(tdd.out PUBLIC
		"$<BUILD_INTERFACE:${XIO_INCLUDE_PATHS}>"
		)

target_link_libraries(tdd.out ${CMAKE_DL_LIBS} utils sqlitedb logbook sqlite3)#journal grammar lexer utils sqlitedb)


IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
			${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
			)
ENDIF()


install(TARGETS tdd.out
		RUNTIME DESTINATION bin
		)



#install(DIRECTORY
#		include/${FrameWorkName}/${PROJECT_NAME}/
#		DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${FrameWorkName}/${PROJECT_NAME}"
#		#        FILES_MATCHING PATTERN "*.h"
#		)
#
#
#install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
#		RUNTIME DESTINATION bin
#		INCLUDES DESTINATION include
#		)
#
#
#string(REPLACE "++" "" NS PROJECTNAME)

