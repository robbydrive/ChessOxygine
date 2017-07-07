cmake_minimum_required (VERSION 2.6)
project (Chess)

add_subdirectory(../../../oxygine/oxygine-framework/ oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

add_definitions(--std=c++11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")

add_executable(Chess ../src/main.cpp ../src/Piece.cpp ../src/Piece.h ../src/res.cpp ../src/res.h ../src/Cell.cpp ../src/Cell.h ../src/Chessboard.cpp ../src/Chessboard.h ../src/GameManager.cpp ../src/GameManager.h)
target_link_libraries(Chess ${OXYGINE_CORE_LIBS})

FILE(COPY ../data DESTINATION .)
#macro(copy_files GLOBPAT DESTINATION)
#	file(GLOB COPY_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${GLOBPAT})
#	foreach(FILENAME ${COPY_FILES})
#		set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
#		set(DST "${DESTINATION}/${FILENAME}")
#		add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST})
#	endforeach(FILENAME)
#endmacro(copy_files)
#
#set(DATA_FILES_LIST ../data/images ../data/fonts ../data/res.xml)
#copy_files("${DATA_FILES_LIST}" ${CMAKE_CURRENT_BINARY_DIR})


if (WIN32) #disable console mode for VC++
	set_target_properties(Chess PROPERTIES WIN32_EXECUTABLE TRUE)
endif(WIN32)



if (EMSCRIPTEN)
	SET(CMAKE_EXECUTABLE_SUFFIX ".html")	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_EXIT_RUNTIME=1 -s WARN_ON_UNDEFINED_SYMBOLS=1 --memory-init-file 0 -s TOTAL_MEMORY=50331648")
	em_link_pre_js(Chess  ${OXYGINE_JS_LIBRARIES}  ${CMAKE_CURRENT_SOURCE_DIR}/data.js)
endif(EMSCRIPTEN)