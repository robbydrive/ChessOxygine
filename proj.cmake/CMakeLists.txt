cmake_minimum_required (VERSION 2.6)
project (Chess)

add_subdirectory(../../../oxygine/oxygine-framework/ oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS})
link_directories(${OXYGINE_LIBRARY_DIRS})

add_definitions(--std=c++11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")

add_executable(Chess ../src/main.cpp ../src/Piece.cpp ../src/Piece.h ../src/res.cpp ../src/res.h ../src/Cell.cpp ../src/Cell.h ../src/Chessboard.cpp ../src/Chessboard.h)
target_link_libraries(Chess ${OXYGINE_CORE_LIBS})



if (WIN32) #disable console mode for VC++
	set_target_properties(Chess PROPERTIES WIN32_EXECUTABLE TRUE)
endif(WIN32)



if (EMSCRIPTEN)
	SET(CMAKE_EXECUTABLE_SUFFIX ".html")	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_EXIT_RUNTIME=1 -s WARN_ON_UNDEFINED_SYMBOLS=1 --memory-init-file 0 -s TOTAL_MEMORY=50331648")
	em_link_pre_js(Chess  ${OXYGINE_JS_LIBRARIES}  ${CMAKE_CURRENT_SOURCE_DIR}/data.js)
endif(EMSCRIPTEN)